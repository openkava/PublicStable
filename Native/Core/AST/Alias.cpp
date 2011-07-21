/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/AST/Alias.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Alias );
    
    Alias::Alias(
      CG::Location const &location,
      std::string const &name,
      std::string const &adapterName
      )
      : Global( location )
      , m_name( name )
      , m_adapterName( adapterName )
    {
    }
    
    RC::Handle<JSON::Object> Alias::toJSON() const
    {
      RC::Handle<JSON::Object> result = Global::toJSON();
      result->set( "newTypeName", JSON::String::Create( m_name ) );
      result->set( "oldTypeName", JSON::String::Create( m_adapterName ) );
      return result;
    }
    
    void Alias::registerTypes( RC::Handle<RT::Manager> const &rtManager, CG::Diagnostics &diagnostics ) const
    {
      RC::ConstHandle<RT::Desc> desc = rtManager->maybeGetDesc( m_adapterName );
      if ( !desc )
        throw CG::Error( getLocation(), "no registered type named " + _(m_adapterName) );
      try
      {
        rtManager->registerAlias( m_name, desc );
      }
      catch ( Exception e )
      {
        addError( diagnostics, e.getDesc() );
      }
    }
  };
};
