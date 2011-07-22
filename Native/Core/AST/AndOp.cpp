/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/AST/AndOp.h>
#include <Fabric/Core/CG/BooleanAdapter.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/FunctionBuilder.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( AndOp );
    
    RC::ConstHandle<AndOp> AndOp::Create( CG::Location const &location, RC::ConstHandle<Expr> const &left, RC::ConstHandle<Expr> const &right )
    {
      return new AndOp( location, left, right );
    }

    AndOp::AndOp( CG::Location const &location, RC::ConstHandle<Expr> const &left, RC::ConstHandle<Expr> const &right )
      : Expr( location )
      , m_left( left )
      , m_right( right )
    {
    }
    
    RC::Handle<JSON::Object> AndOp::toJSONImpl() const
    {
      RC::Handle<JSON::Object> result = Expr::toJSONImpl();
      if ( m_left )
        result->set( "lhs", m_left->toJSON() );
      if ( m_right )
        result->set( "rhs", m_right->toJSON() );
      return result;
    }
    
    RC::ConstHandle<CG::Adapter> AndOp::getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      RC::ConstHandle<CG::Adapter> lhsType = m_left->getType( basicBlockBuilder );
      RC::ConstHandle<CG::Adapter> rhsType = m_right->getType( basicBlockBuilder );
  
      RC::ConstHandle<CG::Adapter> adapter;
      if ( lhsType && rhsType )
      {
        // The true/false value types need to be "equivalent". We'll cast into whoever wins the
        // casting competition, or fail if they can't.
        RC::ConstHandle<RT::Desc> castType = basicBlockBuilder.getStrongerTypeOrNone( lhsType->getDesc(), rhsType->getDesc() );
        if ( !castType )
          throw CG::Error( getLocation(), "types " + _(lhsType->getUserName()) + " and " + _(rhsType->getUserName()) + " are unrelated" );
        adapter = lhsType->getManager()->getAdapter( castType );
      }
      return adapter;
    }
    
    void AndOp::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      m_left->llvmPrepareModule( moduleBuilder, diagnostics );
      m_right->llvmPrepareModule( moduleBuilder, diagnostics );
    }
    
    CG::ExprValue AndOp::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      if ( usage == CG::USAGE_LVALUE )
        throw CG::Error( getLocation(), "the result of a boolean 'and' operation " + lValueErrorDesc );
      else usage = CG::USAGE_RVALUE;
      
      RC::ConstHandle<CG::BooleanAdapter> booleanAdapter = basicBlockBuilder.getManager()->getBooleanAdapter();
      RC::ConstHandle<CG::Adapter> castAdapter = getType( basicBlockBuilder );
      
      llvm::BasicBlock *lhsTrueBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "andLHSTrue" );
      llvm::BasicBlock *lhsFalseBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "andLHSFalse" );
      llvm::BasicBlock *mergeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "andMerge" );

      CG::ExprValue lhsExprValue = m_left->buildExprValue( basicBlockBuilder, usage, lValueErrorDesc );
      llvm::Value *lhsBooleanRValue = booleanAdapter->llvmCast( basicBlockBuilder, lhsExprValue );
      basicBlockBuilder->CreateCondBr( lhsBooleanRValue, lhsTrueBB, lhsFalseBB );

      basicBlockBuilder->SetInsertPoint( lhsTrueBB );
      CG::ExprValue rhsExprValue = m_right->buildExprValue( basicBlockBuilder, usage, lValueErrorDesc );
      
      llvm::Value *rhsCastedRValue;
      if ( castAdapter )
        rhsCastedRValue = castAdapter->llvmCast( basicBlockBuilder, rhsExprValue );
      llvm::BasicBlock *lhsTruePredBB = basicBlockBuilder->GetInsertBlock();
      basicBlockBuilder->CreateBr( mergeBB );
      
      basicBlockBuilder->SetInsertPoint( lhsFalseBB );
      llvm::Value *lhsCastedRValue;
      if ( castAdapter )
        lhsCastedRValue = castAdapter->llvmCast( basicBlockBuilder, lhsExprValue );
      llvm::BasicBlock *lhsFalsePredBB = basicBlockBuilder->GetInsertBlock();
      basicBlockBuilder->CreateBr( mergeBB );

      basicBlockBuilder->SetInsertPoint( mergeBB );
      if ( castAdapter )
      {
        llvm::PHINode *phi = basicBlockBuilder->CreatePHI( castAdapter->llvmRType() );
        phi->addIncoming( rhsCastedRValue, lhsTruePredBB );
        phi->addIncoming( lhsCastedRValue, lhsFalsePredBB );
        return CG::ExprValue( castAdapter, usage, phi );
      }
      else return CG::ExprValue();
    }
  };
};
