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
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Call );
    
    RC::ConstHandle<Call> Call::Create(
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
    
    void Call::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Expr::appendJSONMembers( jsonObjectGenerator );
      jsonObjectGenerator.makeMember( "functionFriendlyName" ).makeString( m_name );
      m_args->appendJSON( jsonObjectGenerator.makeMember( "args" ) );
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
    
    RC::ConstHandle<CG::Adapter> Call::getType( CG::BasicBlockBuilder &basicBlockBuilder ) const
    {
      RC::ConstHandle<CG::Adapter> adapter = basicBlockBuilder.maybeGetAdapter( m_name );
      if ( !adapter )
        adapter = getFunctionSymbol( basicBlockBuilder )->getReturnInfo().getAdapter();
      if ( adapter )
        adapter->llvmCompileToModule( basicBlockBuilder.getModuleBuilder() );
      return adapter;
    }
    
    void Call::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      m_args->registerTypes( cgManager, diagnostics );
    }
    
    CG::ExprValue Call::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      RC::ConstHandle<CG::Adapter> adapter = basicBlockBuilder.maybeGetAdapter( m_name );
      if ( adapter )
      {
        adapter->llvmCompileToModule( basicBlockBuilder.getModuleBuilder() );
        
        if ( usage == CG::USAGE_LVALUE )
          throw Exception( "temporary values cannot be used in an l-value context" );
        else usage = CG::USAGE_RVALUE;
        
        llvm::Value *thisLValue = adapter->llvmAlloca( basicBlockBuilder, "temp"+adapter->getUserName() );
        adapter->llvmInit( basicBlockBuilder, thisLValue );
        CG::ExprValue result( adapter, CG::USAGE_LVALUE, basicBlockBuilder.getContext(), thisLValue );
        
        std::vector< RC::ConstHandle<CG::Adapter> > argTypes;
        m_args->appendTypes( basicBlockBuilder, argTypes );
        if ( argTypes.size() == 1 && argTypes[0] == adapter )
        {
          std::vector<CG::Usage> usages;
          usages.push_back( CG::USAGE_RVALUE );
          std::vector<CG::ExprValue> exprValues;
          m_args->appendExprValues( basicBlockBuilder, usages, exprValues, "cannot be used as an io argument" );
          FABRIC_ASSERT( exprValues.size() == 1 );
          adapter->llvmAssign( basicBlockBuilder, thisLValue, exprValues[0].getValue() );
          exprValues[0].llvmDispose( basicBlockBuilder );
        }
        else
        {
          std::string initializerName = constructOverloadName( result.getAdapter(), argTypes );
            
          RC::ConstHandle<CG::FunctionSymbol> functionSymbol = basicBlockBuilder.maybeGetFunction( initializerName );
          if ( !functionSymbol )
          {
            if ( argTypes.size() == 1 )
              throw CG::Error( getLocation(), "no cast exists from " + argTypes[0]->getUserName() + " to " + adapter->getUserName() );
            else
            {
              std::string initializerName = adapter->getUserName() + "(";
              for ( size_t i=0; i<argTypes.size(); ++i )
              {
                if ( i > 0 )
                  initializerName += ", ";
                initializerName += argTypes[i]->getUserName();
              }
              initializerName += ")";
              
              throw CG::Error( getLocation(), "initializer " + initializerName + " not found" );
            }
          }

          std::vector<CG::FunctionParam> const functionParams = functionSymbol->getParams();
          
          std::vector<CG::Usage> argUsages;
          for ( size_t i=1; i<functionParams.size(); ++i )
            argUsages.push_back( functionParams[i].getUsage() );
            
          std::vector<CG::ExprValue> exprValues;
          exprValues.push_back( result );
          m_args->appendExprValues( basicBlockBuilder, argUsages, exprValues, "cannot be used as an io argument" );
          
          functionSymbol->llvmCreateCall( basicBlockBuilder, exprValues );
        }
        
        result.castTo( basicBlockBuilder, usage );
        llvm::Value *thisRValue = adapter->llvmLValueToRValue( basicBlockBuilder, thisLValue );
        adapter->llvmRelease( basicBlockBuilder, thisRValue );
        return result;
      }
      else
      {
        RC::ConstHandle<CG::FunctionSymbol> functionSymbol = getFunctionSymbol( basicBlockBuilder );
        if ( usage == CG::USAGE_LVALUE && functionSymbol->getReturnInfo().getUsage() != CG::USAGE_LVALUE )
          throw Exception( "result of function "+_(m_name)+" is not an l-value" );
        
        CG::ExprValue result( basicBlockBuilder.getContext() );
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
    }
  };
};
