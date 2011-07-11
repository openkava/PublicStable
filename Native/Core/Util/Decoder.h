/*
 *
 *  Created by Peter Zion on 10-10-28.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */
 
#ifndef _FABRIC_UTIL_DECODER_H
#define _FABRIC_UTIL_DECODER_H

#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace Util
  {
    class Decoder
    {
    public:
    
      Decoder &get( bool &value )
      {
        uint8_t uint8Value;
        get( uint8Value );
        value = uint8Value ? true : false;
        return *this;
      }
      Decoder &get( uint8_t &value )
      {
        return getData( &value, sizeof(value), true );
      }
      Decoder &get( int8_t &value )
      {
        return getData( &value, sizeof(value), true );
      }
      Decoder &get( uint16_t &value )
      {
        return getData( &value, sizeof(value), true );
      }
      Decoder &get( int16_t &value )
      {
        return getData( &value, sizeof(value), true );
      }
      Decoder &get( uint32_t &value )
      {
        return getData( &value, sizeof(value), true );
      }
      Decoder &get( int32_t &value )
      {
        return getData( &value, sizeof(value), true );
      }
      Decoder &get( uint64_t &value )
      {
        return getData( &value, sizeof(value), true );
      }
      Decoder &get( int64_t &value )
      {
        return getData( &value, sizeof(value), true );
      }
      Decoder &getSize( size_t &value )
      {
        uint64_t valueUInt64;
        Decoder &result = get( valueUInt64 );
        value = size_t(valueUInt64);
        if ( uint64_t(value) != valueUInt64 )
          throw Exception( "value is too large for architecture" );
        return result;
      }
      Decoder &get( float &value )
      {
        return getData( &value, sizeof(value), true );
      }
      Decoder &get( double &value )
      {
        return getData( &value, sizeof(value), true );
      }
      Decoder &get( std::string &str )
      {
        size_t length;
        getStringLength( length );
        str.resize( length );
        return getData( &str[0], length, false );
      }
      Decoder &getStringLength( size_t &length )
      {
        uint64_t uint64Length;
        get( uint64Length );
        length = size_t( uint64Length );
        if ( uint64_t( length ) != uint64Length )
          throw Exception( "string is too large for architecture" );
        return *this;
      }
      Decoder &getStringData( char *data, size_t length )
      {
        getData( data, length, false );
        data[length] = '\0';
        return *this;
      }
      
      virtual Decoder &skip( size_t count ) = 0;
      
      virtual Decoder &getData( void *data, size_t count, bool endian ) = 0;    
    };
  };
};

#endif //_FABRIC_UTIL_DECODER_H
