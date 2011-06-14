/*
 *
 *  Created by Peter Zion on 10-08-20.
 *  Copyright 2010 Fabric 3D Inc.. All rights reserved.
 *
 */

#include "Debug.h"
#include <stdint.h>

namespace Fabric
{
  namespace Util
  {
#if defined(FABRIC_OS_WINDOWS)
    ErrorFile::ErrorFile( const char *filename )
    {
      m_hFile = ::CreateFileA( filename, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH, 0 );
    }
    ErrorFile::~ErrorFile( )
    {
      ::CloseHandle( m_hFile );
    }

    ErrorFile *ErrorFile::getInstance( )
    {
      static ErrorFile errFile( "C:\\fabric-log.txt" );
      return( &errFile );
    }
    void ErrorFile::logInternal( const char *str )
    {
      std::string   out;
      out = "[FABRIC] ";
      out += str;
      out += "\n";

      if( ::IsDebuggerPresent() )
        ::OutputDebugStringA( out.c_str() );

      if( ::GetStdHandle( STD_OUTPUT_HANDLE ) != NULL )
        printf( out.c_str() );

      if( m_hFile != INVALID_HANDLE_VALUE )
        ::WriteFile( m_hFile, out.c_str(), out.length(), 0, 0 );
    }
    void ErrorFile::Log( const char *fmt, ... )
    {
      char		msg[1024];
      va_list argList;
      
      va_start( argList, fmt );
      int length = _vsnprintf( msg, 1024, fmt, argList );
      va_end( argList );
      
      if ( length >= 0 )
        getInstance()->logInternal( msg );
    }
    
    void ErrorFile::Log( std::string const &string )
    {
      if( !string.empty() )
        getInstance()->logInternal( string.c_str() );
    }
#endif
  }
}
