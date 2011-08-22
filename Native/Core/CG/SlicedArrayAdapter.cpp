/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "SlicedArrayAdapter.h"
#include "VariableArrayAdapter.h"
#include "BooleanAdapter.h"
#include "SizeAdapter.h"
#include "ConstStringAdapter.h"
#include "StringAdapter.h"
#include "OpaqueAdapter.h"
#include "Manager.h"
#include "ModuleBuilder.h"
#include "FunctionBuilder.h"
#include "BasicBlockBuilder.h"
#include "OverloadNames.h"

#include <Fabric/Core/RT/SlicedArrayDesc.h>
#include <Fabric/Core/RT/Impl.h>

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Intrinsics.h>

namespace Fabric
{
  namespace CG
  {
    SlicedArrayAdapter::SlicedArrayAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::SlicedArrayDesc> const &slicedArrayDesc )
      : ArrayAdapter( manager, slicedArrayDesc, FL_PASS_BY_REFERENCE )
      , m_slicedArrayDesc( slicedArrayDesc )
      , m_memberAdapter( manager->getAdapter( slicedArrayDesc->getMemberDesc() ) )
      , m_variableArrayAdapter( manager->getVariableArrayOf( m_memberAdapter ) )
    {
      llvm::Type const *llvmSizeTy = llvmSizeType();
      
      std::vector<llvm::Type const *> memberLLVMTypes;
      memberLLVMTypes.push_back( llvmSizeTy ); // offset
      memberLLVMTypes.push_back( llvmSizeTy ); // size
      memberLLVMTypes.push_back( m_variableArrayAdapter->llvmRType() ); // variableArrayBits
      m_implType = llvm::StructType::get( getLLVMContext(), memberLLVMTypes, true );
      
      setLLVMType( m_implType );
    }

    void SlicedArrayAdapter::llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const
    {
      if ( moduleBuilder.contains( getCodeName(), buildFunctions ) )
        return;
      m_memberAdapter->llvmPrepareModule( moduleBuilder, buildFunctions );
      m_variableArrayAdapter->llvmPrepareModule( moduleBuilder, buildFunctions );
      
      moduleBuilder->addTypeName( getCodeName(), llvmRawType() );

      RC::ConstHandle<BooleanAdapter> booleanAdapter = getManager()->getBooleanAdapter();
      RC::ConstHandle<SizeAdapter> sizeAdapter = getManager()->getSizeAdapter();
      RC::ConstHandle<StringAdapter> stringAdapter = getManager()->getStringAdapter();
      RC::ConstHandle<OpaqueAdapter> dataAdapter = getManager()->getDataAdapter();
      
      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__Retain", ExprType(), params, false );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *rValue = functionBuilder[0];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *slicedArayLValue = llvmRValueToLValue( basicBlockBuilder, rValue );
          llvm::Value *variableArrayLValue = basicBlockBuilder->CreateConstGEP2_32( slicedArayLValue, 0, 2 );
          llvm::Value *variableArrayRValue = m_variableArrayAdapter->llvmLValueToRValue( basicBlockBuilder, variableArrayLValue );
          m_variableArrayAdapter->llvmRetain( basicBlockBuilder, variableArrayRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }

      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__Release", ExprType(), params, false );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *rValue = functionBuilder[0];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *slicedArayLValue = llvmRValueToLValue( basicBlockBuilder, rValue );
          llvm::Value *variableArrayLValue = basicBlockBuilder->CreateConstGEP2_32( slicedArayLValue, 0, 2 );
          llvm::Value *variableArrayRValue = m_variableArrayAdapter->llvmLValueToRValue( basicBlockBuilder, variableArrayLValue );
          m_variableArrayAdapter->llvmRelease( basicBlockBuilder, variableArrayRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }

      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "slicedArayLValue", this, CG::USAGE_LVALUE ) );
        params.push_back( FunctionParam( "sizeRValue", sizeAdapter, CG::USAGE_RVALUE ) );
        std::string overloadName = constructOverloadName( this, sizeAdapter );
        FunctionBuilder functionBuilder( moduleBuilder, overloadName, ExprType(), params, false );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *slicedArayLValue = functionBuilder[0];
          llvm::Value *sizeRValue = functionBuilder[1];
        
          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *offsetLValue = basicBlockBuilder->CreateConstGEP2_32( slicedArayLValue, 0, 0 );
          sizeAdapter->llvmDefaultAssign( basicBlockBuilder, offsetLValue, sizeAdapter->llvmConst( 0 ) );
          llvm::Value *sizeLValue = basicBlockBuilder->CreateConstGEP2_32( slicedArayLValue, 0, 1 );
          sizeAdapter->llvmDefaultAssign( basicBlockBuilder, sizeLValue, sizeRValue );
          llvm::Value *variableArrayLValue = basicBlockBuilder->CreateConstGEP2_32( slicedArayLValue, 0, 2 );
          m_variableArrayAdapter->llvmInit( basicBlockBuilder, variableArrayLValue );
          m_variableArrayAdapter->llvmCallResize( basicBlockBuilder, variableArrayLValue, sizeRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::vector<FunctionParam> params;
        params.push_back( FunctionParam( "dstSlicedArrayLValue", this, CG::USAGE_LVALUE ) );
        params.push_back( FunctionParam( "srcSlicedArrayRValue", this, CG::USAGE_RVALUE ) );
        params.push_back( FunctionParam( "offsetRValue", sizeAdapter, CG::USAGE_RVALUE ) );
        params.push_back( FunctionParam( "sizeRValue", sizeAdapter, CG::USAGE_RVALUE ) );
        std::vector< RC::ConstHandle<CG::Adapter> > paramTypes;
        paramTypes.push_back( this );
        paramTypes.push_back( sizeAdapter );
        paramTypes.push_back( sizeAdapter );
        std::string overloadName = constructOverloadName( this, paramTypes );
        FunctionBuilder functionBuilder( moduleBuilder, overloadName, ExprType(), params, false );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *dstSlicedArrayLValue = functionBuilder[0];
          llvm::Value *srcSlicedArrayRValue = functionBuilder[1];
          llvm::Value *offsetRValue = functionBuilder[2];
          llvm::Value *sizeRValue = functionBuilder[3];
        
          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *offsetLValue = basicBlockBuilder->CreateConstGEP2_32( dstSlicedArrayLValue, 0, 0 );
          sizeAdapter->llvmDefaultAssign( basicBlockBuilder, offsetLValue, offsetRValue );
          llvm::Value *sizeLValue = basicBlockBuilder->CreateConstGEP2_32( dstSlicedArrayLValue, 0, 1 );
          sizeAdapter->llvmDefaultAssign( basicBlockBuilder, sizeLValue, sizeRValue );
          llvm::Value *srcSlicedArrayLValue = llvmRValueToLValue( basicBlockBuilder, srcSlicedArrayRValue );
          llvm::Value *srcVariableArrayLValue = basicBlockBuilder->CreateConstGEP2_32( srcSlicedArrayLValue, 0, 2 );
          llvm::Value *srcVariableArrayRValue = m_variableArrayAdapter->llvmLValueToRValue( basicBlockBuilder, srcVariableArrayLValue );
          llvm::Value *dstVariableArrayLValue = basicBlockBuilder->CreateConstGEP2_32( dstSlicedArrayLValue, 0, 2 );
          m_variableArrayAdapter->llvmInit( basicBlockBuilder, dstVariableArrayLValue );
          m_variableArrayAdapter->llvmRetain( basicBlockBuilder, srcVariableArrayRValue );
          m_variableArrayAdapter->llvmDefaultAssign( basicBlockBuilder, dstVariableArrayLValue, srcVariableArrayRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "dstLValue", this, CG::USAGE_LVALUE ) );
        params.push_back( FunctionParam( "srcRValue", this, CG::USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__DefaultAssign", ExprType(), params, false );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *dstLValue = functionBuilder[0];
          llvm::Value *srcRValue = functionBuilder[1];
        
          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *srcLValue = llvmRValueToLValue( basicBlockBuilder, srcRValue );
          llvm::Value *srcOffsetLValue = basicBlockBuilder->CreateConstGEP2_32( srcLValue, 0, 0 );
          llvm::Value *srcOffsetRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, srcOffsetLValue );
          llvm::Value *dstOffsetLValue = basicBlockBuilder->CreateConstGEP2_32( dstLValue, 0, 0 );
          sizeAdapter->llvmDefaultAssign( basicBlockBuilder, dstOffsetLValue, srcOffsetRValue );
          llvm::Value *srcSizeLValue = basicBlockBuilder->CreateConstGEP2_32( srcLValue, 0, 1 );
          llvm::Value *srcSizeRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, srcSizeLValue );
          llvm::Value *dstSizeLValue = basicBlockBuilder->CreateConstGEP2_32( dstLValue, 0, 1 );
          sizeAdapter->llvmDefaultAssign( basicBlockBuilder, dstSizeLValue, srcSizeRValue );
          llvm::Value *srcVariableArrayLValue = basicBlockBuilder->CreateConstGEP2_32( srcLValue, 0, 2 );
          llvm::Value *srcVariableArrayRValue = m_variableArrayAdapter->llvmLValueToRValue( basicBlockBuilder, srcVariableArrayLValue );
          llvm::Value *dstVariableArrayLValue = basicBlockBuilder->CreateConstGEP2_32( dstLValue, 0, 2 );
          m_variableArrayAdapter->llvmDefaultAssign( basicBlockBuilder, dstVariableArrayLValue, srcVariableArrayRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
        params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__ConstIndex", ExprType( m_memberAdapter, USAGE_RVALUE ), params, false, true );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *arrayRValue = functionBuilder[0];
          llvm::Value *indexRValue = functionBuilder[1];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          llvm::BasicBlock *inRangeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "inRange" );
          llvm::BasicBlock *outOfRangeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "outOfRange" );

          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *arrayLValue = llvmRValueToLValue( basicBlockBuilder, arrayRValue );
          llvm::Value *sizeLValue = basicBlockBuilder->CreateConstGEP2_32( arrayLValue, 0, 1 );
          llvm::Value *sizeRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, sizeLValue );
          llvm::Value *inRangeCond = basicBlockBuilder->CreateICmpULT( indexRValue, sizeRValue );
          basicBlockBuilder->CreateCondBr( inRangeCond, inRangeBB, outOfRangeBB );
          
          basicBlockBuilder->SetInsertPoint( inRangeBB );
          llvm::Value *offsetLValue = basicBlockBuilder->CreateConstGEP2_32( arrayLValue, 0, 0 );
          llvm::Value *offsetRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, offsetLValue );
          llvm::Value *absoluteIndexRValue = basicBlockBuilder->CreateAdd( offsetRValue, indexRValue );
          llvm::Value *variableArrayLValue = basicBlockBuilder->CreateConstGEP2_32( arrayLValue, 0, 2 );
          llvm::Value *variableArrayRValue = m_variableArrayAdapter->llvmLValueToRValue( basicBlockBuilder, variableArrayLValue );
          basicBlockBuilder->CreateRet( m_variableArrayAdapter->llvmConstIndexOp_NoCheck( basicBlockBuilder, variableArrayRValue, absoluteIndexRValue ) );
          
          basicBlockBuilder->SetInsertPoint( outOfRangeBB );
          std::string errorMsg = "KL: "+getUserName()+" index out-of-bounds";
          RC::ConstHandle<ConstStringAdapter> errorConstStringAdapter = getManager()->getConstStringAdapter( errorMsg.length() );
          ExprValue errorExprValue( errorConstStringAdapter, USAGE_RVALUE, errorConstStringAdapter->llvmConst( basicBlockBuilder, errorMsg ) );
          llvm::Value *errorStringRValue = stringAdapter->llvmCast( basicBlockBuilder, errorExprValue );
          stringAdapter->llvmReport( basicBlockBuilder, errorStringRValue );
          stringAdapter->llvmRelease( basicBlockBuilder, errorStringRValue );
          llvm::Value *defaultRValue = m_memberAdapter->llvmDefaultRValue( basicBlockBuilder );
          m_memberAdapter->llvmRetain( basicBlockBuilder, defaultRValue );
          basicBlockBuilder->CreateRet( defaultRValue );
        }
      }

      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "array", this, CG::USAGE_LVALUE ) );
        params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__NonConstIndex", ExprType( m_memberAdapter, USAGE_LVALUE ), params, false, true );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *arrayLValue = functionBuilder[0];
          llvm::Value *indexRValue = functionBuilder[1];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          llvm::BasicBlock *inRangeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "inRange" );
          llvm::BasicBlock *outOfRangeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "outOfRange" );

          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *sizeLValue = basicBlockBuilder->CreateConstGEP2_32( arrayLValue, 0, 1 );
          llvm::Value *sizeRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, sizeLValue );
          llvm::Value *inRangeCond = basicBlockBuilder->CreateICmpULT( indexRValue, sizeRValue );
          basicBlockBuilder->CreateCondBr( inRangeCond, inRangeBB, outOfRangeBB );
          
          basicBlockBuilder->SetInsertPoint( inRangeBB );
          llvm::Value *offsetLValue = basicBlockBuilder->CreateConstGEP2_32( arrayLValue, 0, 0 );
          llvm::Value *offsetRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, offsetLValue );
          llvm::Value *absoluteIndexRValue = basicBlockBuilder->CreateAdd( offsetRValue, indexRValue );
          llvm::Value *variableArrayLValue = basicBlockBuilder->CreateConstGEP2_32( arrayLValue, 0, 2 );
          basicBlockBuilder->CreateRet( m_variableArrayAdapter->llvmNonConstIndexOp_NoCheckNoSplit( basicBlockBuilder, variableArrayLValue, absoluteIndexRValue ) );
          
          basicBlockBuilder->SetInsertPoint( outOfRangeBB );
          std::string errorMsg = "KL: "+getUserName()+" index out-of-bounds";
          RC::ConstHandle<ConstStringAdapter> errorConstStringAdapter = getManager()->getConstStringAdapter( errorMsg.length() );
          ExprValue errorExprValue( errorConstStringAdapter, USAGE_RVALUE, errorConstStringAdapter->llvmConst( basicBlockBuilder, errorMsg ) );
          llvm::Value *errorStringRValue = stringAdapter->llvmCast( basicBlockBuilder, errorExprValue );
          stringAdapter->llvmReport( basicBlockBuilder, errorStringRValue );
          stringAdapter->llvmRelease( basicBlockBuilder, errorStringRValue );
          llvm::Constant *defaultLValue = m_memberAdapter->llvmDefaultLValue( basicBlockBuilder );
          basicBlockBuilder->CreateRet( defaultLValue );
        }
      }

      {
        std::string name = constructOverloadName( booleanAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "booleanLValue", booleanAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "arrayRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *booleanLValue = functionBuilder[0];
          llvm::Value *arrayRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *sizeLValue = basicBlockBuilder->CreateConstGEP2_32( arrayRValue, 0, 1 );
          llvm::Value *sizeRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, sizeLValue );
          llvm::Value *booleanRValue = basicBlockBuilder->CreateICmpNE( sizeRValue, llvm::ConstantInt::get( sizeRValue->getType(), 0 ) );
          booleanAdapter->llvmAssign( basicBlockBuilder, booleanLValue, booleanRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = constructOverloadName( stringAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "stringLValue", stringAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "arrayRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *stringLValue = functionBuilder[0];
          llvm::Value *arrayRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *arrayLValue = llvmRValueToLValue( basicBlockBuilder, arrayRValue );
          llvm::Value *stringRValue = stringAdapter->llvmCallCast( basicBlockBuilder, this, arrayLValue );
          stringAdapter->llvmAssign( basicBlockBuilder, stringLValue, stringRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = methodOverloadName( "size", this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "selfRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( sizeAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *selfRValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *sizeLValue = basicBlockBuilder->CreateConstGEP2_32( selfRValue, 0, 1 );
          llvm::Value *sizeRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, sizeLValue );
          basicBlockBuilder->CreateRet( sizeRValue );
        }
      }
      
      if ( m_memberAdapter->getImpl()->isShallow() )
      {
        {
          std::string name = methodOverloadName( "dataSize", this );
          std::vector< FunctionParam > params;
          params.push_back( FunctionParam( "selfRValue", this, USAGE_RVALUE ) );
          FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( sizeAdapter, USAGE_RVALUE ), params );
          if ( buildFunctions )
          {
            llvm::Value *selfRValue = functionBuilder[0];
            BasicBlockBuilder basicBlockBuilder( functionBuilder );
            basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
            llvm::Value *sizeLValue = basicBlockBuilder->CreateConstGEP2_32( selfRValue, 0, 1 );
            llvm::Value *sizeRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, sizeLValue );
            llvm::Value *memberSizeRValue = sizeAdapter->llvmConst( m_memberAdapter->getDesc()->getSize() );
            llvm::Value *dataSizeRValue = basicBlockBuilder->CreateMul( sizeRValue, memberSizeRValue );
            basicBlockBuilder->CreateRet( dataSizeRValue );
          }
        }
        
        {
          std::string name = methodOverloadName( "data", this );
          std::vector< FunctionParam > params;
          params.push_back( FunctionParam( "arrayRValue", this, USAGE_RVALUE ) );
          FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( dataAdapter, USAGE_RVALUE ), params );
          if ( buildFunctions )
          {
            llvm::Value *arrayRValue = functionBuilder[0];
            BasicBlockBuilder basicBlockBuilder( functionBuilder );
            basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
            llvm::Value *offsetLValue = basicBlockBuilder->CreateConstGEP2_32( arrayRValue, 0, 0 );
            llvm::Value *offsetRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, offsetLValue );
            llvm::Value *arrayLValue = llvmRValueToLValue( basicBlockBuilder, arrayRValue );
            llvm::Value *variableArrayLValue = basicBlockBuilder->CreateConstGEP2_32( arrayLValue, 0, 2 );
            llvm::Value *variableArrayRValue = m_variableArrayAdapter->llvmLValueToRValue( basicBlockBuilder, variableArrayLValue );
            llvm::Value *memberLValue = m_variableArrayAdapter->llvmConstIndexOp_NoCheckLValue( basicBlockBuilder, variableArrayRValue, offsetRValue );
            basicBlockBuilder->CreateRet( basicBlockBuilder->CreatePointerCast( memberLValue, dataAdapter->llvmRType() ) );
          }
        }
      }
    }
    
    void *SlicedArrayAdapter::llvmResolveExternalFunction( std::string const &functionName ) const
    {
      return 0;
    }

    llvm::Value *SlicedArrayAdapter::llvmConstIndexOp( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *arrayRValue, llvm::Value *indexRValue ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
      params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__ConstIndex", ExprType( m_memberAdapter, USAGE_RVALUE ), params, false, true );
      return basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), arrayRValue, indexRValue );
    }


    llvm::Value *SlicedArrayAdapter::llvmNonConstIndexOp( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *exprLValue, llvm::Value *indexRValue ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();

      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_LVALUE ) );
      params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__NonConstIndex", ExprType( m_memberAdapter, USAGE_LVALUE ), params, false, true );
      return basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), exprLValue, indexRValue );
    }
    
    void SlicedArrayAdapter::llvmInit( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *lValue ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();
      llvm::Value *offsetLValue = basicBlockBuilder->CreateConstGEP2_32( lValue, 0, 0 );
      sizeAdapter->llvmInit( basicBlockBuilder, offsetLValue );
      llvm::Value *sizeLValue = basicBlockBuilder->CreateConstGEP2_32( lValue, 0, 1 );
      sizeAdapter->llvmInit( basicBlockBuilder, sizeLValue );
      llvm::Value *variableArrayLValue = basicBlockBuilder->CreateConstGEP2_32( lValue, 0, 2 );
      m_variableArrayAdapter->llvmInit( basicBlockBuilder, variableArrayLValue );
    }
    
    void SlicedArrayAdapter::llvmRelease( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__Release", ExprType(), params, false );
      basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), rValue );
    }

    void SlicedArrayAdapter::llvmDefaultAssign( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const
    {
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "dstLValue", this, CG::USAGE_LVALUE ) );
      params.push_back( FunctionParam( "srcRValue", this, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__DefaultAssign", ExprType(), params, false );
      basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), dstLValue, srcRValue );
    }

    void SlicedArrayAdapter::llvmRetain( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__Retain", ExprType(), params, false );
      basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), rValue );
    }
    
    llvm::Constant *SlicedArrayAdapter::llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();
      std::vector<llvm::Constant *> elementDefaultRValues;
      elementDefaultRValues.push_back( sizeAdapter->llvmDefaultValue( basicBlockBuilder ) );
      elementDefaultRValues.push_back( sizeAdapter->llvmDefaultValue( basicBlockBuilder ) );
      elementDefaultRValues.push_back( m_variableArrayAdapter->llvmDefaultValue( basicBlockBuilder ) );
      return llvm::ConstantStruct::get( (llvm::StructType const *)llvmRawType(), elementDefaultRValues );
    }
  };
};
