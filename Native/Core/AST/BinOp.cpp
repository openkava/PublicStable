/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/BinOp.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace AST
  {
    BinOp::BinOp( CG::Location const &location, CG::BinOpType binOpType, RC::ConstHandle<Expr> const &left, RC::ConstHandle<Expr> const &right )
      : Expr( location )
      , m_binOpType( binOpType )
      , m_left( left )
      , m_right( right )
    {
    }
    
    std::string BinOp::localDesc() const
    {
      return "BinOp( " + _(m_binOpType) + " )";
    }
    
    std::string BinOp::deepDesc( std::string const &indent ) const
    {
      std::string subIndent = indent + "  ";
      return indent + localDesc() + "\n"
        + m_left->deepDesc(subIndent)
        + m_right->deepDesc(subIndent);
    }
    
    RC::ConstHandle<CG::FunctionSymbol> BinOp::getFunctionSymbol( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      RC::ConstHandle<CG::Adapter> lhsType = m_left->getType( basicBlockBuilder );
      RC::ConstHandle<CG::Adapter> rhsType = m_right->getType( basicBlockBuilder );
      
      std::string functionName = CG::binOpOverloadName( m_binOpType, lhsType, rhsType );
      RC::ConstHandle<CG::FunctionSymbol> functionSymbol = basicBlockBuilder.maybeGetFunction( functionName );
      if ( functionSymbol )
        return functionSymbol;
      
      // [pzion 20110317] Fall back on stronger type
      
      RC::ConstHandle<RT::Desc> castDesc = RT::getStrongerTypeOrNone( lhsType->getDesc(), rhsType->getDesc() );
      if ( castDesc )
      {
        RC::ConstHandle<CG::Adapter> castType = basicBlockBuilder.getManager()->getAdapter( castDesc );

        functionName = CG::binOpOverloadName( m_binOpType, castType, castType );
        functionSymbol = basicBlockBuilder.maybeGetFunction( functionName );
        if ( functionSymbol )
          return functionSymbol;
      }

      throw Exception( "binary operator " + _(CG::binOpUserName( m_binOpType )) + " not supported for types " + _(lhsType->getUserName()) + " and " + _(rhsType->getUserName()) );
    }
    
    RC::ConstHandle<CG::Adapter> BinOp::getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      return getFunctionSymbol( basicBlockBuilder )->getReturnInfo().getAdapter();
    }
    
    CG::ExprValue BinOp::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      RC::ConstHandle< CG::FunctionSymbol > functionSymbol = getFunctionSymbol( basicBlockBuilder );
      std::vector<CG::FunctionParam> const &functionParams = functionSymbol->getParams();
        
      if ( usage == CG::USAGE_LVALUE )
        throw Exception( "the result of " + functionParams[0].getAdapter()->getUserName() + " " + CG::binOpUserName( m_binOpType ) + " " + functionParams[1].getAdapter()->getUserName() + " is not an l-value" );
      else usage = CG::USAGE_RVALUE;
      
      CG::ExprValue result;
      CG::ExprValue lhsExprValue = m_left->buildExprValue( basicBlockBuilder, functionParams[0].getUsage(), lValueErrorDesc );
      CG::ExprValue rhsExprValue = m_right->buildExprValue( basicBlockBuilder, functionParams[1].getUsage(), lValueErrorDesc );
      try
      {
        result = functionSymbol->llvmCreateCall( basicBlockBuilder, lhsExprValue, rhsExprValue );
      }
      catch ( Exception e )
      {
        throw CG::Error( getLocation(), "in expression " + functionParams[0].getAdapter()->getUserName() + " " + CG::binOpUserName( m_binOpType ) + " " + functionParams[1].getAdapter()->getUserName() + ": " + e );
      }
      return result;
    }
  };
};
