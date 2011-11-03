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
      , m_context( context.ptr() )
    {
      if ( name.length() == 0 )
        throw Exception( "name must be non-empty" );
        
      m_notificationSrc.push_back( "DG" );
      m_notificationSrc.push_back( name );
      
      Context::NamedObjectMap &namedObjectMap = context->getNamedObjectRegistry();
      bool insertResult = namedObjectMap.insert( Context::NamedObjectMap::value_type( name, this ) ).second;
      if ( !insertResult )
        throw Exception( "an object named "+_(name)+" already exists" );
    }
    
    std::string const &NamedObject::getName() const
    {
      return m_notificationSrc[1];
    }

    void NamedObject::jsonRoute(
      std::vector<std::string> const &dst,
      size_t dstOffset,
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( dst.size() - dstOffset == 0 )
      {
        try
        {
          jsonExec( cmd, arg, resultJAG );
        }
        catch ( Exception e )
        {
          throw "command " + _(cmd) + ": " + e;
        }
      }
      else throw Exception( "unroutable" );
    }
      
    void NamedObject::jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      throw Exception( "unknown command" );
    }
      
    void NamedObject::jsonDesc( Util::JSONGenerator &resultJG ) const
    {
      Util::JSONObjectGenerator resultJOG = resultJG.makeObject();
      jsonDesc( resultJOG );
    }
    
    void NamedObject::jsonDesc( Util::JSONObjectGenerator &resultJOG ) const
    {
      {
        Util::JSONGenerator memberJG = resultJOG.makeMember( "type", 4 );
        jsonDescType( memberJG );
      }
      {
        Util::JSONGenerator memberJG = resultJOG.makeMember( "errors", 6 );
        jsonDescErrors( memberJG );
      }
    }
    
    void NamedObject::jsonNotify( char const *cmdData, size_t cmdLength, Util::SimpleString const *arg ) const
    {
      m_context->jsonNotify( m_notificationSrc, cmdData, cmdLength, arg );
    }
    
    void NamedObject::jsonNotifyDelta( Util::SimpleString const &delta ) const
    {
      jsonNotify( "delta", 5, &delta );
    }
    
    void NamedObject::jsonNotifyMemberDelta(
      char const *memberData,
      size_t memberLength,
      Util::SimpleString const &memberDeltaJSON
      ) const
    {
      Util::SimpleString json;
      {
        Util::JSONGenerator jg( &json );
        Util::JSONObjectGenerator jog = jg.makeObject();
        Util::JSONGenerator deltaJG = jog.makeMember( memberData, memberLength );
        deltaJG.appendJSON( memberDeltaJSON );
      }
      jsonNotifyDelta( json );
    }
    
    void NamedObject::jsonNotifyErrorDelta() const
    {
      Util::SimpleString json;
      {
        Util::JSONGenerator jg( &json );
        jsonDescErrors( jg );
      }
      jsonNotifyMemberDelta( "errors", 6, json );
    }
  };
};
