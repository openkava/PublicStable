/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_UTIL_MD5_H
#define _FABRIC_UTIL_MD5_H

#include <string>
#include <stdint.h>

namespace Fabric
{
  namespace Util
  {
    void md5Digest( void const *data, size_t length, uint8_t digest[16] );
    std::string md5HexDigest( void const *data, size_t length );
  };
};

#endif //_FABRIC_UTIL_MD5_H
