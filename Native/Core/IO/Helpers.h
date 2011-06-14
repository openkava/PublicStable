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

    //void safeCall( void (*callback)( int fd ) );
  };
};

#endif //_FABRIC_IO_HELPERS_H
