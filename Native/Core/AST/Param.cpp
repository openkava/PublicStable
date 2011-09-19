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
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Param );
    
    RC::ConstHandle<Param> Param::Create(
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
    
    void Param::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const
    {
      Node::appendJSONMembers( jsonObjectGenerator, includeLocation );
      jsonObjectGenerator.makeMember( "name" ).makeString( m_name );
      jsonObjectGenerator.makeMember( "type" ).makeString( m_type );
      jsonObjectGenerator.makeMember( "usage" ).makeString( CG::usageDesc( m_usage ) );
    }

    CG::FunctionParam Param::getFunctionParam( RC::Handle<CG::Manager> const &cgManager ) const
    {
      return CG::FunctionParam( m_name, getAdapter( cgManager ), m_usage );
    }
    
    RC::ConstHandle<CG::Adapter> Param::getAdapter( RC::Handle<CG::Manager> const &cgManager ) const
    {
      return cgManager->getAdapter( m_type );
    }
    
    CG::ExprType Param::getExprType( RC::Handle<CG::Manager> const &cgManager ) const
    {
      return CG::ExprType( getAdapter( cgManager ), m_usage );
    }
    
    RC::ConstHandle<CG::Adapter> Param::getAdapter( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      RC::ConstHandle<CG::Adapter> result;
      try
      {
        result = cgManager->getAdapter( m_type );
      }
      catch ( Exception e )
      {
        addError( diagnostics, e );
      }
      return result;
    }
    
    void Param::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      getAdapter( cgManager, diagnostics );
    }

    void Param::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const
    {
      getAdapter( moduleBuilder.getManager(), diagnostics )->llvmCompileToModule( moduleBuilder );
    }
  };
};
