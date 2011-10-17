/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "DictAdapter.h"
#include "BooleanAdapter.h"
#include "ComparableAdapter.h"
#include "IntegerAdapter.h"
#include "SizeAdapter.h"
#include "ConstStringAdapter.h"
#include "StringAdapter.h"
#include "OpaqueAdapter.h"
#include "Manager.h"
#include "ModuleBuilder.h"
#include "FunctionBuilder.h"
#include "BasicBlockBuilder.h"
#include "OverloadNames.h"

#include <Fabric/Core/RT/DictDesc.h>
#include <Fabric/Core/RT/Impl.h>

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Intrinsics.h>
#include <llvm/Type.h>

namespace Fabric
{
  namespace CG
  {
    DictAdapter::DictAdapter(
      RC::ConstHandle<Manager> const &manager,
      RC::ConstHandle<RT::DictDesc> const &dictDesc
      )
      : Adapter( manager, dictDesc, FL_PASS_BY_REFERENCE )
      , m_dictDesc( dictDesc )
      , m_keyAdapter( RC::ConstHandle<ComparableAdapter>::StaticCast( manager->getAdapter( dictDesc->getKeyDesc() ) ) )
      , m_valueAdapter( manager->getAdapter( dictDesc->getValueDesc() ) )
    {
    }
    
    llvm::Type const *DictAdapter::buildLLVMRawType( RC::Handle<Context> const &context ) const
    {
      llvm::LLVMContext &llvmContext = context->getLLVMContext();
      
      llvm::Type const *llvmSizeTy = llvmSizeType( context );
      
      /*
      struct node_t
      {
        node_t *bitsPrevNode;
        node_t *bitsNextNode;
        node_t *bucketPrevNode;
        node_t *bucketNextNode;
        size_t keyHash;
        char keyAndValue[0];
      };
      */
      
      llvm::StructType *llvmAbstractNodeType = llvm::StructType::get( llvmContext, true );
      std::vector<llvm::Type const *> llvmNodeTypeMembers;
      llvmNodeTypeMembers.push_back( llvmAbstractNodeType->getPointerTo() ); // bitsPrevNode
      llvmNodeTypeMembers.push_back( llvmAbstractNodeType->getPointerTo() ); // bitsNextNode
      llvmNodeTypeMembers.push_back( llvmAbstractNodeType->getPointerTo() ); // bucketPrevNode
      llvmNodeTypeMembers.push_back( llvmAbstractNodeType->getPointerTo() ); // bucketNextNode
      llvmNodeTypeMembers.push_back( llvmSizeTy ); // keyHash
      llvmNodeTypeMembers.push_back( llvm::ArrayType::get( llvm::Type::getInt8Ty( llvmContext ), 0 ) );
      llvm::StructType *llvmNodeType = llvm::StructType::get( llvmContext, llvmNodeTypeMembers, true );
      llvmNodeType->refineAbstractType( llvmAbstractNodeType, llvmNodeType );
      
      /*
      struct bucket_t
      {
        node_t *firstNode;
        node_t *lastNode;
      };
      */

      std::vector<llvm::Type const *> llvmBucketTypeMembers;
      llvmBucketTypeMembers.push_back( llvmNodeType->getPointerTo() ); // firstNode
      llvmBucketTypeMembers.push_back( llvmNodeType->getPointerTo() ); // lastNode
      llvm::Type const *llvmBucketType = llvm::StructType::get( llvmContext, llvmBucketTypeMembers, true );
      
      /*
      struct bits_t
      {
        size_t bucketCount;
        size_t nodeCount;
        node_t *firstNode;
        node_t *lastNode;
        bucket_t *buckets;
      };
      */

      std::vector<llvm::Type const *> llvmBitsTypeMembers;
      llvmBitsTypeMembers.push_back( llvmSizeTy ); // bucketCount
      llvmBitsTypeMembers.push_back( llvmSizeTy ); // nodeCount
      llvmBitsTypeMembers.push_back( llvmNodeType->getPointerTo() ); // numMembers
      llvmBitsTypeMembers.push_back( llvmNodeType->getPointerTo() ); // numMembers
      llvmBitsTypeMembers.push_back( llvmBucketType->getPointerTo() ); // numMembers
      llvm::Type const *implType = llvm::StructType::get( llvmContext, llvmBitsTypeMembers, true );
      
      return implType;
    }

    void DictAdapter::llvmCompileToModule( ModuleBuilder &moduleBuilder ) const
    {
      if ( moduleBuilder.haveCompiledToModule( getCodeName() ) )
        return;
      
      RC::Handle<Context> context = moduleBuilder.getContext();
      
      m_keyAdapter->llvmCompileToModule( moduleBuilder );
      m_valueAdapter->llvmCompileToModule( moduleBuilder );
      RC::ConstHandle<BooleanAdapter> booleanAdapter = getManager()->getBooleanAdapter();
      booleanAdapter->llvmCompileToModule( moduleBuilder );
      RC::ConstHandle<SizeAdapter> sizeAdapter = getManager()->getSizeAdapter();
      sizeAdapter->llvmCompileToModule( moduleBuilder );
      RC::ConstHandle<StringAdapter> stringAdapter = getManager()->getStringAdapter();
      stringAdapter->llvmCompileToModule( moduleBuilder );
      
      llvm::StructType const *llvmBitsType = static_cast<llvm::StructType const *>( llvmRawType( context ) );
      moduleBuilder->addTypeName( getCodeName() + "Bits", llvmBitsType );
      llvm::PointerType const *llvmBucketPtrType = static_cast<llvm::PointerType const *>( llvmBitsType->getTypeAtIndex( 4 ) );
      llvm::StructType const *llvmBucketType = static_cast<llvm::StructType const *>( llvmBucketPtrType->getElementType() );
      moduleBuilder->addTypeName( getCodeName() + "Bucket", llvmBucketType );
      llvm::PointerType const *llvmNodePtrType = static_cast<llvm::PointerType const *>( llvmBucketType->getTypeAtIndex( unsigned(0) ) );
      llvm::Type const *llvmNodeType = static_cast<llvm::PointerType const *>( llvmNodePtrType->getElementType() );
      moduleBuilder->addTypeName( getCodeName() + "Node", llvmNodeType );
      
      static const bool buildFunctions = true;
      
      {
        std::string name = methodOverloadName( "size", this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "rValue", this, CG::USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( sizeAdapter, USAGE_RVALUE ), params, false );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          
          llvm::Value *rValue = functionBuilder[0];

          llvm::BasicBlock *entryBB = functionBuilder.createBasicBlock( "entry" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *lValue = llvmRValueToLValue( basicBlockBuilder, rValue );
          llvm::Value *nodeCountLValue = basicBlockBuilder->CreateStructGEP( lValue, 1 );
          llvm::Value *nodeCountRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, nodeCountLValue );
          basicBlockBuilder->CreateRet( nodeCountRValue );
        }
      }

      {
        std::string name = constructOverloadName( booleanAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "booleanLValue", booleanAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "rValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *booleanLValue = functionBuilder[0];
          llvm::Value *rValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *sizeRValue = llvmCallSize( basicBlockBuilder, rValue );
          llvm::Value *booleanRValue = basicBlockBuilder->CreateICmpNE( sizeRValue, llvm::ConstantInt::get( sizeRValue->getType(), 0 ) );
          booleanAdapter->llvmAssign( basicBlockBuilder, booleanLValue, booleanRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = constructOverloadName( stringAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "stringLValue", stringAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "rValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *stringLValue = functionBuilder[0];
          llvm::Value *rValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *lValue = llvmRValueToLValue( basicBlockBuilder, rValue );
          llvm::Value *stringRValue = stringAdapter->llvmCallCast( basicBlockBuilder, this, lValue );
          stringAdapter->llvmAssign( basicBlockBuilder, stringLValue, stringRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
    }
    
    void *DictAdapter::llvmResolveExternalFunction( std::string const &functionName ) const
    {
      return Adapter::llvmResolveExternalFunction( functionName );
    }

    void DictAdapter::llvmRetain( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
    }
    
    void DictAdapter::llvmRelease( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
    }

    void DictAdapter::llvmDefaultAssign( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const
    {
      FABRIC_ASSERT( false && "DictAdapter::llvmDefaultAssign: unimplmemented" );
    }
    
    llvm::Constant *DictAdapter::llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      RC::Handle<Context> context = basicBlockBuilder.getContext();
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();

      llvm::StructType const *llvmBitsType = static_cast<llvm::StructType const *>( llvmRawType( context ) );
      llvm::PointerType const *llvmBucketPtrType = static_cast<llvm::PointerType const *>( llvmBitsType->getTypeAtIndex( 4 ) );
      llvm::StructType const *llvmBucketType = static_cast<llvm::StructType const *>( llvmBucketPtrType->getElementType() );
      llvm::PointerType const *llvmNodePtrType = static_cast<llvm::PointerType const *>( llvmBucketType->getTypeAtIndex( unsigned(0) ) );

      std::vector<llvm::Constant *> nodeDefaultRValues;
      nodeDefaultRValues.push_back( sizeAdapter->llvmConst( context, 0 ) );
      nodeDefaultRValues.push_back( sizeAdapter->llvmConst( context, 0 ) );
      llvm::Constant *llvmNodePtrNull = llvm::ConstantPointerNull::get( llvmNodePtrType );
      nodeDefaultRValues.push_back( llvmNodePtrNull );
      nodeDefaultRValues.push_back( llvmNodePtrNull );
      llvm::Constant *llvmBucketPtrNull = llvm::ConstantPointerNull::get( llvmBucketPtrType );
      nodeDefaultRValues.push_back( llvmBucketPtrNull );
      return llvm::ConstantStruct::get( context->getLLVMContext(), nodeDefaultRValues, true );
    }
      
    llvm::Constant *DictAdapter::llvmDefaultRValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      return llvmDefaultLValue( basicBlockBuilder );
    }

    llvm::Constant *DictAdapter::llvmDefaultLValue( BasicBlockBuilder &basicBlockBuilder ) const
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
    
    std::string DictAdapter::toString( void const *data ) const
    {
      return m_dictDesc->descData( data );
    }
    
    llvm::Value *DictAdapter::llvmCallSize( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();
      std::vector<FunctionParam> params;
      params.push_back( FunctionParam( "rValue", this, CG::USAGE_RVALUE ) );
      std::string name = methodOverloadName( "size", this );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), name, ExprType( sizeAdapter, USAGE_RVALUE ), params, false );
      return basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), rValue );
    }
  };
};
