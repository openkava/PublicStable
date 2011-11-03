/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/CG/ComparableAdapter.h>
#include <Fabric/Core/CG/IntegerAdapter.h>
#include <Fabric/Core/CG/FunctionBuilder.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Core/CG/SizeAdapter.h>
#include <Fabric/Core/RT/ComparableDesc.h>

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Intrinsics.h>

namespace Fabric
{
  namespace CG
  {
    ComparableAdapter::ComparableAdapter(
      RC::ConstHandle<Manager> const &manager,
      RC::ConstHandle<RT::ComparableDesc> const &comparableDesc,
      Flags flags
      )
      : Adapter( manager, comparableDesc, flags )
    {
    }

    llvm::Value *ComparableAdapter::llvmHash( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = getManager()->getSizeAdapter();
      std::vector<FunctionParam> params;
      params.push_back( FunctionParam( "rValue", this, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), methodOverloadName( "hash", this ), ExprType( sizeAdapter, USAGE_RVALUE ), params, false );
      return basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), rValue );
    }
    
    llvm::Value *ComparableAdapter::llvmCompare( BasicBlockBuilder &basicBlockBuilder, llvm::Value *lhsRValue, llvm::Value *rhsRValue ) const
    {
      RC::ConstHandle<IntegerAdapter> integerAdapter = getManager()->getIntegerAdapter();
      std::vector<FunctionParam> params;
      params.push_back( FunctionParam( "lhsRValue", this, CG::USAGE_RVALUE ) );
      params.push_back( FunctionParam( "rhsRValue", this, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), methodOverloadName( "compare", this, this ), ExprType( integerAdapter, USAGE_RVALUE ), params, false );
      return basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), lhsRValue, rhsRValue );
    }
  };
};
