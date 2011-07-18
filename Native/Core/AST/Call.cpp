/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/Call.h>
#include <Fabric/Core/AST/ExprVector.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/ExprValue.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Call );
    
    RC::Handle<Call> Call::Create(
      CG::Location const &location,
      std::string const &name,
      RC::ConstHandle<ExprVector> const &args
      )
    {
      return new Call( location, name, args );
    }

    Call::Call(
      CG::Location const &location,
      std::string const &name,
      RC::ConstHandle<ExprVector> const &args
      )
      : Expr( location )
      , m_name( name )
      , m_args( args )
    {
    }
    
    RC::Handle<JSON::Object> Call::toJSON() const
    {
      RC::Handle<JSON::Object> result = Expr::toJSON();
      result->set( "functionFriendlyName", JSON::String::Create( m_name ) );
      result->set( "args", m_args->toJSON() );
      return result;
    }
    
    RC::ConstHandle<CG::FunctionSymbol> Call::getFunctionSymbol( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      RC::ConstHandle<CG::Symbol> symbol = basicBlockBuilder.getScope().get( m_name );
      if ( !symbol )
        throw Exception( "function " + _(m_name) + " not found" );

      if ( !symbol->isFunction() )
        throw Exception( _(m_name) + " is not a function" );
      return RC::ConstHandle<CG::FunctionSymbol>::StaticCast( symbol );
    }
    
    RC::ConstHandle<CG::Adapter> Call::getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      return getFunctionSymbol( basicBlockBuilder )->getReturnInfo().getAdapter();
    }
    
    CG::ExprValue Call::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      RC::ConstHandle<CG::FunctionSymbol> functionSymbol = getFunctionSymbol( basicBlockBuilder );
      if ( usage == CG::USAGE_LVALUE && functionSymbol->getReturnInfo().getUsage() != CG::USAGE_LVALUE )
        throw Exception( "result of function "+_(m_name)+" is not an l-value" );
      
      CG::ExprValue result;
      try
      {
        std::vector<CG::FunctionParam> const functionParams = functionSymbol->getParams();
        
        std::vector<CG::Usage> paramUsages;
        for ( size_t i=0; i<functionParams.size(); ++i )
          paramUsages.push_back( functionParams[i].getUsage() );
          
        if ( paramUsages.size() != m_args->size() )
          throw CG::Error( getLocation(), "incorrect number of arguments: expected "+_(paramUsages.size())+", actual "+_(m_args->size()) );

        std::vector<CG::ExprValue> args;
        m_args->appendExprValues( basicBlockBuilder, paramUsages, args, "cannot be an io argument" );
        
        result = functionSymbol->llvmCreateCall( basicBlockBuilder, args );
      }
      catch ( CG::Error e )
      {
        throw "calling function " + _(m_name) + ": " + e;
      }
      return result.castTo( basicBlockBuilder, usage );
    }
  };
};
