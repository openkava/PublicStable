/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "VarDecl.h"
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( VarDecl );
    
    RC::ConstHandle<VarDecl> VarDecl::Create(
      CG::Location const &location,
      std::string const &name,
      std::string const &arrayModifier
      )
    {
      return new VarDecl( location, name, arrayModifier );
    }
    
    VarDecl::VarDecl(
      CG::Location const &location,
      std::string const &name,
      std::string const &arrayModifier
      )
      : Node( location )
      , m_name( name )
      , m_arrayModifier( arrayModifier )
    {
    }
    
    void VarDecl::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Node::appendJSONMembers( jsonObjectGenerator );
      jsonObjectGenerator.makeMember( "name" ).makeString( m_name );
      jsonObjectGenerator.makeMember( "arrayModifier" ).makeString( m_arrayModifier );
    }
    
    void VarDecl::llvmPrepareModule( std::string const &baseType, CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      std::string type = baseType + m_arrayModifier;
      RC::ConstHandle<CG::Adapter> adapter = moduleBuilder.getAdapter( type, getLocation() );
      adapter->llvmPrepareModule( moduleBuilder, true );
    }

    void VarDecl::llvmCompileToBuilder( std::string const &baseType, CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      llvmAllocateVariable( baseType, basicBlockBuilder, diagnostics );
    }

    CG::ExprValue VarDecl::llvmAllocateVariable( std::string const &baseType, CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      std::string type = baseType + m_arrayModifier;
      RC::ConstHandle<CG::Adapter> adapter = basicBlockBuilder.maybeGetAdapter( type );
      if ( !adapter )
        throw CG::Error( getLocation(), "variable type " + _(type) + " not registered" );
      
      llvm::Value *result = adapter->llvmAlloca( basicBlockBuilder, m_name );
      adapter->llvmInit( basicBlockBuilder, result );
      
      CG::Scope &scope = basicBlockBuilder.getScope();
      if ( scope.has( m_name ) )
        addError( diagnostics, ("variable '" + m_name + "' already exists").c_str() );
      else scope.put( m_name, CG::VariableSymbol::Create( CG::ExprValue( adapter, CG::USAGE_LVALUE, result ) ) );
        
      return CG::ExprValue( adapter, CG::USAGE_LVALUE, result );
    }
  };
};
