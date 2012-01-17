/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/Util/JSONDecoder.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/Assert.h>

#include <stdio.h>
#include <string>
#include <iostream>

void displayEntity( Fabric::Util::JSONEntityInfo const &entityInfo, std::string const &indent )
{
  std::cout << indent;
  
  switch ( entityInfo.type )
  {
    case Fabric::Util::ET_NULL:
      std::cout << "NULL" << std::endl;
      break;
    case Fabric::Util::ET_BOOLEAN:
      std::cout << "BOOLEAN ";
      std::cout << (entityInfo.value.boolean? "true": "false");
      std::cout << std::endl;
      break;
    case Fabric::Util::ET_INTEGER:
      std::cout << "INTEGER ";
      std::cout << Fabric::_(entityInfo.value.integer);
      std::cout << std::endl;
      break;
    case Fabric::Util::ET_SCALAR:
      std::cout << "SCALAR ";
      std::cout << Fabric::_(entityInfo.value.scalar);
      std::cout << std::endl;
      break;
    case Fabric::Util::ET_STRING:
      std::cout << "STRING ";
      std::cout << entityInfo.value.string.length;
      std::cout << " ";
      std::cout << Fabric::_( entityInfo.value.string.shortData, entityInfo.value.string.length, Fabric::Util::jsonDecoderShortStringMaxLength );
      std::cout << std::endl;
      break;
    case Fabric::Util::ET_OBJECT:
    {
      std::cout << "OBJECT" << std::endl;
      Fabric::Util::JSONObjectParser objectParser( entityInfo );
      Fabric::Util::JSONEntityInfo keyEntityInfo, valueEntityInfo;
      while ( objectParser.getNext( keyEntityInfo, valueEntityInfo ) )
      {
        displayEntity( keyEntityInfo, "  " + indent );
        displayEntity( valueEntityInfo, "    " + indent );
      }
    }
    break;
    case Fabric::Util::ET_ARRAY:
    {
      std::cout << "ARRAY" << std::endl;
      Fabric::Util::JSONArrayParser arrayParser( entityInfo );
      Fabric::Util::JSONEntityInfo elementEntityInfo;
      while ( arrayParser.getNext( elementEntityInfo ) )
        displayEntity( elementEntityInfo, "  " + indent );
    }
    break;
    
    default:
      FABRIC_ASSERT( false );
      break;
  }
}

void parseJSON( FILE *fp )
{
  static const size_t maxLength = 16*1024*1024;
  char *buffer = new char[maxLength];
  
  char const *data = buffer;
  size_t length = 0;
  while ( !feof( fp ) )
  {
    int read = fread( &buffer[length], 1, maxLength - length, fp );
    if ( read <= 0 )
      break;
    length += read;
  }
  
  Fabric::Util::JSONEntityInfo entityInfo;
  while ( Fabric::Util::jsonGetEntity( data, length, entityInfo ) )
    displayEntity( entityInfo, "" );
    
  delete [] buffer;
}

int main( int argc, char **argv )
{
  if ( argc == 1 )
    parseJSON( stdin );
  else
  {
    for ( int i=1; i<argc; ++i )
    {
      FILE *fp = fopen( argv[i], "r" );
      if ( !fp )
        perror( argv[i] );
      parseJSON( fp );
      fclose( fp );
    }
  }
  return 0;
}
