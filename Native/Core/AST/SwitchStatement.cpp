/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/SwitchStatement.h>
#include <Fabric/Core/AST/Expr.h>
#include <Fabric/Core/AST/CaseList.h>
#include <Fabric/Core/AST/Case.h>
#include <Fabric/Core/AST/StatementList.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/FunctionBuilder.h>
#include <Fabric/Core/CG/BooleanAdapter.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/OverloadNames.h>

namespace Fabric
{
  namespace AST
  {
    RC::Handle<SwitchStatement> SwitchStatement::Create(
      CG::Location const &location,
      RC::ConstHandle<Expr> const &expr,
      RC::ConstHandle<CaseList> const &caseList
      )
    {
      return new SwitchStatement( location, expr, caseList );
    }
    
    SwitchStatement::SwitchStatement(
      CG::Location const &location,
      RC::ConstHandle<Expr> const &expr,
      RC::ConstHandle<CaseList> const &caseList
      )
      : Statement( location )
      , m_expr( expr )
      , m_caseList( caseList )
    {
    }
    
    std::string SwitchStatement::localDesc() const
    {
      return "SwitchStatement";
    }
    
    std::string SwitchStatement::deepDesc( std::string const &indent ) const
    {
      std::string subIndent = indent + "  ";
      return indent + localDesc() + "\n"
        + m_expr->deepDesc( subIndent )
        + m_caseList->deepDesc( subIndent );
    }

    void SwitchStatement::llvmCompileToBuilder( CG::BasicBlockBuilder &parentBasicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      try
      {
        CG::FunctionBuilder &functionBuilder = parentBasicBlockBuilder.getFunctionBuilder();
        RC::ConstHandle<CG::BooleanAdapter> booleanAdapter = parentBasicBlockBuilder.getManager()->getBooleanAdapter();

        size_t numCases = m_caseList->getNumCases();
        std::vector<llvm::BasicBlock *> caseBodyBBs;
        llvm::BasicBlock *caseDefaultBodyBB = 0;
        for ( size_t i=0; i<numCases; ++i )
        {
          llvm::BasicBlock *caseBodyBB = functionBuilder.createBasicBlock( "switchBody"+_(i+1) );
          caseBodyBBs.push_back( caseBodyBB );

          RC::ConstHandle<Case> case_ = m_caseList->getCase(i);
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
          RC::ConstHandle<Case> case_ = m_caseList->getCase(i);
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
            case_ = m_caseList->getCase(caseIndex);
            if ( case_->getExpr() )
              break;
            ++caseIndex;
          }
          
          CG::ExprValue caseExprValue = case_->getExpr()->buildExprValue( basicBlockBuilder, CG::USAGE_RVALUE, "cannot be an l-value" );

          RC::ConstHandle<CG::Symbol> symbol;
          std::string eqFunctionName = CG::binOpOverloadName( CG::BIN_OP_EQ, exprValue.getAdapter(), caseExprValue.getAdapter() );
          symbol = switchScope.get( eqFunctionName );
          if ( !symbol )
          {
            eqFunctionName = CG::binOpOverloadName( CG::BIN_OP_EQ, exprValue.getAdapter(), exprValue.getAdapter() );
            symbol = switchScope.get( eqFunctionName );
            if ( !symbol )
              throw Exception( "binary operator " + _(CG::binOpUserName(CG::BIN_OP_EQ)) + " not supported for types " + _(exprValue.getTypeUserName()) + " and " + _(caseExprValue.getTypeUserName()) );
            CG::ExprValue newCaseExprValue( exprValue.getAdapter(), CG::USAGE_RVALUE, exprValue.getAdapter()->llvmCast( basicBlockBuilder, caseExprValue ) );
            caseExprValue.llvmDispose( basicBlockBuilder );
            caseExprValue = newCaseExprValue;
          }
          FABRIC_ASSERT( symbol->isFunction() );
          RC::ConstHandle<CG::FunctionSymbol> functionSymbol = RC::ConstHandle<CG::FunctionSymbol>::StaticCast( symbol );
          
          CG::ExprValue cmpExprValue = functionSymbol->llvmCreateCall( basicBlockBuilder, exprValue, caseExprValue );
          llvm::Value *cmpBooleanRValue = booleanAdapter->llvmCast( basicBlockBuilder, cmpExprValue );
          cmpExprValue.llvmDispose( basicBlockBuilder );
          
          basicBlockBuilder->CreateCondBr( cmpBooleanRValue, caseBodyBBs[caseIndex], caseTestBBs[i] );
          
          ++caseIndex;
        }
        
        for ( size_t i=0; i<numCases; ++i )
        {
          RC::ConstHandle<Case> case_ = m_caseList->getCase(i);

          basicBlockBuilder->SetInsertPoint( caseBodyBBs[i] );
          case_->getStatementList()->llvmCompileToBuilder( basicBlockBuilder, diagnostics );
          if ( !basicBlockBuilder->GetInsertBlock()->getTerminator() )
            basicBlockBuilder->CreateBr( i+1 == numCases? doneBB: caseBodyBBs[i+1] );
        }
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
