/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "OpaqueAdapter.h"
#include "BooleanAdapter.h"
#include "SizeAdapter.h"
#include "StringAdapter.h"
#include "Manager.h"
#include "ModuleBuilder.h"
#include "FunctionBuilder.h"
#include "BasicBlockBuilder.h"
#include "Scope.h"
#include "OverloadNames.h"

#include <Fabric/Core/RT/OpaqueDesc.h>
#include <Fabric/Core/Util/Hex.h>

// For debugging, if contents of the opaque type are wanted
// when casting from string.
// #define FABRIC_PRINT_OPAQUE

namespace Fabric
{
  namespace CG
  {
    OpaqueAdapter::OpaqueAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::OpaqueDesc> const &opaqueDesc )
      : SimpleAdapter( manager, opaqueDesc )
    {
      size_t size = opaqueDesc->getSize();
      if ( size == sizeof( void * ) )
        setLLVMType( llvm::Type::getInt8PtrTy( manager->getLLVMContext() ) );
      else
      {
        switch ( size )
        {
          case 1:
            setLLVMType( llvm::Type::getInt8Ty( manager->getLLVMContext() ) );
            break;
          case 2:
            setLLVMType( llvm::Type::getInt16Ty( manager->getLLVMContext() ) );
            break;
          case 4:
            setLLVMType( llvm::Type::getInt32Ty( manager->getLLVMContext() ) );
            break;
          case 8:
            setLLVMType( llvm::Type::getInt64Ty( manager->getLLVMContext() ) );
            break;
          default:
            FABRIC_ASSERT( false && "Opaque size must be 1, 2, 4 or 8" );
            break;
        }
      }
    }
    
    llvm::Constant *OpaqueAdapter::llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      if ( getDesc()->getSize() == sizeof( void * ) )
        return llvm::ConstantPointerNull::get( (llvm::PointerType const *)llvmRType() );
      else 
        return llvm::ConstantInt::get( llvmRType(), 0 );
    }

    void OpaqueAdapter::llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const
    {
      if ( moduleBuilder.contains( getCodeName(), buildFunctions ) )
        return;
      
      RC::ConstHandle<BooleanAdapter> booleanAdapter = getManager()->getBooleanAdapter();
      RC::ConstHandle<SizeAdapter> sizeAdapter = getManager()->getSizeAdapter();
      RC::ConstHandle<StringAdapter> stringAdapter = getManager()->getStringAdapter();
      RC::ConstHandle<OpaqueAdapter> dataAdapter = getManager()->getDataAdapter();
      
      {
        std::string name = constructOverloadName( booleanAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "booleanLValue", booleanAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "opaqueRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *booleanLValue = functionBuilder[0];
          llvm::Value *opaqueRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *booleanRValue = basicBlockBuilder->CreateIsNotNull( opaqueRValue );
          booleanAdapter->llvmAssign( basicBlockBuilder, booleanLValue, booleanRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = constructOverloadName( stringAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "stringLValue", stringAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "opaqueRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *stringLValue = functionBuilder[0];
          llvm::Value *opaqueRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *opaqueLValue = llvmRValueToLValue( basicBlockBuilder, opaqueRValue );
          llvm::Value *stringRValue = stringAdapter->llvmCallCast( basicBlockBuilder, this, opaqueLValue );
          stringAdapter->llvmAssign( basicBlockBuilder, stringLValue, stringRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = methodOverloadName( "dataSize", this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "selfRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( sizeAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *dataSizeRValue = sizeAdapter->llvmConst( getDesc()->getSize() );
          basicBlockBuilder->CreateRet( dataSizeRValue );
        }
      }
      
      {
        std::string name = methodOverloadName( "data", this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "selfLValue", this, USAGE_LVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( dataAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *selfLValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreatePointerCast( selfLValue, dataAdapter->llvmRType() ) );
        }
      }
    }

    void OpaqueAdapter::llvmInit( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *lValue ) const
    {
      basicBlockBuilder->CreateStore( llvm::Constant::getNullValue( llvmRType() ), lValue ); 
    }
    
    std::string OpaqueAdapter::toString( void const *data ) const
    {
#if defined( FABRIC_PRINT_OPAQUE )
      return "<" + Util::hexBuf( getDesc()->getSize(), data ) + ">";
#else
      return "<Opaque>";
#endif
    }
  }; // namespace CG
}; // namespace FABRIC
