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
    
    RC::Handle<Param> Param::Create(
      CG::Location const &location,
      std::string const &name,
      std::string const &type,
      CG::Usage usage
      )
    {
      return new Param( location, name, type, usage );
    }

    Param::Param(
      CG::Location const &location,
      std::string const &name,
      std::string const &type,
      CG::Usage usage
      )
      : Node( location )
      , m_name( name )
      , m_type( type )
      , m_usage( usage )
    {
    }
    
    RC::Handle<JSON::Object> Param::toJSON() const
    {
      RC::Handle<JSON::Object> result = Node::toJSON();
      result->set( "name", JSON::String::Create( m_name ) );
      result->set( "type", JSON::String::Create( m_type ) );
      result->set( "usage", JSON::String::Create( CG::usageDesc( m_usage ) ) );
      return result;
    }

    CG::FunctionParam Param::getFunctionParam( RC::Handle<CG::Manager> const &cgManager ) const
    {
      return CG::FunctionParam( m_name, getAdapter( cgManager ), m_usage );
    }
    
    RC::ConstHandle<CG::Adapter> Param::getAdapter( RC::Handle<CG::Manager> const &cgManager ) const
    {
      RC::ConstHandle<CG::Adapter> adapter = cgManager->maybeGetAdapter( m_type );
      if ( !adapter )
        throw CG::Error( getLocation(), _(m_type) + ": type not registered" );
      return adapter;
    }
    
    CG::ExprType Param::getExprType( RC::Handle<CG::Manager> const &cgManager ) const
    {
      return CG::ExprType( getAdapter( cgManager ), m_usage );
    }
  };
};
