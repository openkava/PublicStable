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

namespace Fabric
{
  namespace AST
  {
    AssignOp::AssignOp( CG::Location const &location, CG::AssignOpType assignOpType, RC::ConstHandle<Expr> const &left, RC::ConstHandle<Expr> const &right )
    : Expr( location )
    , m_assignOpType( assignOpType )
    , m_left( left )
    , m_right( right )
    {
    }
    
    std::string AssignOp::localDesc() const
    {
      char buf[128];
      snprintf( buf, 128, "AssignOp(%s)", CG::assignOpTypeDesc(m_assignOpType) );
      return std::string(buf);
    }
    
    std::string AssignOp::deepDesc( std::string const &indent ) const
    {
      std::string subIndent = indent + "  ";
      return indent + localDesc() + "\n"
      + m_left->deepDesc(subIndent)
      + m_right->deepDesc(subIndent);
    }
    
    RC::ConstHandle<CG::Adapter> AssignOp::getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      return m_left->getType( basicBlockBuilder );
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
        RC::ConstHandle< CG::Symbol > symbol = basicBlockBuilder.getScope().get( name );
        if ( symbol )
        {
          FABRIC_ASSERT( symbol->isFunction() );
          RC::ConstHandle< CG::FunctionSymbol > functionSymbol = RC::ConstHandle< CG::FunctionSymbol >::StaticCast( symbol );
          functionSymbol->llvmCreateCall( basicBlockBuilder, lhsExprValue, rhsExprValue );
          return lhsExprValue;
        }
        
        // [pzion 20110202] Fall back on binOp + simple assignOp composition              
        std::string binOpName = binOpOverloadName( CG::binOpForAssignOp( m_assignOpType ), lhsExprValue.getAdapter(), lhsExprValue.getAdapter() );
        RC::ConstHandle< CG::Symbol > binOpSymbol = basicBlockBuilder.getScope().get( binOpName );
        if ( binOpSymbol )
        {
          FABRIC_ASSERT( binOpSymbol->isFunction() );
          RC::ConstHandle< CG::FunctionSymbol > binOpFunctionSymbol = RC::ConstHandle< CG::FunctionSymbol >::StaticCast( binOpSymbol );
        
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
