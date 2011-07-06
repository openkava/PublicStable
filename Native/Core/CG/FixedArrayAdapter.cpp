#include "FixedArrayAdapter.h"
#include "BooleanAdapter.h"
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

#include <Fabric/Core/RT/FixedArrayDesc.h>

#include <llvm/Module.h>
#include <llvm/Function.h>

namespace Fabric
{
  namespace CG
  {
    FixedArrayAdapter::FixedArrayAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::FixedArrayDesc> const &fixedArrayDesc )
      : ArrayAdapter( manager, fixedArrayDesc, FL_PASS_BY_REFERENCE )
      , m_length( fixedArrayDesc->getNumMembers() )
      , m_memberAdapter( manager->getAdapter( fixedArrayDesc->getMemberDesc() ) )
    {
      setLLVMType( llvm::ArrayType::get( m_memberAdapter->llvmRawType(), m_length ) );
    }
    
    void FixedArrayAdapter::llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const
    {
      if ( moduleBuilder.contains( getCodeName() ) )
        return;
      
      moduleBuilder->addTypeName( getCodeName(), llvmRawType() );
      
      RC::ConstHandle<BooleanAdapter> booleanAdapter = getManager()->getBooleanAdapter();
      RC::ConstHandle<IntegerAdapter> integerAdapter = getManager()->getIntegerAdapter();
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

          llvm::Value *arrayRValue = functionBuilder[0];
        
          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          llvm::BasicBlock *checkBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "check" );
          llvm::BasicBlock *notDoneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "notDone" );
          llvm::BasicBlock *doneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "done" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *arrayLValue = llvmRValueToLValue( basicBlockBuilder, arrayRValue );
          llvm::Value *lengthRValue = sizeAdapter->llvmConst( m_length );
          llvm::Value *indexLValue = sizeAdapter->llvmAlloca( basicBlockBuilder, "index" );
          basicBlockBuilder->CreateStore( sizeAdapter->llvmConst( 0 ), indexLValue );
          basicBlockBuilder->CreateBr( checkBB );
          
          basicBlockBuilder->SetInsertPoint( checkBB );
          llvm::Value *indexRValue = basicBlockBuilder->CreateLoad( indexLValue );
          basicBlockBuilder->CreateCondBr( basicBlockBuilder->CreateICmpULT( indexRValue, lengthRValue ), notDoneBB, doneBB );
          
          basicBlockBuilder->SetInsertPoint( notDoneBB );
          indexRValue = basicBlockBuilder->CreateLoad( indexLValue );
          llvm::Value *arrayData = basicBlockBuilder->CreateConstGEP2_32( arrayLValue, 0, 0 );
          llvm::Value *memberLValue = basicBlockBuilder->CreateGEP( arrayData, indexRValue );
          llvm::Value *memberRValue = m_memberAdapter->llvmLValueToRValue( basicBlockBuilder, memberLValue );
          m_memberAdapter->llvmRetain( basicBlockBuilder, memberRValue );
          llvm::Value *newIndexRValue = basicBlockBuilder->CreateAdd( indexRValue, sizeAdapter->llvmConst( 1 ) );
          basicBlockBuilder->CreateStore( newIndexRValue, indexLValue );
          basicBlockBuilder->CreateBr( checkBB );
          
          basicBlockBuilder->SetInsertPoint( doneBB );
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
          llvm::BasicBlock *checkBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "check" );
          llvm::BasicBlock *notDoneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "notDone" );
          llvm::BasicBlock *doneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "done" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *lengthRValue = sizeAdapter->llvmConst( m_length );
          llvm::Value *indexLValue = sizeAdapter->llvmAlloca( basicBlockBuilder, "index" );
          basicBlockBuilder->CreateStore( sizeAdapter->llvmConst( 0 ), indexLValue );
          basicBlockBuilder->CreateBr( checkBB );
          
          basicBlockBuilder->SetInsertPoint( checkBB );
          llvm::Value *indexRValue = basicBlockBuilder->CreateLoad( indexLValue );
          basicBlockBuilder->CreateCondBr( basicBlockBuilder->CreateICmpULT( indexRValue, lengthRValue ), notDoneBB, doneBB );
          
          basicBlockBuilder->SetInsertPoint( notDoneBB );
          indexRValue = basicBlockBuilder->CreateLoad( indexLValue );
          llvm::Value *dstMemberData = basicBlockBuilder->CreateConstGEP2_32( dstLValue, 0, 0 );
          llvm::Value *dstMemberLValue = basicBlockBuilder->CreateGEP( dstMemberData, indexRValue );
          llvm::Value *srcMemberData = basicBlockBuilder->CreateConstGEP2_32( srcRValue, 0, 0 );
          llvm::Value *srcMemberLValue = basicBlockBuilder->CreateGEP( srcMemberData, indexRValue );
          llvm::Value *srcMemberRValue = m_memberAdapter->llvmLValueToRValue( basicBlockBuilder, srcMemberLValue );
          m_memberAdapter->llvmAssign( basicBlockBuilder, dstMemberLValue, srcMemberRValue );
          llvm::Value *newIndexRValue = basicBlockBuilder->CreateAdd( indexRValue, sizeAdapter->llvmConst( 1 ) );
          basicBlockBuilder->CreateStore( newIndexRValue, indexLValue );
          basicBlockBuilder->CreateBr( checkBB );
          
          basicBlockBuilder->SetInsertPoint( doneBB );
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

          llvm::Value *arrayRValue = functionBuilder[0];
        
          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          llvm::BasicBlock *checkBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "check" );
          llvm::BasicBlock *notDoneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "notDone" );
          llvm::BasicBlock *doneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "done" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *arrayLValue = llvmRValueToLValue( basicBlockBuilder, arrayRValue );
          llvm::Value *lengthRValue = sizeAdapter->llvmConst( m_length );
          llvm::Value *indexLValue = sizeAdapter->llvmAlloca( basicBlockBuilder, "index" );
          basicBlockBuilder->CreateStore( sizeAdapter->llvmConst(0), indexLValue );
          basicBlockBuilder->CreateBr( checkBB );
          
          basicBlockBuilder->SetInsertPoint( checkBB );
          llvm::Value *indexRValue = basicBlockBuilder->CreateLoad( indexLValue );
          basicBlockBuilder->CreateCondBr( basicBlockBuilder->CreateICmpULT( indexRValue, lengthRValue ), notDoneBB, doneBB );
          
          basicBlockBuilder->SetInsertPoint( notDoneBB );
          indexRValue = basicBlockBuilder->CreateLoad( indexLValue );
          llvm::Value *arrayData = basicBlockBuilder->CreateConstGEP2_32( arrayLValue, 0, 0 );
          llvm::Value *memberLValue = basicBlockBuilder->CreateGEP( arrayData, indexRValue );
          llvm::Value *memberRValue = m_memberAdapter->llvmLValueToRValue( basicBlockBuilder, memberLValue );
          m_memberAdapter->llvmRelease( basicBlockBuilder, memberRValue );
          llvm::Value *newIndexRValue = basicBlockBuilder->CreateAdd( indexRValue, sizeAdapter->llvmConst( 1 ) );
          basicBlockBuilder->CreateStore( newIndexRValue, indexLValue );
          basicBlockBuilder->CreateBr( checkBB );
          
          basicBlockBuilder->SetInsertPoint( doneBB );
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
          llvm::Value *lengthRValue = sizeAdapter->llvmConst( m_length );
          llvm::Value *inRangeCond = basicBlockBuilder->CreateICmpULT( indexRValue, lengthRValue );
          basicBlockBuilder->CreateCondBr( inRangeCond, inRangeBB, outOfRangeBB );
          
          basicBlockBuilder->SetInsertPoint( inRangeBB );
          llvm::Value *arrayLValue = llvmRValueToLValue( basicBlockBuilder, arrayRValue );
          llvm::Value *arrayData = basicBlockBuilder->CreateConstGEP2_32( arrayLValue, 0, 0 );
          llvm::Value *memberLValue = basicBlockBuilder->CreateGEP( arrayData, indexRValue );
          llvm::Value *memberRValue = m_memberAdapter->llvmLValueToRValue( basicBlockBuilder, memberLValue );
          m_memberAdapter->llvmRetain( basicBlockBuilder, memberRValue );
          basicBlockBuilder->CreateRet( memberRValue );
          
          basicBlockBuilder->SetInsertPoint( outOfRangeBB );
          std::string errorMsg = "KL: "+getUserName()+" index out-of-bounds";
          RC::ConstHandle<ConstStringAdapter> errorConstStringAdapter = getManager()->getConstStringAdapter( errorMsg.length() );
          ExprValue errorExprValue( errorConstStringAdapter, USAGE_RVALUE, errorConstStringAdapter->llvmConst( basicBlockBuilder, errorMsg ) );
          llvm::Value *errorStringRValue = stringAdapter->llvmCast( basicBlockBuilder, errorExprValue );
          stringAdapter->llvmReport( basicBlockBuilder, errorStringRValue );
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
          llvm::Value *lengthRValue = sizeAdapter->llvmConst( m_length );
          llvm::Value *inRangeCond = basicBlockBuilder->CreateICmpULT( indexRValue, lengthRValue );
          basicBlockBuilder->CreateCondBr( inRangeCond, inRangeBB, outOfRangeBB );
          
          basicBlockBuilder->SetInsertPoint( inRangeBB );
          llvm::Value *arrayData = basicBlockBuilder->CreateConstGEP2_32( arrayLValue, 0, 0 );
          llvm::Value *memberLValue = basicBlockBuilder->CreateGEP( arrayData, indexRValue );
          basicBlockBuilder->CreateRet( memberLValue );
          
          basicBlockBuilder->SetInsertPoint( outOfRangeBB );
          std::string errorMsg = "KL: "+getUserName()+" index out-of-bounds";
          RC::ConstHandle<ConstStringAdapter> errorConstStringAdapter = getManager()->getConstStringAdapter( errorMsg.length() );
          ExprValue errorExprValue( errorConstStringAdapter, USAGE_RVALUE, errorConstStringAdapter->llvmConst( basicBlockBuilder, errorMsg ) );
          llvm::Value *errorStringRValue = stringAdapter->llvmCast( basicBlockBuilder, errorExprValue );
          stringAdapter->llvmReport( basicBlockBuilder, errorStringRValue );
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
          llvm::Value *sizeRValue = llvm::ConstantInt::get( sizeAdapter->llvmRType(), m_length, false );
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
          llvm::Value *sizeRValue = llvm::ConstantInt::get( sizeAdapter->llvmRType(), m_length, false );
          basicBlockBuilder->CreateRet( sizeRValue );
        }
      }
      
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
          llvm::Value *sizeRValue = sizeAdapter->llvmConst( m_length );
          llvm::Value *memberSizeRValue = sizeAdapter->llvmConst( m_memberAdapter->getDesc()->getSize() );
          llvm::Value *dataSizeRValue = basicBlockBuilder->CreateMul( sizeRValue, memberSizeRValue );
          basicBlockBuilder->CreateRet( dataSizeRValue );
        }
      }
      
      {
        std::string name = methodOverloadName( "data", this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "selfRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( dataAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *selfRValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreatePointerCast( selfRValue, dataAdapter->llvmRType() ) );
        }
      }
    }
    
    void *FixedArrayAdapter::llvmResolveExternalFunction( std::string const &functionName ) const
    {
      return 0;
    }
    
    llvm::Constant *FixedArrayAdapter::llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      std::vector<llvm::Constant *> elementDefaultRValues;
      elementDefaultRValues.resize( m_length, m_memberAdapter->llvmDefaultValue( basicBlockBuilder ) );
      return llvm::ConstantArray::get( (llvm::ArrayType const *)llvmRawType(), elementDefaultRValues );
    }
    
    llvm::Constant *FixedArrayAdapter::llvmDefaultRValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      return llvmDefaultLValue( basicBlockBuilder );
    }

    llvm::Constant *FixedArrayAdapter::llvmDefaultLValue( BasicBlockBuilder &basicBlockBuilder ) const
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
    
    llvm::Value *FixedArrayAdapter::llvmConstIndexOp( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *arrayRValue, llvm::Value *indexRValue ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
      params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__ConstIndex", ExprType( m_memberAdapter, USAGE_RVALUE ), params, false, true );
      return basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), arrayRValue, indexRValue );
    }


    llvm::Value *FixedArrayAdapter::llvmNonConstIndexOp( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *arrayLValue, llvm::Value *indexRValue ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_LVALUE ) );
      params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__NonConstIndex", ExprType( m_memberAdapter, USAGE_LVALUE ), params, false, true );
      return basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), arrayLValue, indexRValue );
    }
    
    void FixedArrayAdapter::llvmInit( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *lValue ) const
    {
      
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();

      llvm::BasicBlock *checkBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "check" );
      llvm::BasicBlock *notDoneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "notDone" );
      llvm::BasicBlock *doneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "done" );
      
      llvm::Value *indexLValue = sizeAdapter->llvmAlloca( basicBlockBuilder, "index" );
      llvm::Value *lengthRValue = sizeAdapter->llvmConst( m_length );
      basicBlockBuilder->CreateStore( sizeAdapter->llvmConst( 0 ), indexLValue );
      basicBlockBuilder->CreateBr( checkBB );
      
      basicBlockBuilder->SetInsertPoint( checkBB );
      llvm::Value *indexRValue = basicBlockBuilder->CreateLoad( indexLValue );
      basicBlockBuilder->CreateCondBr( basicBlockBuilder->CreateICmpULT( indexRValue, lengthRValue ), notDoneBB, doneBB );
      
      basicBlockBuilder->SetInsertPoint( notDoneBB );
      indexRValue = basicBlockBuilder->CreateLoad( indexLValue );
      std::vector< llvm::Value * > gepIndices;
      gepIndices.push_back( sizeAdapter->llvmConst( 0 ) );
      gepIndices.push_back( indexRValue );
      llvm::Value *memberLValue = basicBlockBuilder->CreateGEP( lValue, gepIndices.begin(), gepIndices.end() );
      m_memberAdapter->llvmInit( basicBlockBuilder, memberLValue );
      llvm::Value *newIndexRValue = basicBlockBuilder->CreateAdd( indexRValue, sizeAdapter->llvmConst( 1 ) );
      basicBlockBuilder->CreateStore( newIndexRValue, indexLValue );
      basicBlockBuilder->CreateBr( checkBB );
      
      basicBlockBuilder->SetInsertPoint( doneBB );
    }
    
    void FixedArrayAdapter::llvmRelease( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__Release", ExprType(), params, false );
      basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), rValue );
    }
    
    void FixedArrayAdapter::llvmDefaultAssign( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const
    {
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "dstLValue", this, CG::USAGE_LVALUE ) );
      params.push_back( FunctionParam( "srcRValue", this, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__DefaultAssign", ExprType(), params, false );
      basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), dstLValue, srcRValue );
    }

    void FixedArrayAdapter::llvmRetain( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__Retain", ExprType(), params, false );
      basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), rValue );
    }
  };
};
