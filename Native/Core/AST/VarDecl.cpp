/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "VarDecl.h"
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( VarDecl );
    
    RC::Handle<VarDecl> VarDecl::Create(
      CG::Location const &location,
      std::string const &name,
      std::string const &type
      )
    {
      return new VarDecl( location, name, type );
    }
    
    VarDecl::VarDecl(
      CG::Location const &location,
      std::string const &name,
      std::string const &type
      )
      : Statement( location )
      , m_name( name )
      , m_type( type )
    {
    }
    
    RC::Handle<JSON::Object> VarDecl::toJSON() const
    {
      RC::Handle<JSON::Object> result = Statement::toJSON();
      result->set( "name", JSON::String::Create( m_name ) );
      result->set( "type", JSON::String::Create( m_type ) );
      return result;
    }

    void VarDecl::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      llvmAllocateVariable( basicBlockBuilder, diagnostics );
    }

    CG::ExprValue VarDecl::llvmAllocateVariable( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      RC::ConstHandle<CG::Adapter> adapter = basicBlockBuilder.getAdapter( m_type );
      
      llvm::Value *result = adapter->llvmAlloca( basicBlockBuilder, m_name );
      adapter->llvmInit( basicBlockBuilder, result );
      
      CG::Scope &scope = basicBlockBuilder.getScope();
      if ( scope.has( m_name ) )
        addError( diagnostics, "variable '" + m_name + "' already exists" );
      else scope.put( m_name, CG::VariableSymbol::Create( CG::ExprValue( adapter, CG::USAGE_LVALUE, result ) ) );
        
      return CG::ExprValue( adapter, CG::USAGE_LVALUE, result );
    }
  };
};
