/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/SwitchStatement.h>
#include <Fabric/Core/AST/Expr.h>
#include <Fabric/Core/AST/CaseVector.h>
#include <Fabric/Core/AST/Case.h>
#include <Fabric/Core/AST/StatementVector.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/FunctionBuilder.h>
#include <Fabric/Core/CG/BooleanAdapter.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Core/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( SwitchStatement );
    
    RC::ConstHandle<SwitchStatement> SwitchStatement::Create(
      CG::Location const &location,
      RC::ConstHandle<Expr> const &expr,
      RC::ConstHandle<CaseVector> const &cases
      )
    {
      return new SwitchStatement( location, expr, cases );
    }
    
    SwitchStatement::SwitchStatement(
      CG::Location const &location,
      RC::ConstHandle<Expr> const &expr,
      RC::ConstHandle<CaseVector> const &cases
      )
      : Statement( location )
      , m_expr( expr )
      , m_cases( cases )
    {
    }
    
    void SwitchStatement::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Statement::appendJSONMembers( jsonObjectGenerator );
      m_expr->appendJSON( jsonObjectGenerator.makeMember( "expr" ) );
      m_cases->appendJSON( jsonObjectGenerator.makeMember( "cases" ) );
    }
    
    void SwitchStatement::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      m_expr->llvmPrepareModule( moduleBuilder, diagnostics );
      m_cases->llvmPrepareModule( moduleBuilder, diagnostics );
    }

    void SwitchStatement::llvmCompileToBuilder( CG::BasicBlockBuilder &parentBasicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      try
      {
        CG::FunctionBuilder &functionBuilder = parentBasicBlockBuilder.getFunctionBuilder();
        RC::ConstHandle<CG::BooleanAdapter> booleanAdapter = parentBasicBlockBuilder.getManager()->getBooleanAdapter();

        size_t numCases = m_cases->size();
        std::vector<llvm::BasicBlock *> caseBodyBBs;
        llvm::BasicBlock *caseDefaultBodyBB = 0;
        for ( size_t i=0; i<numCases; ++i )
        {
          llvm::BasicBlock *caseBodyBB = functionBuilder.createBasicBlock( "switchBody"+_(i+1) );
          caseBodyBBs.push_back( caseBodyBB );

          RC::ConstHandle<Case> case_ = m_cases->get(i);
          if ( !case_->getExpr() )
          {
            if ( caseDefaultBodyBB )
              throw CG::Error( case_->getLocation(), "cannot have more than one 'default' clause in a switch statement" );
            caseDefaultBodyBB = caseBodyBB;
          }
        }
        
        llvm::BasicBlock *doneBB = functionBuilder.createBasicBlock( "switchDone" );
        if ( !caseDefaultBodyBB )
          caseDefaultBodyBB = doneBB;
        
        std::vector<llvm::BasicBlock *> caseTestBBs;
        for ( size_t i=0; i<numCases; ++i )
        {
          RC::ConstHandle<Case> case_ = m_cases->get(i);
          if ( case_->getExpr() )
            caseTestBBs.push_back( functionBuilder.createBasicBlock( "switchTest"+_(i+1) ) );
        }
        caseTestBBs.push_back( caseDefaultBodyBB );

        CG::SwitchScope switchScope( parentBasicBlockBuilder.getScope(), doneBB );
        CG::BasicBlockBuilder basicBlockBuilder( parentBasicBlockBuilder, switchScope );
        
        CG::ExprValue exprValue = m_expr->buildExprValue( basicBlockBuilder, CG::USAGE_RVALUE, "cannot be an l-value" );
        basicBlockBuilder->CreateBr( caseTestBBs[0] );
        
        size_t caseIndex = 0;
        for ( size_t i=1; i<caseTestBBs.size(); ++i )
        {
          basicBlockBuilder->SetInsertPoint( caseTestBBs[i-1] );

          RC::ConstHandle<Case> case_;
          for (;;)
          {
            case_ = m_cases->get(caseIndex);
            if ( case_->getExpr() )
              break;
            ++caseIndex;
          }
          
          CG::ExprValue caseExprValue = case_->getExpr()->buildExprValue( basicBlockBuilder, CG::USAGE_RVALUE, "cannot be an l-value" );

          std::string eqFunctionName = CG::binOpOverloadName( CG::BIN_OP_EQ, exprValue.getAdapter(), caseExprValue.getAdapter() );
          RC::ConstHandle<CG::FunctionSymbol> functionSymbol;
          functionSymbol = basicBlockBuilder.maybeGetFunction( eqFunctionName );
          if ( !functionSymbol )
          {
            eqFunctionName = CG::binOpOverloadName( CG::BIN_OP_EQ, exprValue.getAdapter(), exprValue.getAdapter() );
            functionSymbol = basicBlockBuilder.maybeGetFunction( eqFunctionName );
            if ( !functionSymbol )
              throw Exception( "binary operator " + _(CG::binOpUserName(CG::BIN_OP_EQ)) + " not supported for types " + _(exprValue.getTypeUserName()) + " and " + _(caseExprValue.getTypeUserName()) );
            CG::ExprValue newCaseExprValue( exprValue.getAdapter(), CG::USAGE_RVALUE, exprValue.getAdapter()->llvmCast( basicBlockBuilder, caseExprValue ) );
            caseExprValue.llvmDispose( basicBlockBuilder );
            caseExprValue = newCaseExprValue;
          }
          
          exprValue.llvmRetain( basicBlockBuilder );
          CG::ExprValue cmpExprValue = functionSymbol->llvmCreateCall( basicBlockBuilder, exprValue, caseExprValue );
          llvm::Value *cmpBooleanRValue = booleanAdapter->llvmCast( basicBlockBuilder, cmpExprValue );
          cmpExprValue.llvmDispose( basicBlockBuilder );
          
          basicBlockBuilder->CreateCondBr( cmpBooleanRValue, caseBodyBBs[caseIndex], caseTestBBs[i] );
          
          ++caseIndex;
        }
        
        for ( size_t i=0; i<numCases; ++i )
        {
          RC::ConstHandle<Case> case_ = m_cases->get(i);

          basicBlockBuilder->SetInsertPoint( caseBodyBBs[i] );
          case_->getStatements()->llvmCompileToBuilder( basicBlockBuilder, diagnostics );
          if ( !basicBlockBuilder->GetInsertBlock()->getTerminator() )
            basicBlockBuilder->CreateBr( i+1 == numCases? doneBB: caseBodyBBs[i+1] );
        }
        basicBlockBuilder->SetInsertPoint( doneBB );
        
        exprValue.llvmDispose( basicBlockBuilder );
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
