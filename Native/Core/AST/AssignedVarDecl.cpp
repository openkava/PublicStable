/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/AssignedVarDecl.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( AssignedVarDecl );
    
    AssignedVarDecl::AssignedVarDecl(
      CG::Location const &location,
      std::string const &name,
      RC::ConstHandle< CG::Adapter > const &adapter,
      RC::ConstHandle<Expr> const &initialExpr
      )
      : VarDecl( location, name, adapter )
      , m_initialExpr( initialExpr )
    {
    }
    
    RC::Handle<JSON::Object> AssignedVarDecl::toJSON() const
    {
      RC::Handle<JSON::Object> result = VarDecl::toJSON();
      result->set( "initialValue", m_initialExpr->toJSON() );
      return result;
    }

    void AssignedVarDecl::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      CG::ExprValue result = VarDecl::llvmAllocateVariable( basicBlockBuilder, diagnostics );
      
      CG::ExprValue initialExprExprValue;
      try
      {
        initialExprExprValue = m_initialExpr->buildExprValue( basicBlockBuilder, CG::USAGE_RVALUE, "cannot be an l-value" );
      }
      catch ( CG::Error e )
      {
        addError( diagnostics, e );
      }
      
      if( initialExprExprValue.isValid() )
      {
        llvm::Value *initialExprCastedRValue = result.getAdapter()->llvmCast( basicBlockBuilder, initialExprExprValue );
        result.getAdapter()->llvmAssign( basicBlockBuilder, result.getValue(), initialExprCastedRValue );
      }
      else addError( diagnostics, "Invalid expression for variable assignment" );
    }
  };
};
