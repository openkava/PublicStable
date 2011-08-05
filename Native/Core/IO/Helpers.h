/*
 *
 *  Created by Peter Zion on 10-12-07.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_IO_HELPERS_H
#define _FABRIC_IO_HELPERS_H

#include <string>

namespace Fabric
{
  namespace IO
  {
    void validateEntry( std::string const &entry );
    
    std::string const &getRootPath();
    
    std::string joinPath( std::string const &lhs, std::string const &rhs );
    inline std::string joinPath( std::string const &arg1, std::string const &arg2, std::string const &arg3 )
    {
      return joinPath( joinPath( arg1, arg2 ), arg3 );
    }
    inline std::string joinPath( std::string const &arg1, std::string const &arg2, std::string const &arg3, std::string const &arg4 )
    {
      return joinPath( joinPath( arg1, arg2, arg3 ), arg4 );
    }
    inline std::string joinPath( std::string const &arg1, std::string const &arg2, std::string const &arg3, std::string const &arg4, std::string const &arg5 )
    {
      return joinPath( joinPath( arg1, arg2, arg3, arg4 ), arg5 );
    }
    inline std::string joinPath( std::string const &arg1, std::string const &arg2, std::string const &arg3, std::string const &arg4, std::string const &arg5, std::string const &arg6 )
    {
      return joinPath( joinPath( arg1, arg2, arg3, arg4, arg5 ), arg6 );
    }
    inline std::string joinPath( std::string const &arg1, std::string const &arg2, std::string const &arg3, std::string const &arg4, std::string const &arg5, std::string const &arg6, std::string const &arg7 )
    {
      return joinPath( joinPath( arg1, arg2, arg3, arg4, arg5, arg6 ), arg7 );
    }

    //void safeCall( void (*callback)( int fd ) );
  };
};

#endif //_FABRIC_IO_HELPERS_H
