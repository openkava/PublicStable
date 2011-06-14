/*
 *
 *  Created by Peter Zion on 10-12-07.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/IO/Helpers.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Base/Config.h>
#include <Fabric/Core/Util/Format.h>

#include <errno.h>
#include <stdlib.h>
#if defined(FABRIC_POSIX)
# include <sys/stat.h>
#elif defined(FABRIC_WIN32)
# include <windows.h>
#endif 

namespace Fabric
{
  
  
  namespace IO
  {
    void validateEntry( std::string const &entry )
    {
      if ( entry.length() == 0 )
        throw Exception("driectory entries cannot be empty");
      if ( entry == "." || entry == ".." )
        throw Exception("directory entries cannot be '.' or '..'");
      size_t length = entry.length();
      for ( size_t i=0; i<length; ++i )
      {
        char ch = entry[i];
#if defined(FABRIC_POSIX)
        if ( ch == '/' )
          throw Exception("directory entries cannot contain '/'");
#elif defined(FABRIC_WIN32)
        if( strchr( "<>:\"/\\|?*", ch ) != 0 )
          throw Exception("directory entries cannot contain any of '<>:\"/\\|?*'");
#else
# error "missing platform!"
#endif
      }
    }
    
    std::string const &getRootPath()
    {
      static std::string s_rootPath;
      if ( s_rootPath.length() == 0 )
      {
        char const *homeDir = 0;
#if defined(FABRIC_POSIX)
        homeDir = getenv( "HOME" );
#elif defined(FABRIC_WIN32)
        homeDir = getenv( "APPDATA" );
#else
# error "Unsupported platform"
#endif
        if ( !homeDir || !*homeDir )
          throw Exception("unable to obtain home directory");
#if defined(FABRIC_POSIX)
        s_rootPath = joinPath( homeDir, ".fabric" );
        if ( mkdir( s_rootPath.c_str(), 0777 ) && errno != EEXIST )
          throw Exception("unable to create directory " + _(s_rootPath));
#elif defined(FABRIC_WIN32)
        s_rootPath = joinPath( homeDir, "Fabric" );
        if ( !::CreateDirectoryA( s_rootPath.c_str(), NULL ) && ::GetLastError() != ERROR_ALREADY_EXISTS )
          throw Exception("unable to create directory " + _(s_rootPath));
#else
# error "Unsupported platform"
#endif
      }
      return s_rootPath;
    }
    
    std::string joinPath( std::string const &lhs, std::string const &rhs )
    {
#if defined(FABRIC_POSIX)
      static const char *s_pathSeparator = "/";
#elif defined(FABRIC_WIN32)
      static const char *s_pathSeparator = "\\";
#else
# error "Unsupported platform"
#endif

      if ( lhs.length() == 0 )
        return rhs;
      else if ( rhs.length() == 0 )
        return lhs;
      else
        return lhs + s_pathSeparator + rhs;
    }
    
    /*
    void safeCall( int fd, void (*callback)( int fd ) )
    {
      FABRIC_IO_TRACE( "safeCall( %d, %p )", fd, callback );
      pid_t childPID = fork();
      if ( childPID == 0 )
      {
        chroot( getRootPathSpec().c_str() );
        callback( fd );
        _exit(0);
      }
      else
      {
        int childStatus;
        if ( waitpid( childPID, &childStatus, 0 ) == -1 )
          throw Exception("safeCall failure");
      }
    }
    */
  };
};
