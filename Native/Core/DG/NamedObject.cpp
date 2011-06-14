/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "NamedObject.h"
#include "Context.h"
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>

namespace Fabric
{
  

  namespace DG
  {
    NamedObject::NamedObject( std::string const &name, RC::Handle<Context> const &context )
      : CompiledObject( context )
      , m_context( context )
      , m_name( name )
    {
      if ( name.length() == 0 )
        throw Exception( "name must be non-empty" );
      
      Context::NamedObjectMap &namedObjectMap = context->getNamedObjectRegistry();
      bool insertResult = namedObjectMap.insert( Context::NamedObjectMap::value_type( name, this ) ).second;
      if ( !insertResult )
        throw Exception( "an object named "+_(name)+" already exists" );
    }
    
    std::string const &NamedObject::getName() const
    {
      return m_name;
    }

    RC::ConstHandle<JSON::Value> NamedObject::jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Value> result;

      if ( dst.size() - dstOffset == 0 )
      {
        try
        {
          result = jsonExec( cmd, arg );
        }
        catch ( Exception e )
        {
          throw "command " + _(cmd) + ": " + e;
        }
      }
      else throw Exception( "unroutable" );
      
      return result;
    }
      
    RC::ConstHandle<JSON::Value> NamedObject::jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      throw Exception( "unknown command" );
    }
      
    RC::Handle<JSON::Object> NamedObject::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( "type", jsonDescType() );
      result->set( "errors", jsonDescErrors() );
      return result;
    }
    
    void NamedObject::notify( std::string const &cmd, RC::ConstHandle<JSON::Object> const &arg ) const
    {
      std::vector<std::string> src;
      src.push_back( "DG" );
      src.push_back( getName() );
      
      m_context->jsonNotify( src, cmd, arg );
    }
    
    void NamedObject::notifyDelta( RC::ConstHandle<JSON::Object> const &delta ) const
    {
      notify( "delta", delta );
    }
    
    void NamedObject::notifyDelta( std::string const &propertyName, RC::ConstHandle<JSON::Value> const &propertyValue ) const
    {
      RC::Handle<JSON::Object> deltas = JSON::Object::Create();
      deltas->set( propertyName, propertyValue );
      notifyDelta( deltas );
    }
    
    void NamedObject::jsonNotifyErrorDelta() const
    {
      notifyDelta( "errors", jsonDescErrors() );
    }
  };
};
