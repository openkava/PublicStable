/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/CG/Function.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Core/CG/PencilSymbol.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/Symbol.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/RT/Impl.h>

#include <llvm/Function.h>

namespace Fabric
{
  namespace CG
  {
    bool Function::isExactMatch( ExprTypeVector const &argTypes ) const
    {
      if ( argTypes.size() != m_params.size() )
        return false;
      
      for ( size_t i=0; i<argTypes.size(); ++i )
      {
        if ( argTypes[i] == m_params[i].getExprType() )
          continue;
        
        return false;
      }
      
      return true;
    }
    
    bool Function::isLValueToRValueMatch( ExprTypeVector const &argTypes ) const
    {
      if ( argTypes.size() != m_params.size() )
        return false;
      
      for ( size_t i=0; i<argTypes.size(); ++i )
      {
        if ( argTypes[i] == m_params[i].getExprType() )
          continue;
          
        if ( argTypes[i].getAdapter() == m_params[i].getAdapter()
          && argTypes[i].getUsage() == CG::USAGE_LVALUE && m_params[i].getUsage() == CG::USAGE_RVALUE )
          continue;
          
        return false;
      }
      
      return true;
    }
    
    bool Function::isImplicitCastMatch( ExprTypeVector const &argTypes, ModuleBuilder const &moduleBuilder, size_t &maxCost ) const
    {
      if ( argTypes.size() != m_params.size() )
        return false;
     
      maxCost = 0;
       
      for ( size_t i=0; i<argTypes.size(); ++i )
      {
        if ( argTypes[i] == m_params[i].getExprType() )
          continue;
        
        if ( m_params[i].getUsage() == CG::USAGE_RVALUE )
        {
          if ( argTypes[i].getAdapter() == m_params[i].getAdapter()
            && argTypes[i].getUsage() == CG::USAGE_LVALUE )
            continue;
          
          Function const *function = moduleBuilder.maybeGetPreciseFunction(
            ConstructorPencilName( m_params[i].getAdapter() ),
            ExprType( m_params[i].getAdapter(), USAGE_LVALUE ),
            ExprType( argTypes[i].getAdapter(), USAGE_RVALUE )
            );
          if ( function )
          {
            maxCost += function->getCost();
            continue;
          }
        }
          
        return false;
      }
      
      return true;
    }
    
    ExprValue Function::llvmCreateCall( BasicBlockBuilder &basicBlockBuilder, std::vector<ExprValue> &args ) const
    {
      RC::Handle<Context> context = basicBlockBuilder.getContext();
      
      if ( m_params.size() != args.size() )
        throw Exception( "incorrect number of arguments (expected %u, actual %u)", (unsigned)m_params.size(), (unsigned)args.size() );

      for ( size_t i=0; i<args.size(); ++i )
      {
        FunctionParam const &param = m_params[i];
        ExprValue &arg = args[i];

        if( !arg.isValid() )
          throw Exception( "expression is invalid" );
        
        try
        {
          arg.castTo( basicBlockBuilder, param.getExprType() );
        }
        catch ( Exception e )
        {
          throw Exception( "argument %u ('%s'): %s", (unsigned)(i + 1), param.getName().c_str(), (const char*)e.getDesc() );
        }
      }

      llvm::Value *returnValue = 0;
      if ( m_returnInfo.usesReturnLValue() )
      {
        RC::ConstHandle<Adapter> returnAdapter = m_returnInfo.getAdapter();
        returnValue = returnAdapter->llvmAlloca( basicBlockBuilder, "resultLValue" );
        returnAdapter->llvmInit( basicBlockBuilder, returnValue );
        args.resize( args.size() + 1, ExprValue(context) );
        for ( size_t i=1; i<args.size(); ++i )
          args[args.size()-i] = args[args.size()-i-1];
        args[0] = ExprValue( returnAdapter, USAGE_LVALUE, context, returnValue );
        basicBlockBuilder.getScope().put( CG::VariableSymbol::Create( args[0] ) );
      }
      
      std::vector<llvm::Value *> argValues;
      for ( size_t i=0; i<args.size(); ++i )
        argValues.push_back( args[i].getValue() );
      
      llvm::Value *resultValue = basicBlockBuilder->CreateCall( m_llvmFunction, argValues.begin(), argValues.end() );
      if( !m_returnInfo.usesReturnLValue() )
        returnValue = resultValue;

      return CG::ExprValue( m_returnInfo.getExprType(), context, returnValue );
    }
  }
}
