/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/Hex.h>

#include <stdint.h>
#include <cmath>

namespace Fabric
{
  namespace Util
  {
    std::string formatHex( size_t count, void const *data )
    {
      std::string result;
      result.resize( 2 * count );
      for ( size_t i=0; i<count; ++i )
      {
        static char const *hexDigits = "0123456789ABCDEF";
        uint8_t const &byte = ((uint8_t const *)data)[i];
        result[2*i] = hexDigits[byte >> 4];
        result[2*i+1] = hexDigits[byte&0x0F];
      }
      return result;
    }
  };
  
  std::string _( char const *data, size_t length, char quote )
  {
    std::string result;
    if ( quote )
      result.push_back(quote);
    for ( size_t i=0; i<length; ++i )
    {
      char ch = data[i];
      switch ( ch )
      {
        case '\\': result.append( "\\\\" ); break;
        case '\0': result.append( "\\0" ); break;
        case '\n': result.append( "\\n" ); break;
        case '\r': result.append( "\\r" ); break;
        case '\b': result.append( "\\b" ); break;
        case '\t': result.append( "\\t" ); break;
        case '\f': result.append( "\\f" ); break;
        case '\a': result.append( "\\a" ); break;
        case '\v': result.append( "\\v" ); break;
        default:
          if ( quote && ch == quote )
          {
            result.push_back('\\');
            result.push_back(quote);
          }
          else if ( isprint((unsigned char)ch) )
            result.push_back(ch);
          else
          {
            result.append( "\\x" );
            result.append( Util::formatHex( 1, &ch ) );
          }
          break;
      }
    }
    if ( quote )
      result.push_back(quote);
    return result;
  }
  
  std::string _( float value )
  {
    static const uint32_t signBit = 0x80000000u;
    uint32_t const &valueAsUint32 = *(uint32_t const *)&value;
    if( ( valueAsUint32 & 0x7F800000 ) == 0x7F800000 )
    {
      if( ( valueAsUint32 & 0x007FFFFF ) == 0 )
        return (valueAsUint32&signBit)? "-Inf": "Inf";
      else
        return "NaN";
    }
    else if( ( valueAsUint32 & ~signBit ) == 0 )
      return (valueAsUint32&signBit)? "-0": "0";

    char buffer[64];
    snprintf( buffer, 64, "%g", value );
    return std::string( buffer );
  }
};
