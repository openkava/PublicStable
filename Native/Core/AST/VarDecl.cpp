/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "VarDecl.h"
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Scope.h>

namespace Fabric
{
  namespace AST
  {
    VarDecl::VarDecl( CG::Location const &location, std::string const &name, RC::ConstHandle< CG::Adapter > const &adapter )
      : Statement( location )
      , m_name( name )
      , m_adapter( adapter )
    {
    }
    
    std::string VarDecl::localDesc() const
    {
      return "VarDecl( "+_(m_name)+", "+_(m_adapter)+" )";
    }

    void VarDecl::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      llvmAllocateVariable( basicBlockBuilder, diagnostics );
    }

    CG::ExprValue VarDecl::llvmAllocateVariable( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      llvm::Value *result = m_adapter->llvmAlloca( basicBlockBuilder, m_name );
      m_adapter->llvmInit( basicBlockBuilder, result );
      
      CG::Scope &scope = basicBlockBuilder.getScope();
      if ( scope.has( m_name ) )
        addError( diagnostics, "variable '" + m_name + "' already exists" );
      else scope.put( m_name, CG::VariableSymbol::Create( CG::ExprValue( m_adapter, CG::USAGE_LVALUE, result ) ) );
        
      return CG::ExprValue( m_adapter, CG::USAGE_LVALUE, result );
    }
  };
};
