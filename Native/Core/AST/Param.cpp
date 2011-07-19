/*
 *
 *  Created by Peter Zion on 10-12-09.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "Param.h"
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Location.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Param );
    
    Param::Param( CG::Location const &location, std::string const &name, std::string const &adapterName, CG::Usage usage )
      : Node( location )
      , m_name( name )
      , m_adapterName( adapterName )
      , m_usage( usage )
    {
    }
    
    RC::Handle<JSON::Object> Param::toJSON() const
    {
      RC::Handle<JSON::Object> result = Node::toJSON();
      result->set( "name", JSON::String::Create( m_name ) );
      result->set( "type", JSON::String::Create( m_adapterName ) );
      result->set( "usage", JSON::String::Create( CG::usageDesc( m_usage ) ) );
      return result;
    }

    CG::FunctionParam Param::getFunctionParam( RC::Handle<CG::Manager> const &cgManager ) const
    {
      RC::ConstHandle<CG::Adapter> adapter = cgManager->maybeGetAdapter( m_adapterName );
      if ( !adapter )
        throw Exception( _(m_adapterName) + ": type not registered" );
      return CG::FunctionParam( m_name, adapter, m_usage );
    }
  };
};
