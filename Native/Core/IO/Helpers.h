/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_IO_HELPERS_H
#define _FABRIC_IO_HELPERS_H

#include <string>
#include <vector>

namespace Fabric
{
  namespace IO
  {
    void validateEntry( std::string const &entry );
    
    std::string const &getRootPath();
    
    std::string JoinPath( std::string const &lhs, std::string const &rhs );
    inline std::string JoinPath( std::string const &arg1, std::string const &arg2, std::string const &arg3 )
    {
      return JoinPath( JoinPath( arg1, arg2 ), arg3 );
    }
    inline std::string JoinPath( std::string const &arg1, std::string const &arg2, std::string const &arg3, std::string const &arg4 )
    {
      return JoinPath( JoinPath( arg1, arg2, arg3 ), arg4 );
    }
    inline std::string JoinPath( std::string const &arg1, std::string const &arg2, std::string const &arg3, std::string const &arg4, std::string const &arg5 )
    {
      return JoinPath( JoinPath( arg1, arg2, arg3, arg4 ), arg5 );
    }
    inline std::string JoinPath( std::string const &arg1, std::string const &arg2, std::string const &arg3, std::string const &arg4, std::string const &arg5, std::string const &arg6 )
    {
      return JoinPath( JoinPath( arg1, arg2, arg3, arg4, arg5 ), arg6 );
    }
    inline std::string JoinPath( std::string const &arg1, std::string const &arg2, std::string const &arg3, std::string const &arg4, std::string const &arg5, std::string const &arg6, std::string const &arg7 )
    {
      return JoinPath( JoinPath( arg1, arg2, arg3, arg4, arg5, arg6 ), arg7 );
    }

    void SplitPath( std::string const &path, std::string &parentDir, std::string &entry );

    //void safeCall( void (*callback)( int fd ) );
    
    bool DirExists( std::string const &dirPath );
    void CreateDir( std::string const &dirPath );
    std::vector<std::string> GetSubDirEntries( std::string const &dirPath, bool followLinks = true );
    
    void GlobDirPaths( std::string const &dirPathSpec, std::vector<std::string> &result );
  };
};

#endif //_FABRIC_IO_HELPERS_H
