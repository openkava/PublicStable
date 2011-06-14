/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_UTIL_LOG_H
#define _FABRIC_UTIL_LOG_H

#include <Fabric/Base/Config.h>

#if defined(FABRIC_OS_NACL) ||  defined(FABRIC_OS_MACOSX) || defined(FABRIC_OS_LINUX)

# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <stdarg.h>

inline void FABRIC_LOG_BUFFER( char const *data, size_t length )
{
  char buf[1024];
  memcpy( &buf[0], "[FABRIC] ", 9 );
  memcpy( &buf[9], data, length );
  buf[9+length] = '\n';
  write( 1, buf, 9+length+1 );
}

inline void FABRIC_LOG( char const *format, ... )
{
  char buf[1024];
  va_list args;
  va_start( args, format );
  int length = vsnprintf( buf, 1024, format, args );
  va_end( args );
  if ( length >= 0 )
    FABRIC_LOG_BUFFER( buf, length );
}

inline void FABRIC_LOG( std::string const &string )
{
  FABRIC_LOG_BUFFER( string.data(), string.length() );
}

#elif defined(FABRIC_OS_WINDOWS)

# include <stdio.h>
# include <stdarg.h>
# include <windows.h>

namespace Fabric
{
  namespace Util
  {
    class ErrorFile
    {
      HANDLE    m_hFile;

      ErrorFile( const char *filename );
      ~ErrorFile( );
      static ErrorFile *getInstance();
      void logInternal( const char *data );
    public:
      static void Log( const char *fmt, ... );
      static void Log( std::string const &string );
    };
  }
}


#define FABRIC_LOG Fabric::Util::ErrorFile::Log

#else
# error "Unsupported FABRIC_OS_..."
#endif

#endif //_FABRIC_UTIL_LOG_H
