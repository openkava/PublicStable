/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "ConditionalStatement.h"
#include <Fabric/Core/CG/BooleanAdapter.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/FunctionBuilder.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( ConditionalStatement );
    
    RC::ConstHandle<ConditionalStatement> ConditionalStatement::Create(
      CG::Location const &location,
      RC::ConstHandle<Expr> const &expr,
      RC::ConstHandle<Statement> const &trueStatement,
      RC::ConstHandle<Statement> const &falseStatement
      )
    {
      return new ConditionalStatement( location, expr, trueStatement, falseStatement );
    }

    ConditionalStatement::ConditionalStatement(
      CG::Location const &location,
      RC::ConstHandle<Expr> const &expr,
      RC::ConstHandle<Statement> const &trueStatement,
      RC::ConstHandle<Statement> const &falseStatement
      )
      : Statement( location )
      , m_expr( expr )
      , m_trueStatement( trueStatement )
      , m_falseStatement( falseStatement )
    {
    }
    
    RC::Handle<JSON::Object> ConditionalStatement::toJSON() const
    {
      RC::Handle<JSON::Object> result = Statement::toJSON();
      result->set( "testExpr", m_expr->toJSON() );
      if ( m_trueStatement )
        result->set( "ifTrue", m_trueStatement->toJSON() );
      if ( m_falseStatement )
        result->set( "ifFalse", m_falseStatement->toJSON() );
      return result;
    }
    
    void ConditionalStatement::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      m_expr->llvmPrepareModule( moduleBuilder, diagnostics );
      if ( m_trueStatement )
        m_trueStatement->llvmPrepareModule( moduleBuilder, diagnostics );
      if ( m_falseStatement )
        m_falseStatement->llvmPrepareModule( moduleBuilder, diagnostics );
    }

    void ConditionalStatement::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      llvm::BasicBlock *trueBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "cond_true" );
      llvm::BasicBlock *falseBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "cond_false" );
      llvm::BasicBlock *doneBB = 0;

      try
      {
        CG::ExprValue exprExprValue = m_expr->buildExprValue( basicBlockBuilder, CG::USAGE_RVALUE, "cannot be an l-value" );
        RC::ConstHandle<CG::BooleanAdapter> booleanAdapter = basicBlockBuilder.getManager()->getBooleanAdapter();
        llvm::Value *exprBoolValue = booleanAdapter->llvmCast( basicBlockBuilder, exprExprValue );
        basicBlockBuilder->CreateCondBr( exprBoolValue, trueBB, falseBB );
        
        basicBlockBuilder->SetInsertPoint( trueBB );
        if ( m_trueStatement )
          m_trueStatement->llvmCompileToBuilder( basicBlockBuilder, diagnostics );
        if ( !basicBlockBuilder->GetInsertBlock()->getTerminator() )
        {
          if ( !doneBB )
            doneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "cond_done" );
          basicBlockBuilder->CreateBr( doneBB );
        }
        
        basicBlockBuilder->SetInsertPoint( falseBB );
        if ( m_falseStatement )
          m_falseStatement->llvmCompileToBuilder( basicBlockBuilder, diagnostics );
        if ( !basicBlockBuilder->GetInsertBlock()->getTerminator() )
        {
          if ( !doneBB )
            doneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "cond_done" );
          basicBlockBuilder->CreateBr( doneBB );
        }
        
        if ( doneBB )
          basicBlockBuilder->SetInsertPoint( doneBB );
      }
      catch ( CG::Error e )
      {
        addError( diagnostics, e );
      }
      catch ( Exception e )
      {
        addError( diagnostics, e );
      }
    }
  };
};
