/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/Util/Parse.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/Assert.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>

namespace Fabric
{
  namespace Util
  {
    size_t parseSize( char const *data, size_t length )
    {
      char *cString = static_cast<char *>( alloca( length + 1 ) );
      memcpy( cString, data, length );
      cString[length] = '\0';
      return parseSize( cString );
    }
    
    size_t parseSize( char const *cString )
    {
      char const *scanfFormat;
      if ( sizeof(size_t) == sizeof(unsigned) )
        scanfFormat = "%u";
      else if ( sizeof(size_t) == sizeof(long unsigned) )
        scanfFormat = "%lu";
      else FABRIC_ASSERT( false && "Unable to determine scanf format for this architecture" );

      size_t result;
      if ( sscanf( cString, scanfFormat, &result ) != 1 )
        throw Exception( "malformed size constant" );
      return result;
    }
    
    size_t parseSize( std::string const &string )
    {
      return parseSize( string.c_str() );
    }

    float parseFloat( char const *data, size_t length )
    {
      char *cString = static_cast<char *>( alloca( length + 1 ) );
      memcpy( cString, data, length );
      cString[length] = '\0';
      return parseFloat( cString );
    }
    
    float parseFloat( char const *cString )
    {
      char const *scanfFormat;
      if ( sizeof(size_t) == sizeof(unsigned) )
        scanfFormat = "%u";
      else if ( sizeof(size_t) == sizeof(long unsigned) )
        scanfFormat = "%lu";
      else FABRIC_ASSERT( false && "Unable to determine scanf format for this architecture" );

      float result;
      if ( sscanf( cString, scanfFormat, &result ) != 1 )
        throw Exception( "malformed floating-point constant" );
      return result;
    }

    float parseFloat( std::string const &string )
    {
      return parseFloat( string.c_str() );
    }
    
    static inline char unescapeStringChar( char ch )
    {
      switch ( ch )
      {
      }
    }
    
    std::string parseQuotedString( char const *data, size_t length )
    {
      static Exception exception("malformed string constant");
      std::string result;
      
      char const *dataEnd = data + length;
      
      char quote = *data++;
      if ( quote != '\'' && quote != '"' )
        throw exception;
              
      while ( data != dataEnd )
      {
        char ch = *data++;
        if ( ch == quote )
        {
          if ( data != dataEnd )
            throw exception;
          break;
        }
        else if ( ch == '\\' )
        {
          if ( data == dataEnd )
            throw exception;
          char ch = *data++;
          switch ( ch )
          {
            case 'n':
              result.push_back( '\n' );
              break;
            case 'f':
              result.push_back( '\f' );
              break;
            case 'r':
              result.push_back( '\r' );
              break;
            case 't':
              result.push_back( '\t' );
              break;
            case 'v':
              result.push_back( '\v' );
              break;
            case 'a':
              result.push_back( '\a' );
              break;
            case 'b':
              result.push_back( '\b' );
              break;
            case '0':
              result.push_back( '\0' );
              break;
            case '"':
              result.push_back( '"' );
              break;
            case '\'':
              result.push_back( '\'' );
              break;
            case '\\':
              result.push_back( '\\' );
              break;
            case 'x':
            {
              if ( data == dataEnd )
                throw exception;
              char const *hexData = data;
              if ( data++ == dataEnd || data++ == dataEnd )
                throw exception;
              result.push_back( parseHex( hexData, 2 ) );
            }
            break;
            default:
              throw exception;
          }
        }
        else result.push_back( ch );
      }
      
      return result;
    }

    std::string parseQuotedString( char const *cString )
    {
      return parseQuotedString( cString, strlen(cString) );
    }
    
    std::string parseQuotedString( std::string const &string )
    {
      return parseQuotedString( string.data(), string.length() );
    }

    size_t parseHex( char const *data, size_t length )
    {
      size_t result = 0;
      for ( size_t i=0; i<length; ++i )
      {
        try
        {
          char ch = data[i];
          switch ( ch )
          {
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
              result = 16*result + ch - '0';
              break;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
              result = 16*result + 10 + ch - 'a';
              break;
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
              result = 16*result + 10 + ch - 'A';
              break;
            default:
              throw Exception("invalid hex character");
          }
        }
        catch ( Exception e )
        {
          throw "index "+_(i)+": "+e;
        }
      }
      return result;
    }
  };
};
