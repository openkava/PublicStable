#include "Scope.h"

#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/RT/Impl.h>

#include <llvm/Function.h>

namespace Fabric
{
  namespace CG
  {
    ExprValue FunctionSymbol::llvmCreateCall( BasicBlockBuilder &basicBlockBuilder, std::vector<ExprValue> &args ) const
    {
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
        returnValue = returnAdapter->llvmAlloca( basicBlockBuilder, "retVal" );
        returnAdapter->llvmInit( basicBlockBuilder, returnValue );
        args.resize( args.size() + 1 );
        for ( size_t i=1; i<args.size(); ++i )
          args[args.size()-i] = args[args.size()-i-1];
        args[0] = ExprValue( returnAdapter, USAGE_LVALUE, returnValue );
      }
      
      std::vector<llvm::Value *> argValues;
      for ( size_t i=0; i<args.size(); ++i )
        argValues.push_back( args[i].getValue() );
      
      llvm::Value *resultValue = basicBlockBuilder->CreateCall( m_llvmFunction, argValues.begin(), argValues.end() );
      if( !m_returnInfo.usesReturnLValue() )
        returnValue = resultValue;

      for ( size_t i=0; i<args.size(); ++i )
        args[i].llvmDispose( basicBlockBuilder );

      return CG::ExprValue( m_returnInfo.getExprType(), returnValue );
    }
  };
};
