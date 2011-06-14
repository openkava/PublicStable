/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_UTIL_PARSE_H
#define _FABRIC_UTIL_PARSE_H

#include <string>

namespace Fabric
{
  namespace Util
  {
    size_t parseSize( char const *data, size_t length );
    size_t parseSize( char const *cString );
    size_t parseSize( std::string const &string );
    
    float parseFloat( char const *data, size_t length );
    float parseFloat( char const *cString );
    float parseFloat( std::string const &string );

    std::string parseQuotedString( char const *data, size_t length );
    std::string parseQuotedString( char const *cString );
    std::string parseQuotedString( std::string const &string );
    
    size_t parseHex( char const *data, size_t length );
  };
};

#endif //_FABRIC_UTIL_PARSE_H
