/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "TempValue.h"
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/OverloadNames.h>

namespace Fabric
{
  namespace AST
  {
    TempValue::TempValue( CG::Location const &location, RC::ConstHandle<CG::Adapter> const &adapter, RC::ConstHandle<ArgList> const &args )
      : Expr( location )
      , m_adapter( adapter )
      , m_args( args )
    {
    }
    
    std::string TempValue::localDesc() const
    {
      return "TempValue( " + _(m_adapter) + " )";
    }
    
    std::string TempValue::deepDesc( std::string const &indent ) const
    {
      std::string subIndent = indent + "  ";
      return indent + localDesc() + "\n"
        + m_args->deepDesc(subIndent);
    }
    
    RC::ConstHandle<CG::Adapter> TempValue::getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      return m_adapter;
    }
    
    CG::ExprValue TempValue::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      if ( usage == CG::USAGE_LVALUE )
        throw Exception( "temporary values cannot be used in an l-value context" );
      else usage = CG::USAGE_RVALUE;
      
      llvm::Value *selfLValue = m_adapter->llvmAlloca( basicBlockBuilder, "temp"+m_adapter->getUserName() );
      m_adapter->llvmInit( basicBlockBuilder, selfLValue );
      CG::ExprValue result( m_adapter, CG::USAGE_LVALUE, selfLValue );
      
      std::vector< RC::ConstHandle<CG::Adapter> > argTypes;
      m_args->appendTypes( basicBlockBuilder, argTypes );
      
      std::string initializerName = constructOverloadName( result.getAdapter(), argTypes );
        
      RC::ConstHandle<CG::FunctionSymbol> functionSymbol = basicBlockBuilder.maybeGetFunction( initializerName );
      if ( !functionSymbol )
        throw Exception( "initializer " + _(initializerName) + " not found" );

      std::vector<CG::FunctionParam> const functionParams = functionSymbol->getParams();
      
      std::vector<CG::Usage> argUsages;
      for ( size_t i=1; i<functionParams.size(); ++i )
        argUsages.push_back( functionParams[i].getUsage() );
        
      std::vector<CG::ExprValue> exprValues;
      exprValues.push_back( result );
      m_args->appendExprValues( basicBlockBuilder, argUsages, exprValues, "cannot be used as an io argument" );
      
      functionSymbol->llvmCreateCall( basicBlockBuilder, exprValues );
      
      return result.castTo( basicBlockBuilder, usage );
    }
  };
};
