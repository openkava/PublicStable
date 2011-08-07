/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/IO/Helpers.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Base/Config.h>
#include <Fabric/Core/Util/Format.h>

#include <errno.h>
#include <stdlib.h>
#if defined(FABRIC_POSIX)
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
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
        if ( strchr( "<>:\"/\\|?*", ch ) != 0 )
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
        s_rootPath = JoinPath( homeDir, ".fabric" );
        if ( mkdir( s_rootPath.c_str(), 0777 ) && errno != EEXIST )
          throw Exception("unable to create directory " + _(s_rootPath));
#elif defined(FABRIC_WIN32)
        s_rootPath = JoinPath( homeDir, "Fabric" );
        if ( !::CreateDirectoryA( s_rootPath.c_str(), NULL ) && ::GetLastError() != ERROR_ALREADY_EXISTS )
          throw Exception("unable to create directory " + _(s_rootPath));
#else
# error "Unsupported platform"
#endif
      }
      return s_rootPath;
    }
    
    std::string JoinPath( std::string const &lhs, std::string const &rhs )
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

    void CreateDir( std::string const &fullPath )
    {
#if defined(FABRIC_POSIX)
      if ( mkdir( fullPath.c_str(), 0777 ) && errno != EEXIST )
        throw Exception("unable to create directory");
#elif defined(FABRIC_WIN32)
      if ( !::CreateDirectoryA( fullPath.c_str(), NULL ) && GetLastError() != ERROR_ALREADY_EXISTS )
        throw Exception("unable to create directory");
#endif 
    }
      
    bool DirExists( std::string const &fullPath )
    {
      bool result;
#if defined(FABRIC_POSIX)
      struct stat st;
      result = stat( fullPath.c_str(), &st ) && S_ISDIR(st.st_mode);
#elif defined(FABRIC_WIN32)
      DWORD dwAttrib = ::GetFileAttributesA( fullPath.c_str() );
      result = (dwAttrib != INVALID_FILE_ATTRIBUTES)
      	&& (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
#endif 
      return result;
    }
    
    std::vector<std::string> GetSubDirEntries( std::string const &dirPath )
    {
      std::vector<std::string> result;
#if defined(FABRIC_POSIX)
      DIR *dir = opendir( dirPath.length() > 0? dirPath.c_str(): "." );
      if ( !dir )
        throw Exception("unable to open directory");
      for (;;)
      {
        struct dirent *de = readdir( dir );
        if ( !de )
          break;
        if ( de->d_type != DT_DIR )
          continue;
        std::string entry( de->d_name );
        if ( entry == "." || entry == ".." )
          continue;
        result.push_back( entry );
      }
      closedir( dir );
#elif defined(FABRIC_WIN32)
      WIN32_FIND_DATAA fd;
      ::ZeroMemory( &fd, sizeof( fd ) );
      std::string searchGlob = JoinPath( dirPath.length() > 0? dirPath.c_str(): ".", "*" );
      HANDLE hDir = ::FindFirstFileA( searchGlob.c_str(), &fd );
      if( hDir == INVALID_HANDLE_VALUE )
        throw Exception("unable to open directory");
      do
      {
        if( !( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
          continue;

        std::string entry( fd.cFileName );
        if ( entry == "." || entry == ".." )
          continue;
        result.push_back( entry );
      } while( ::FindNextFileA( hDir, &fd ) );
      ::FindClose( hDir );
#endif
      return result;
    }
    
    static inline bool EqInsensitive( std::string const &lhs, std::string const &rhs )
    {
      return strcasecmp( lhs.c_str(), rhs.c_str() ) == 0;
    }
    
    void GlobDirPaths( std::string const &dirPathSpec, std::vector<std::string> &result )
    {
      size_t asteriskPos = dirPathSpec.rfind( '*' );
      if ( asteriskPos == std::string::npos )
        result.push_back( dirPathSpec );
      else
      {
        std::string prefixDirPathSpec;
        std::string prefixEntry;
        
#if defined(FABRIC_POSIX)
        char dirSep = '/';
#elif defined(FABRIC_WIN32)
        char dirSep = '\\';
#endif
        size_t dirSepPos = dirPathSpec.rfind( dirSep, asteriskPos );
        if ( dirSepPos == std::string::npos )
        {
          prefixDirPathSpec = "";
          prefixEntry = dirPathSpec.substr( 0, asteriskPos );
        }
        else
        {
          prefixDirPathSpec = dirPathSpec.substr( 0, dirSepPos );
          prefixEntry = dirPathSpec.substr( dirSepPos + 1, asteriskPos - (dirSepPos + 1) );
        }
        
        std::string suffixDirPath;
        size_t postDirSepPos = dirPathSpec.find( dirSep, asteriskPos );
        if ( postDirSepPos == std::string::npos )
        {
          suffixDirPath = "";
        }
        else
        {
          suffixDirPath = dirPathSpec.substr( postDirSepPos + 1 );
        }

        std::vector<std::string> prefixDirPaths;
        GlobDirPaths( prefixDirPathSpec, prefixDirPaths );
        for ( std::vector<std::string>::const_iterator it=prefixDirPaths.begin(); it!=prefixDirPaths.end(); ++it )
        {
          std::string const &prefixDirPath = *it;
          std::vector<std::string> subDirEntries;
          try
          {
            subDirEntries = GetSubDirEntries( prefixDirPath );
          }
          catch ( Exception e )
          {
            continue;
          }
          for ( std::vector<std::string>::const_iterator jt=subDirEntries.begin(); jt!=subDirEntries.end(); ++jt )
          {
            std::string const &subDirEntry = *jt;
            if ( subDirEntry.length() >= prefixEntry.length()
              && EqInsensitive( subDirEntry.substr( 0, prefixEntry.length() ), prefixEntry ) )
            {
              result.push_back( JoinPath( prefixDirPath, subDirEntry, suffixDirPath ) );
            }
          }
        }
      }
    }
  };
};
