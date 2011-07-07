/*
 *
 *  Created by Peter Zion on 10-10-28.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */
 
#ifndef _FABRIC_UTIL_ENCODER_H
#define _FABRIC_UTIL_ENCODER_H

namespace Fabric
{
  namespace Util
  {
    class Encoder
    {
    public:
    
      Encoder &put( bool value )
      {
        return put( (uint8_t)value );
      }
      Encoder &put( uint8_t value )
      {
        return putData( &value, sizeof(value), true );
      }
      Encoder &put( int8_t value )
      {
        return putData( &value, sizeof(value), true );
      }
      Encoder &put( uint16_t value )
      {
        return putData( &value, sizeof(value), true );
      }
      Encoder &put( int16_t value )
      {
        return putData( &value, sizeof(value), true );
      }
      Encoder &put( uint32_t value )
      {
        return putData( &value, sizeof(value), true );
      }
      Encoder &put( int32_t value )
      {
        return putData( &value, sizeof(value), true );
      }
      Encoder &put( uint64_t value )
      {
        return putData( &value, sizeof(value), true );
      }
      Encoder &put( int64_t value )
      {
        return putData( &value, sizeof(value), true );
      }
      Encoder &putSize( size_t value )
      {
        return put( uint64_t(value) );
      }
      Encoder &put( float value )
      {
        return putData( &value, sizeof(value), true );
      }
      Encoder &put( double value )
      {
        return putData( &value, sizeof(value), true );
      }
      Encoder &put( std::string const &str )
      {
        return putString( str.data(), str.length() );
      }
      Encoder &putString( char const *cStr, size_t length )
      {
        uint64_t uint64Length = length;
        return put( uint64Length ).putData( cStr, length, false );
      }

      virtual Encoder &putData( void const *data, size_t count, bool endian ) = 0;
    };
  };
};

#endif //_FABRIC_UTIL_ENCODER_H
