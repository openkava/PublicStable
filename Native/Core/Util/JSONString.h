/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_UTIL_SIMPLE_STRING_H
#define _FABRIC_UTIL_SIMPLE_STRING_H

#include <Fabric/Core/Util/SimpleString.h>

namespace Fabric
{
  namespace Util
  {
    class JSONBase : protected SimpleString
    {
    public:
    
      JSONString()
        : SimpleString()
      {
      }
      
      char const *getData() const
      {
        return SimpleString::getData();
      }
      
      size_t getLength() const
      {
        return SimpleString::getLength();
      }
    }
    
    class JSON
    
            
      void appendJSONBoolean( bool value )
      {
        if ( value )
          append( "true", 4 );
        else
          append( "false", 5 );
      }
      
      void appendJSONInteger( int32_t value )
      {
        increaseLength( snprintf( expand( 11 ), 11, "%d", (int)value ) );
      }
      
      void appendJSONScalar( float value )
      {
        increaseLength( snprintf( expand( 32 ), 32, "%f", value ) );
      }
      
      void appendJSONString( char const *data, size_t length )
      {
        char *tail = expand( 2*length+2 );
        char *p = tail;
        *p++ = '"';
        char const *dataEnd = data + length;
        for ( ; data != dataEnd; ++data )
        {
          switch ( *data )
          {
            case '"':
              *p++ = '\\';
              *p++ = '"';
              break;
            case '\\':
              *p++ = '\\';
              *p++ = '\\';
              break;
            case '\b':
              *p++ = '\\';
              *p++ = 'b';
              break;
            case '\f':
              *p++ = '\\';
              *p++ = 'f';
              break;
            case '\n':
              *p++ = '\\';
              *p++ = 'n';
              break;
            case '\r':
              *p++ = '\\';
              *p++ = 'r';
              break;
            case '\t':
              *p++ = '\\';
              *p++ = 't';
              break;
            default:
              *p++ = *data;
              break;
          }
        }
        *p++ = '"';
        increaseLength( p - tail );
      }
      
      void appendJSONString( char const *cStr )
      {
        appendJSONString( cStr, strlen( cStr ) );
      }
      
      void appendJSONString( std::string const &string )
      {
        appendJSONString( string.data(), string.length() );
      }
      
      void appendJSONObjectBegin()
      {
        append( '{' );
      }
      
      void appendJSONObjectEnd()
      {
        append( '}' );
      }
      
      void appendJSONObjectMemberBegin( char const *name )
      {
        size_t length = getLength();
        FABRIC_ASSERT( length > 0 );
        if ( getData()[length-1] != '{' )
          append( ',' );
        appendJSONString( name );
        append( ':' );
      }
      
      void appendJSONObjectMemberEnd()
      {
      }
      
      void appendJSONArrayBegin()
      {
        append( '[' );
      }
      
      void appendJSONArrayEnd()
      {
        append( ']' );
      }
      
      void appendJSONArrayMemberBegin()
      {
        size_t length = getLength();
        FABRIC_ASSERT( length > 0 );
        if ( getData()[length-1] != '[' )
          append( ',' );
      }
      
      void appendJSONArrayMemberEnd()
      {
      }
      
      char *takeCString()
      {
        char *result = m_data;
        if ( result )
          result[m_length] = '\0';
        m_length = 0;
        m_allocSize = 0;
        m_data = 0;
        return result;
      }
    
    protected:
    
      size_t AllocSizeForLengthPlusOne( size_t lengthPlusOne )
      {
        return std::max<size_t>( nextPowerOfTwoMinusOne(lengthPlusOne), 255 );
      }
    
      void init( char const *data, size_t length )
      {
        if ( (m_length = length) )
        {
          m_allocSize = AllocSizeForLengthPlusOne( length + 1 );
          m_data = (char *)malloc( m_allocSize );
          memcpy( m_data, data, length );
          m_data[length] = '\0';
        }
        else
        {
          m_allocSize = 0;
          m_data = 0;
        }
      }
      
    private:
    
      char *m_data;
      size_t m_length;
      size_t m_allocSize;
    };
  };
};

#endif //_FABRIC_UTIL_SIMPLE_STRING_H
