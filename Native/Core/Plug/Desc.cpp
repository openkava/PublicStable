/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "Desc.h"

#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Core/Util/Format.h>

namespace Fabric
{
  
  
  namespace Plug
  {
    static HostSpec parseHostSpec( RC::ConstHandle<JSON::Value> value )
    {
      RC::ConstHandle<JSON::Array> array;
      if ( value->isString() )
      {
        RC::Handle<JSON::Array> mutableArray = JSON::Array::Create();
        mutableArray->push_back( value );
        array = mutableArray;
      }
      else if ( value->isArray() )
        array = RC::ConstHandle<JSON::Array>::StaticCast( value );
      else
        throw Exception( "must be a string or an array" );
      
      HostSpec result;
      size_t size = array->size();
      for ( size_t i=0; i<size; ++i )
      {
        try
        {
          RC::ConstHandle<JSON::String> elementString = array->get(i)->toString();
          result.push_back( std::string( elementString->data(), elementString->length() ) );
        }
        catch ( Exception e )
        {
          throw "element "+_(i+1)+": "+e;
        }
      }
      return result;
    }
    
    static void parseHostStrings( RC::ConstHandle<JSON::Value> value, HostStrings &result )
    {
      RC::ConstHandle<JSON::Object> object;
      if ( value->isString() )
      {
        RC::ConstHandle<JSON::String> string = RC::ConstHandle<JSON::String>::StaticCast( value );
        
        RC::Handle<JSON::Object> mutableObject = JSON::Object::Create();
        mutableObject->set( std::string( string->data(), string->length() ), JSON::String::Create( "*", 1 ) );
        object = mutableObject;
      }
      else if ( value->isObject() )
        object = RC::ConstHandle<JSON::Object>::StaticCast( value );
      else throw Exception( "must be a string or object" );
      
      for ( JSON::Object::const_iterator it=object->begin(); it!=object->end(); ++it )
      {
        std::string const &lib = it->first;
        HostSpec hostSpec;
        try
        {
          hostSpec = parseHostSpec( it->second );
        }
        catch ( Exception e )
        {
          throw "lib "+_(lib)+" host spec: ";
        }
        result.insert( HostStrings::value_type( lib, hostSpec ) );
      }
    }

    static void parseHostStringsVector( RC::ConstHandle<JSON::Value> value, HostStringsVector &result )
    {
      if ( value->isArray() )
      {
        RC::ConstHandle<JSON::Array> array = RC::ConstHandle<JSON::Array>::StaticCast( value );
        size_t size = array->size();
        for ( size_t i=0; i<size; ++i )
        {
          try
          {
            HostStrings hostStrings;
            parseHostStrings( array->get(i), hostStrings );
            result.push_back( hostStrings );
          }
          catch ( Exception e )
          {
            throw "element "+_(i+1)+": "+e;
          }
        }
      }
      else
      {
        HostStrings hostStrings;
        parseHostStrings( value, hostStrings );
        result.push_back( hostStrings );
      }
    }
    
    void parseMethods( RC::ConstHandle<JSON::Array> const &methodsArray, std::vector<std::string> &methods )
    {
      size_t numMethods = methodsArray->size();
      for ( size_t i=0; i<numMethods; ++i )
      {
        try
        {
          RC::ConstHandle<JSON::String> methodString = methodsArray->get(i)->toString();
          methods.push_back( std::string( methodString->data(), methodString->length() ) );
        }
        catch ( Exception e )
        {
          throw "element "+_(i+1)+": "+e;
        }
      }
    }
    
    void parseInterface( RC::ConstHandle<JSON::Object> const &interfaceObject, Interface &interface )
    {
      RC::ConstHandle<JSON::Value> methodsValue = interfaceObject->maybeGet( "methods" );
      if ( methodsValue )
      {
        try
        {
          RC::ConstHandle<JSON::Array> methodsArray = methodsValue->toArray();
          parseMethods( methodsArray, interface.methods );
        }
        catch ( Exception e )
        {
          throw "methods: " + e;
        }
      }
    }
    
    Desc parseDesc( RC::ConstHandle<JSON::Object> const &object )
    {
      Desc result;
      
      try
      {
        parseHostStringsVector( object->get( "libs" ), result.libs );
      }
      catch ( Exception e )
      {
        throw "libs: " + e;
      }
      
      try
      {
        parseHostStringsVector( object->get( "code" ), result.code );
      }
      catch ( Exception e )
      {
        throw "code: " + e;
      }
      
      RC::ConstHandle<JSON::Value> interfaceValue = object->maybeGet( "interface" );
      if ( interfaceValue )
      {
        try
        {
          RC::ConstHandle<JSON::Object> interfaceObject = interfaceValue->toObject();
          parseInterface( interfaceObject, result.interface );
        }
        catch ( Exception e )
        {
          throw "interface: " + e;
        }
      }
      
      return result;
    }
  };
};

