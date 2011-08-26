/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/CStyleLoop.h>
#include <Fabric/Core/AST/Expr.h>
#include <Fabric/Core/CG/BooleanAdapter.h>
#include <Fabric/Core/CG/Context.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/FunctionBuilder.h>
#include <Fabric/Base/Util/SimpleString.h>

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
    
    void CStyleLoop::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Statement::appendJSONMembers( jsonObjectGenerator );
      if ( m_startStatement )
        m_startStatement->appendJSON( jsonObjectGenerator.makeMember( "startStatement" ) );
      if ( m_preCondExpr )
        m_preCondExpr->appendJSON( jsonObjectGenerator.makeMember( "preCondExpr" ) );
      if ( m_nextExpr )
        m_nextExpr->appendJSON( jsonObjectGenerator.makeMember( "nextExpr" ) );
      if ( m_postCondExpr )
        m_postCondExpr->appendJSON( jsonObjectGenerator.makeMember( "postCondExpr" ) );
      if ( m_body )
        m_body->appendJSON( jsonObjectGenerator.makeMember( "body" ) );
    }
    
    void CStyleLoop::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      if ( m_startStatement )
        m_startStatement->registerTypes( cgManager, diagnostics );
      if ( m_preCondExpr )
        m_preCondExpr->registerTypes( cgManager, diagnostics );
      if ( m_nextExpr )
        m_nextExpr->registerTypes( cgManager, diagnostics );
      if ( m_postCondExpr )
        m_postCondExpr->registerTypes( cgManager, diagnostics );
      if ( m_body )
        m_body->registerTypes( cgManager, diagnostics );
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
        else preCondExprBoolRValue = booleanAdapter->llvmConst( loopBasicBlockBuilder.getContext(), true );
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
        else postCondExprBoolRValue = booleanAdapter->llvmConst( loopBasicBlockBuilder.getContext(), true );
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
