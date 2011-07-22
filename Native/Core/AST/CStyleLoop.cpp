/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/CStyleLoop.h>
#include <Fabric/Core/CG/BooleanAdapter.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/FunctionBuilder.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( CStyleLoop );
    
    RC::ConstHandle<CStyleLoop> CStyleLoop::Create(
      CG::Location const &location,
      RC::ConstHandle<Statement> const &startStatement,
      RC::ConstHandle<Expr> const &preCondExpr,
      RC::ConstHandle<Expr> const &nextExpr,
      RC::ConstHandle<Expr> const &postCondExpr,
      RC::ConstHandle<Statement> const &body
      )
    {
      return new CStyleLoop( location, startStatement, preCondExpr, nextExpr, postCondExpr, body );
    }
    
    CStyleLoop::CStyleLoop(
        CG::Location const &location,
        RC::ConstHandle<Statement> const &startStatement,
        RC::ConstHandle<Expr> const &preCondExpr,
        RC::ConstHandle<Expr> const &nextExpr,
        RC::ConstHandle<Expr> const &postCondExpr,
        RC::ConstHandle<Statement> const &body
      )
      : Statement( location )
      , m_startStatement( startStatement )
      , m_preCondExpr( preCondExpr )
      , m_nextExpr( nextExpr )
      , m_postCondExpr( postCondExpr )
      , m_body( body )
    {
    }
    
    RC::Handle<JSON::Object> CStyleLoop::toJSONImpl() const
    {
      RC::Handle<JSON::Object> result = Statement::toJSONImpl();
      if ( m_startStatement )
        result->set( "startStatement", m_startStatement->toJSON() );
      if ( m_preCondExpr )
        result->set( "preCondExpr", m_preCondExpr->toJSON() );
      if ( m_nextExpr )
        result->set( "nextExpr", m_nextExpr->toJSON() );
      if ( m_postCondExpr )
        result->set( "postCondExpr", m_postCondExpr->toJSON() );
      if ( m_body )
        result->set( "body", m_body->toJSON() );
      return result;
    }
    
    void CStyleLoop::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      if ( m_startStatement )
        m_startStatement->llvmPrepareModule( moduleBuilder, diagnostics );
      if ( m_preCondExpr )
        m_preCondExpr->llvmPrepareModule( moduleBuilder, diagnostics );
      if ( m_nextExpr )
        m_nextExpr->llvmPrepareModule( moduleBuilder, diagnostics );
      if ( m_postCondExpr )
        m_postCondExpr->llvmPrepareModule( moduleBuilder, diagnostics );
      if ( m_body )
        m_body->llvmPrepareModule( moduleBuilder, diagnostics );
    }

    void CStyleLoop::llvmCompileToBuilder( CG::BasicBlockBuilder &parentBasicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      try
      {
        RC::ConstHandle<CG::BooleanAdapter> booleanAdapter = parentBasicBlockBuilder.getManager()->getBooleanAdapter();
        
        llvm::BasicBlock *checkPreCondBB = parentBasicBlockBuilder.getFunctionBuilder().createBasicBlock( "loopCheckPreCond" );
        llvm::BasicBlock *bodyBB = parentBasicBlockBuilder.getFunctionBuilder().createBasicBlock( "loopBody" );
        llvm::BasicBlock *stepBB = parentBasicBlockBuilder.getFunctionBuilder().createBasicBlock( "loopStep" );
        llvm::BasicBlock *checkPostCondBB = parentBasicBlockBuilder.getFunctionBuilder().createBasicBlock( "loopCheckPostCond" );
        llvm::BasicBlock *endBB = parentBasicBlockBuilder.getFunctionBuilder().createBasicBlock( "loopEnd" );
        
        CG::LoopScope loopScope( parentBasicBlockBuilder.getScope(), endBB, stepBB );
        CG::BasicBlockBuilder loopBasicBlockBuilder( parentBasicBlockBuilder, loopScope );
        
        if ( m_startStatement )
          m_startStatement->llvmCompileToBuilder( loopBasicBlockBuilder, diagnostics );
        loopBasicBlockBuilder->CreateBr( checkPreCondBB );
        
        loopBasicBlockBuilder->SetInsertPoint( checkPreCondBB );
        llvm::Value *preCondExprBoolRValue;
        if ( m_preCondExpr )
        {
          CG::ExprValue preCondExprRValue = m_preCondExpr->buildExprValue( loopBasicBlockBuilder, CG::USAGE_RVALUE, "cannot be an l-value" );
          preCondExprBoolRValue = booleanAdapter->llvmCast( loopBasicBlockBuilder, preCondExprRValue );
        }
        else preCondExprBoolRValue = booleanAdapter->llvmConst( true );
        loopBasicBlockBuilder->CreateCondBr( preCondExprBoolRValue, bodyBB, endBB );
        
        loopBasicBlockBuilder->SetInsertPoint( bodyBB );
        if ( m_body )
          m_body->llvmCompileToBuilder( loopBasicBlockBuilder, diagnostics );
        loopBasicBlockBuilder->CreateBr( stepBB );
        
        loopBasicBlockBuilder->SetInsertPoint( stepBB );
        if ( m_nextExpr )
          m_nextExpr->buildExprValue( loopBasicBlockBuilder, CG::USAGE_UNSPECIFIED, "cannot be an l-value" );
        loopBasicBlockBuilder->CreateBr( checkPostCondBB );
        
        loopBasicBlockBuilder->SetInsertPoint( checkPostCondBB );
        llvm::Value *postCondExprBoolRValue;
        if ( m_postCondExpr )
        {
          CG::ExprValue postCondExprRValue = m_postCondExpr->buildExprValue( loopBasicBlockBuilder, CG::USAGE_RVALUE, "cannot be an l-value" );
          postCondExprBoolRValue = booleanAdapter->llvmCast( loopBasicBlockBuilder, postCondExprRValue );
        }
        else postCondExprBoolRValue = booleanAdapter->llvmConst( true );
        loopBasicBlockBuilder->CreateCondBr( postCondExprBoolRValue, checkPreCondBB, endBB );
        
        loopBasicBlockBuilder->SetInsertPoint( endBB );
        loopScope.llvmUnwind( loopBasicBlockBuilder );
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
