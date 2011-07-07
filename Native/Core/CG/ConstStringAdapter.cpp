/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "ConstStringAdapter.h"
#include "BooleanAdapter.h"
#include "IntegerAdapter.h"
#include "SizeAdapter.h"
#include "StringAdapter.h"
#include "OpaqueAdapter.h"
#include "Manager.h"
#include "ModuleBuilder.h"
#include "FunctionBuilder.h"
#include "BasicBlockBuilder.h"
#include "OverloadNames.h"

#include <Fabric/Core/RT/ConstStringDesc.h>

#include <llvm/Module.h>
#include <llvm/Function.h>

namespace Fabric
{
  namespace CG
  {
    ConstStringAdapter::ConstStringAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::ConstStringDesc> const &constStringDesc )
      : Adapter( manager, constStringDesc, FL_PASS_BY_REFERENCE )
      , m_length( constStringDesc->getLength() )
    {
      setLLVMType( llvm::ArrayType::get( llvm::Type::getInt8Ty( getLLVMContext() ), m_length ) );
    }
    
    void ConstStringAdapter::llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const
    {
      if ( moduleBuilder.contains( getCodeName() ) )
        return;
      
      moduleBuilder->addTypeName( getCodeName(), llvmRawType() );
      
      RC::ConstHandle<BooleanAdapter> booleanAdapter = getManager()->getBooleanAdapter();
      RC::ConstHandle<StringAdapter> stringAdapter = getManager()->getStringAdapter();

      {
        // [pzion 20110202] Cast string to boolean
        std::string name = constructOverloadName( booleanAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "booleanLValue", booleanAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "constStringRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *booleanLValue = functionBuilder[0];
          llvm::Value *constStringRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateStore( booleanAdapter->llvmConst( m_length > 0 ), booleanLValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = constructOverloadName( stringAdapter, this );
        std::vector<FunctionParam> params;
        params.push_back( FunctionParam( "stringLValue", stringAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "constStringRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *stringLValue = functionBuilder[0];
          llvm::Value *constStringRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *constStringLValue = llvmRValueToLValue( basicBlockBuilder, constStringRValue );
          llvm::Value *stringRValue = stringAdapter->llvmCallCast( basicBlockBuilder, this, constStringLValue );
          stringAdapter->llvmAssign( basicBlockBuilder, stringLValue, stringRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
    }
    
    llvm::Constant *ConstStringAdapter::llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      std::vector<llvm::Constant *> elementDefaultRValues;
      elementDefaultRValues.resize( m_length, llvm::ConstantInt::get( llvm::Type::getInt8Ty( getLLVMContext() ), 0 ) );
      return llvm::ConstantArray::get( (llvm::ArrayType const *)llvmRawType(), elementDefaultRValues );
    }
    
    llvm::Constant *ConstStringAdapter::llvmDefaultRValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      return llvmDefaultLValue( basicBlockBuilder );
    }

    llvm::Constant *ConstStringAdapter::llvmDefaultLValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      llvm::Constant *defaultValue = llvmDefaultValue( basicBlockBuilder );
      return new llvm::GlobalVariable(
        *basicBlockBuilder.getModuleBuilder(),
        defaultValue->getType(),
        true,
        llvm::GlobalValue::InternalLinkage,
        defaultValue,
        "__" + getCodeName() + "__DefaultValue"
        );
    }
      
    llvm::Value *ConstStringAdapter::llvmConst( CG::BasicBlockBuilder &basicBlockBuilder, char const *data, size_t length ) const
    {
      FABRIC_ASSERT( length == m_length );
      
      llvm::Type const *charType = llvm::Type::getInt8Ty( getLLVMContext() );
      
      std::vector<llvm::Constant *> chars;
      for ( size_t i=0; i<length; ++i )
        chars.push_back( llvm::ConstantInt::get( charType, data[i] ) );

      llvm::Constant *cStringConstant = llvm::ConstantArray::get( llvm::ArrayType::get( llvm::Type::getInt8Ty( getLLVMContext() ), m_length ), chars );

      return new llvm::GlobalVariable(
        *basicBlockBuilder.getModuleBuilder(),
        cStringConstant->getType(),
        true,
        llvm::GlobalValue::InternalLinkage,
        cStringConstant,
        ""
        );
    }
    
    std::string ConstStringAdapter::toString( void const *data ) const
    {
      return std::string( static_cast<char const *>(data), m_length );
    }

    void ConstStringAdapter::llvmInit( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *value ) const
    {
    }

    void ConstStringAdapter::llvmRetain( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
    }

    void ConstStringAdapter::llvmDefaultAssign( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const
    {
      basicBlockBuilder->CreateStore( srcRValue, dstLValue );
    }
    
    void ConstStringAdapter::llvmRelease( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
    }
    
    void *ConstStringAdapter::llvmResolveExternalFunction( std::string const &functionName ) const
    {
      return 0;
    }
  };
};
