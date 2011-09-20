/*
 *  AssignOp.cpp
 *  Fabric
 *
 *  Created by Halfdan Ingvarsson on 11-01-10.
 *  Copyright 2011 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/AssignOp.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( AssignOp );
    
    RC::ConstHandle<AssignOp> AssignOp::Create( CG::Location const &location, CG::AssignOpType assignOpType, RC::ConstHandle<Expr> const &left, RC::ConstHandle<Expr> const &right )
    {
      return new AssignOp( location, assignOpType, left, right );
    }
    
    AssignOp::AssignOp( CG::Location const &location, CG::AssignOpType assignOpType, RC::ConstHandle<Expr> const &left, RC::ConstHandle<Expr> const &right )
      : Expr( location )
      , m_assignOpType( assignOpType )
      , m_left( left )
      , m_right( right )
    {
    }
    
    void AssignOp::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const
    {
      Expr::appendJSONMembers( jsonObjectGenerator, includeLocation );
      jsonObjectGenerator.makeMember( "initialValue" ).makeString( assignOpTypeDesc( m_assignOpType ) );
      m_left->appendJSON( jsonObjectGenerator.makeMember( "lhs" ), includeLocation );
      m_right->appendJSON( jsonObjectGenerator.makeMember( "rhs" ), includeLocation );
    }
    
    void AssignOp::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      m_left->registerTypes( cgManager, diagnostics );
      m_right->registerTypes( cgManager, diagnostics );
    }
    
    RC::ConstHandle<CG::Adapter> AssignOp::getType( CG::BasicBlockBuilder &basicBlockBuilder ) const
    {
      RC::ConstHandle<CG::Adapter> adapter = m_left->getType( basicBlockBuilder );
      adapter->llvmCompileToModule( basicBlockBuilder.getModuleBuilder() );
      return adapter;
    }
    
    CG::ExprValue AssignOp::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      try
      {
        CG::ExprValue lhsExprValue = m_left->buildExprValue( basicBlockBuilder, CG::USAGE_LVALUE, "cannot be assigned to" );

        RC::ConstHandle<CG::Adapter> adapter = lhsExprValue.getAdapter();
        CG::ExprValue rhsExprValue = m_right->buildExprValue( basicBlockBuilder, CG::USAGE_RVALUE, lValueErrorDesc );
          
        if ( m_assignOpType == CG::ASSIGN_OP )
        {
          llvm::Value *rhsCastedRValue = adapter->llvmCast( basicBlockBuilder, rhsExprValue );
          adapter->llvmAssign( basicBlockBuilder, lhsExprValue.getValue(), rhsCastedRValue );
          return lhsExprValue;
        }

        std::string name = methodOverloadName( assignOpMethodName( m_assignOpType ), lhsExprValue.getAdapter(), rhsExprValue.getAdapter() );
        RC::ConstHandle<CG::FunctionSymbol> functionSymbol = basicBlockBuilder.maybeGetFunction( name );
        if ( functionSymbol )
        {
          functionSymbol->llvmCreateCall( basicBlockBuilder, lhsExprValue, rhsExprValue );
          return lhsExprValue;
        }
        
        // [pzion 20110202] Fall back on binOp + simple assignOp composition              
        std::string binOpName = binOpOverloadName( CG::binOpForAssignOp( m_assignOpType ), lhsExprValue.getAdapter(), lhsExprValue.getAdapter() );
        RC::ConstHandle<CG::FunctionSymbol> binOpFunctionSymbol = basicBlockBuilder.maybeGetFunction( binOpName );
        if ( binOpFunctionSymbol )
        {
          CG::ExprValue binOpResultExprValue = binOpFunctionSymbol->llvmCreateCall( basicBlockBuilder, lhsExprValue, rhsExprValue );
          llvm::Value *rhsCastedRValue = adapter->llvmCast( basicBlockBuilder, binOpResultExprValue );
          adapter->llvmAssign( basicBlockBuilder, lhsExprValue.getValue(), rhsCastedRValue );
          return lhsExprValue;
        }
        
        throw CG::Error( getLocation(), "assignment operator " + std::string( CG::assignOpTypeDesc( m_assignOpType ) ) + " not supported for expressions of type " + lhsExprValue.getTypeUserName() );
      }
      catch ( CG::Error e )
      {
        throw "assignment: " + e;
      }
    }
  };
};
