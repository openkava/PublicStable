/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_UTIL_JSON_DECODER_H
#define _FABRIC_UTIL_JSON_DECODER_H

#include <Fabric/Base/Util/SimpleString.h>
#include <Fabric/Core/Util/Assert.h>
#include <Fabric/Base/RC/ConstHandle.h>
#include <Fabric/Base/RC/String.h>

namespace Fabric
{
  namespace Util
  {
    enum JSONEntityType
    {
      ET_NULL,
      ET_BOOLEAN,
      ET_INTEGER,
      ET_SCALAR,
      ET_STRING,
      ET_OBJECT,
      ET_ARRAY
    };
    
    static const size_t jsonDecoderShortStringMaxLength = 16;
    
    struct JSONEntityInfo
    {
      JSONEntityType type;
      char const *data;
      size_t length;
      union
      {
        bool boolean;
        int32_t integer;
        double scalar;
        struct
        {
          size_t length;
          char shortData[jsonDecoderShortStringMaxLength];
        } string;
      } value;
      
      JSONEntityInfo()
      {
      }
      
      void copyFrom( JSONEntityInfo const &that )
      {
        data = that.data;
        length = that.length;
        switch ( (type = that.type) )
        {
        case ET_BOOLEAN:
          value.boolean = that.value.boolean;
          break;
        case ET_INTEGER:
          value.integer = that.value.integer;
          break;
        case ET_SCALAR:
          value.scalar = that.value.scalar;
          break;
        case ET_STRING:
          if ( (value.string.length = that.value.string.length) <= jsonDecoderShortStringMaxLength )
            memcpy( value.string.shortData, that.value.string.shortData, value.string.length );
          break;
        default:
          break;
        }
      }
      
      JSONEntityInfo( JSONEntityInfo const &that )
      {
        copyFrom( that );
      }
      
      JSONEntityInfo &operator =( JSONEntityInfo const &that )
      {
        copyFrom( that );
        return *this;
      }
    };
      
    bool jsonGetEntity( char const *&data, size_t &length, JSONEntityInfo &entityInfo );

    bool jsonEntityStringIsEqual( JSONEntityInfo const &entityInfo, char const *data, size_t length );
    inline bool jsonEntityStringIsEqual( JSONEntityInfo const &entityInfo, char const *cString )
    {
      return jsonEntityStringIsEqual( entityInfo, cString, strlen( cString ) );
    }
    inline bool jsonEntityStringIsEqual( JSONEntityInfo const &entityInfo, std::string const &stdString )
    {
      return jsonEntityStringIsEqual( entityInfo, stdString.data(), stdString.length() );
    }
    
    class JSONObjectParser
    {
    public:
    
      JSONObjectParser( JSONEntityInfo const &entityInfo )
        : m_data( entityInfo.data )
        , m_length( entityInfo.length )
        , m_count( 0 )
      {
        FABRIC_ASSERT( entityInfo.type == ET_OBJECT );
        FABRIC_ASSERT( m_length > 0 );
        FABRIC_ASSERT( m_data[0] == '{' );
        ++m_data, --m_length;
      }
      
      bool getNext( JSONEntityInfo &memberKeyEntityInfo, JSONEntityInfo &memberValueEntityInfo );
    
    private:
    
      char const *m_data;
      size_t m_length;
      size_t m_count;
    };
    
    class JSONArrayParser
    {
    public:
    
      JSONArrayParser( JSONEntityInfo const &entityInfo )
        : m_data( entityInfo.data )
        , m_length( entityInfo.length )
        , m_count( 0 )
      {
        FABRIC_ASSERT( entityInfo.type == ET_ARRAY );
        FABRIC_ASSERT( m_length > 0 );
        FABRIC_ASSERT( m_data[0] == '[' );
        ++m_data, --m_length;
      }
      
      bool getNext( JSONEntityInfo &elementEntityInfo );
    
    private:
    
      char const *m_data;
      size_t m_length;
      size_t m_count;
    };
  }
}

#endif //_FABRIC_UTIL_JSON_DECODER_H
