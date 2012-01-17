/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/Util/JSONDecoder.h>
#include <Fabric/Base/Exception.h>

#include <math.h>

namespace Fabric
{
  namespace Util
  {
    static const Exception malformedJSONException( "malformed JSON" );

    static inline void jsonSkipWhitespace( char const *&data, size_t &length )
    {
      while ( length > 0
        && ( data[0] == ' '
         || data[0] == '\n'
         || data[0] == '\r'
         || data[0] == '\t'
         || data[0] == '\v'
         || data[0] == '\f' )
        )
      {
        ++data; --length;
      }
    }

    static inline char jsonConsumeToken( char const *&data, size_t &length )
    {
      jsonSkipWhitespace( data, length );
      if ( length == 0 )
        throw malformedJSONException;
      char result = data[0];
      ++data, --length;
      return result;
    }
    
    static void jsonSkipString( char const *&data, size_t &length )
    {
      if ( length == 0 || data[0] != '"' )
        throw malformedJSONException;
      ++data, --length;
      
      bool done = false;
      while ( !done )
      {
        if ( length < 1 )
          throw malformedJSONException;
          
        switch ( data[0] )
        {
          case '"':
            done = true;
            break;
            
          case '\\':
            if ( length < 2 )
              throw malformedJSONException;
            switch ( data[1] )
            {
              case '"':
              case '/':
              case '\\':
              case 'b':
              case 'f':
              case 'n':
              case 'r':
              case 't':
                ++data, --length;
                break;
                
              case 'u':
                if ( length < 5 )
                  throw malformedJSONException;
                data += 4;
                length -= 4;
                break;
                
              default:
                throw malformedJSONException;
            }
            break;
          
          default:
            break;
        }
        
        ++data, --length;
      }
    }
    
    static inline uint8_t jsonHexToUInt8( char hex )
    {
      switch ( hex )
      {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'a': case 'A': return 10;
        case 'b': case 'B': return 11;
        case 'c': case 'C': return 12;
        case 'd': case 'D': return 13;
        case 'e': case 'E': return 14;
        case 'f': case 'F': return 15;
        default: throw malformedJSONException;
      }
    }
    
    static inline uint16_t jsonConsumeUCS2( char const *&data, size_t &length )
    {
      if ( length < 4 )
        throw malformedJSONException;
      uint16_t result =
        (uint16_t(jsonHexToUInt8( data[0] )) << 12)
        | (uint16_t(jsonHexToUInt8( data[1] )) << 8)
        | (uint16_t(jsonHexToUInt8( data[2] )) << 4)
        | uint16_t(jsonHexToUInt8( data[3] ));
      data += 4;
      length -= 4;
      return result;
    }
    
    static inline void jsonStringAppendASCII( char ch, JSONEntityInfo &entityInfo )
    {
      if ( entityInfo.value.string.length < jsonDecoderShortStringMaxLength )
        entityInfo.value.string.shortData[entityInfo.value.string.length] = ch;
      ++entityInfo.value.string.length;
    }
    
    static inline size_t jsonUCS2ToUTF8( uint16_t ucs2, char utf8[3] )
    {
      if ( ucs2 < 0x80 )
      {
        utf8[0] = char(ucs2);
        return 1;
      }
      else if ( ucs2 < 0x800)
      {
        utf8[0] = char(ucs2 >> 6) | char(0xC0);
        utf8[1] = char(ucs2& 0x3F) | char(0x80);
        return 2;
      }
      else
      {
        utf8[0] = char(ucs2 >> 12) | char(0xE0);
        utf8[1] = (char(ucs2 >> 6) & char(0x3F)) | char(0xE0);
        utf8[2] = (char(ucs2) & char(0x3F)) | char(0x80);
        return 3;
      }
    }
    
    static inline void jsonStringAppendUCS2( uint16_t ucs2, JSONEntityInfo &entityInfo )
    {
      char utf8[3];
      size_t utf8Length = jsonUCS2ToUTF8( ucs2, utf8 );
      jsonStringAppendASCII( utf8[0], entityInfo );
      if ( utf8Length > 1 )
      {
        jsonStringAppendASCII( utf8[1], entityInfo );
        if ( utf8Length > 2 )
          jsonStringAppendASCII( utf8[2], entityInfo );
      }
    }
      
    bool jsonGetEntity( char const *&data, size_t &length, JSONEntityInfo &entityInfo )
    {
      jsonSkipWhitespace( data, length );
      if ( length == 0 )
        return false;
      
      entityInfo.data = data;
      switch ( data[0] )
      {
        case 'n':
        {
          if ( length < 4
            || data[1] != 'u'
            || data[2] != 'l'
            || data[3] != 'l'
            )
            throw malformedJSONException;
          data += 4;
          length -= 4;
          entityInfo.type = ET_NULL;
        }
        break;
        
        case 't':
        {
          if ( length < 4
            || data[1] != 'r'
            || data[2] != 'u'
            || data[3] != 'e'
            )
            throw malformedJSONException;
          data += 4;
          length -= 4;
          entityInfo.type = ET_BOOLEAN;
          entityInfo.value.boolean = true;
        }
        break;
        
        case 'f':
        {
          if ( length < 5
            || data[1] != 'a'
            || data[2] != 'l'
            || data[3] != 's'
            || data[4] != 'e'
            )
            throw malformedJSONException;
          data += 5;
          length -= 5;
          entityInfo.type = ET_BOOLEAN;
          entityInfo.value.boolean = false;
        }
        break;
        
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
          bool mantissaNeg = false;
          if ( length > 0 && data[0] == '-' )
          {
            mantissaNeg = true;
            ++data; --length;
          }

          if ( length == 0 )
            throw malformedJSONException;
            
          size_t mantissaInt = 0;
          switch ( data[0] )
          {
            case '0':
              mantissaInt = 0;
              ++data; --length;
              break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
              mantissaInt = data[0] - '0';
              ++data; --length;
              while ( length > 0 && data[0] >= '1' && data[0] <= '9' )
              {
                mantissaInt = 10 * mantissaInt + data[0] - '0';
                ++data; --length;
              };
              break;
            default:
              throw malformedJSONException;
          }

          if ( length == 0 || (data[0] != '.' && data[0] != 'e' && data[0] != 'E') )
          {
            entityInfo.type = ET_INTEGER;
            if ( mantissaNeg )
              entityInfo.value.integer = -int32_t(mantissaInt);
            else entityInfo.value.integer = int32_t(mantissaInt);
          }
          else
          {
            entityInfo.type = ET_SCALAR;
            
            double mantissa;
            if ( data[0] == '.' )
            {
              ++data; --length;
              
              if ( length == 0 || data[0] < '0' || data[0] > '9' )
                throw malformedJSONException;
              size_t fractionNumInt = 0;
              size_t fractionDenInt = 1;
              while ( length > 0 && data[0] >= '0' && data[0] <= '9' )
              {
                fractionNumInt = 10 * fractionNumInt + data[0] - '0';
                fractionDenInt = 10 * fractionDenInt;
                ++data; --length;
              }
              mantissa = double(mantissaInt) + double(fractionNumInt) / double(fractionDenInt);
            }
            else mantissa = double(mantissaInt);
            
            if ( mantissaNeg )
              mantissa = -mantissa;
              
            if ( length == 0 || (data[0] != 'e' && data[0] != 'E') )
              entityInfo.value.scalar = mantissa;
            else
            {
              ++data; --length;

              bool exponentNeg = false;
              if ( length > 0 && data[0] == '-' )
              {
                exponentNeg = true;
                ++data; --length;
              }
              else if ( length > 0 && data[0] == '+' )
              {
                ++data; --length;
              }
              
              if ( length == 0 || data[0] < '0' || data[0] > '9' )
                throw malformedJSONException;
              int exponent = data[0] - '0';
              ++data; --length;
              while ( length > 0 && data[0] >= '0' && data[9] <= '9' )
              {
                exponent = 10 * exponent + data[0] - '0';
                ++data; --length;
              }
              
              if ( exponentNeg )
                exponent = -exponent;
              
              entityInfo.value.scalar = mantissa * pow( 10, exponent );
            }
          }
        }
        break;
        
        case '"':
        {
          entityInfo.type = ET_STRING;
          entityInfo.value.string.length = 0;
          ++data; --length;
          
          bool done = false;
          while ( !done )
          {
            if ( length == 0 )
              throw malformedJSONException;
            switch ( data[0] )
            {
              case '"':
                ++data; --length;
                done = true;
                break;
                
              case '\\':
              {
                ++data; --length;
                if ( length == 0 )
                  throw malformedJSONException;
                switch ( data[0] )
                {
                  case '"':
                  case '/':
                  case '\\':
                    jsonStringAppendASCII( data[0], entityInfo );
                    ++data; --length;
                    break;
                  
                  case 'b':
                    jsonStringAppendASCII( '\b', entityInfo );
                    ++data; --length;
                    break;
                  case 'f':
                    jsonStringAppendASCII( '\f', entityInfo );
                    ++data; --length;
                    break;
                  case 'n':
                    jsonStringAppendASCII( '\n', entityInfo );
                    ++data; --length;
                    break;
                  case 'r':
                    jsonStringAppendASCII( '\r', entityInfo );
                    ++data; --length;
                    break;
                  case 't':
                    jsonStringAppendASCII( '\t', entityInfo );
                    ++data; --length;
                    break;
                  
                  case 'u':
                    ++data; --length;
                    jsonStringAppendUCS2( jsonConsumeUCS2( data, length ), entityInfo );
                    break;
                  
                  default:
                    throw malformedJSONException;
                }
              }
              break;
              
              default:
                jsonStringAppendASCII( data[0], entityInfo );
                ++data; --length;
                break;
            }
          }
        }
        break;
        
        case '{':
        {
          entityInfo.type = ET_OBJECT;
          ++data, --length;
          
          size_t nestCount = 1;
          while ( nestCount > 0 )
          {
            if ( length == 0 )
              throw malformedJSONException;
            switch ( data[0] )
            {
              case '{':
                ++nestCount;
                ++data, --length;
                break;
              
              case '}':
                --nestCount;
                ++data, --length;
                break;
              
              case '"':
                jsonSkipString( data, length );
                break;
            
              default:
                ++data, --length;
                break;
            }
          }
        }
        break;
        
        case '[':
        {
          entityInfo.type = ET_ARRAY;
          ++data, --length;
          
          size_t nestCount = 1;
          while ( nestCount > 0 )
          {
            if ( length == 0 )
              throw malformedJSONException;
            switch ( data[0] )
            {
              case '[':
                ++nestCount;
                ++data, --length;
                break;
              
              case ']':
                --nestCount;
                ++data, --length;
                break;
              
              case '"':
                jsonSkipString( data, length );
                break;
            
              default:
                ++data, --length;
                break;
            }
          }
        }
        break;
        
        default:
          throw malformedJSONException;
      }
      
      entityInfo.length = data - entityInfo.data;
      return true;
    }

    static inline void jsonConsumeEntity( char const *&data, size_t &length, JSONEntityInfo &entityInfo )
    {
      if ( !jsonGetEntity( data, length, entityInfo ) )
        throw malformedJSONException;
    }
    
    bool JSONObjectParser::getNext( JSONEntityInfo &memberKeyEntityInfo, JSONEntityInfo &memberValueEntityInfo )
    {
      if ( m_length == 0 || m_data[0] == '}' )
        return false;
        
      jsonConsumeEntity( m_data, m_length, memberKeyEntityInfo );
      
      switch ( jsonConsumeToken( m_data, m_length ) )
      {
        case ':':
          break;
          
        default:
          throw malformedJSONException;
      }
      
      jsonConsumeEntity( m_data, m_length, memberValueEntityInfo );
      
      switch ( jsonConsumeToken( m_data, m_length ) )
      {
        case ',':
          if ( m_length == 0 )
            throw malformedJSONException;
          break;
        case '}':
          if ( m_length > 0 )
            throw malformedJSONException;
          break;
        default:
          throw malformedJSONException;
      }
      
      ++m_count;
      return true;
    }
    
    bool JSONArrayParser::getNext( JSONEntityInfo &elementEntityInfo )
    {
      if ( m_length == 0 || m_data[0] == ']' )
        return false;

      jsonConsumeEntity( m_data, m_length, elementEntityInfo );
      
      switch ( jsonConsumeToken( m_data, m_length ) )
      {
        case ',':
          if ( m_length == 0 )
            throw malformedJSONException;
          break;
        case ']':
          if ( m_length > 0 )
            throw malformedJSONException;
          break;
        default:
          throw malformedJSONException;
      }
      
      ++m_count;
      return true;
    }

    bool jsonEntityStringIsEqual( JSONEntityInfo const &entityInfo, char const *data, size_t length )
    {
      FABRIC_ASSERT( entityInfo.type == ET_STRING );
      if ( length != entityInfo.value.string.length )
        return false;
      if ( memcmp( data, entityInfo.value.string.shortData, std::min( length, jsonDecoderShortStringMaxLength ) ) != 0 )
        return false;
      if ( length > jsonDecoderShortStringMaxLength )
      {
        char const *entityData = entityInfo.data;
        size_t entityLength = entityInfo.length;
        FABRIC_ASSERT( entityLength > 0 );
        FABRIC_ASSERT( entityData[0] == '"' );
        ++entityData, --entityLength;
        while ( length > 0 )
        {
          FABRIC_ASSERT( entityLength > 0 );
          switch ( entityData[0] )
          {
            case '"':
              FABRIC_ASSERT( false );
              return false;
              
            case '\\':
              ++entityData, --entityLength;
              FABRIC_ASSERT( entityLength > 0 );
              switch ( entityData[0] )
              {
              case '"':
              case '/':
              case '\\':
                if ( data[0] != entityData[0] )
                  return false;
                ++entityData, --entityLength;
                ++data, --length;
                break;
              
              case 'b':
                if ( data[0] != '\b' )
                  return false;
                ++entityData, --entityLength;
                ++data, --length;
                break;
              case 'f':
                if ( data[0] != '\f' )
                  return false;
                ++entityData, --entityLength;
                ++data, --length;
                break;
              case 'n':
                if ( data[0] != '\n' )
                  return false;
                ++entityData, --entityLength;
                ++data, --length;
                break;
              case 'r':
                if ( data[0] != '\r' )
                  return false;
                ++entityData, --entityLength;
                ++data, --length;
                break;
              case 't':
                if ( data[0] != '\t' )
                  return false;
                ++entityData, --entityLength;
                ++data, --length;
                break;
              
              case 'u':
              {
                ++entityData, --entityLength;
                FABRIC_ASSERT( entityLength >= 4 );
                uint16_t ucs2 = jsonConsumeUCS2( entityData, entityLength );
                
                char utf8[3];
                size_t utf8Length = jsonUCS2ToUTF8( ucs2, utf8 );
                if ( length < utf8Length
                  || memcmp( utf8, data, utf8Length ) != 0 )
                  return false;
                data += utf8Length, length -= utf8Length;
              }
              break;
              
              default:
                FABRIC_ASSERT( false );
                return false;
            }
            
            default:
              if ( entityData[0] != data[0] )
                return false;
              ++entityData, --entityLength;
              ++data, --length;
              break;
          }
        }
        FABRIC_ASSERT( entityLength == 0 );
      }
      return true;
    }
  }
}