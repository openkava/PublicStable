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
      , m_fixedArrayDesc( fixedArrayDesc )
      , m_memberAdapter( manager->getAdapter( fixedArrayDesc->getMemberDesc() ) )
    {
    }
    
    llvm::Type const *FixedArrayAdapter::buildLLVMRawType( RC::Handle<Context> const &context ) const
    {
      return llvm::ArrayType::get( m_memberAdapter->llvmRawType( context ), m_length );
    }
    
    void FixedArrayAdapter::llvmCompileToModule( ModuleBuilder &moduleBuilder ) const
    {
      if ( moduleBuilder.haveCompiledToModule( getCodeName() ) )
        return;
        
      RC::Handle<Context> context = moduleBuilder.getContext();
      
      m_memberAdapter->llvmCompileToModule( moduleBuilder );
      RC::ConstHandle<BooleanAdapter> booleanAdapter = getManager()->getBooleanAdapter();
      booleanAdapter->llvmCompileToModule( moduleBuilder );
      RC::ConstHandle<IntegerAdapter> integerAdapter = getManager()->getIntegerAdapter();
      integerAdapter->llvmCompileToModule( moduleBuilder );
      RC::ConstHandle<SizeAdapter> sizeAdapter = getManager()->getSizeAdapter();
      sizeAdapter->llvmCompileToModule( moduleBuilder );
      RC::ConstHandle<StringAdapter> stringAdapter = getManager()->getStringAdapter();
      stringAdapter->llvmCompileToModule( moduleBuilder );
      RC::ConstHandle<OpaqueAdapter> dataAdapter = getManager()->getDataAdapter();
      dataAdapter->llvmCompileToModule( moduleBuilder );
      RC::ConstHandle<ConstStringAdapter> constStringAdapter = getManager()->getConstStringAdapter();
      constStringAdapter->llvmCompileToModule( moduleBuilder );

      moduleBuilder->addTypeName( getCodeName(), llvmRawType( context ) );
      
      static const bool buildFunctions = true;
      
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
          llvm::Value *lengthRValue = sizeAdapter->llvmConst( context, m_length );
          llvm::Value *indexLValue = sizeAdapter->llvmAlloca( basicBlockBuilder, "index" );
          basicBlockBuilder->CreateStore( sizeAdapter->llvmConst( context, 0 ), indexLValue );
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
          llvm::Value *newIndexRValue = basicBlockBuilder->CreateAdd( indexRValue, sizeAdapter->llvmConst( context, 1 ) );
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
          llvm::Value *lengthRValue = sizeAdapter->llvmConst( context, m_length );
          llvm::Value *indexLValue = sizeAdapter->llvmAlloca( basicBlockBuilder, "index" );
          basicBlockBuilder->CreateStore( sizeAdapter->llvmConst( context, 0 ), indexLValue );
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
          llvm::Value *newIndexRValue = basicBlockBuilder->CreateAdd( indexRValue, sizeAdapter->llvmConst( context, 1 ) );
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
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__Store", ExprType(), params, false );
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
          llvm::Value *lengthRValue = sizeAdapter->llvmConst( context, m_length );
          llvm::Value *indexLValue = sizeAdapter->llvmAlloca( basicBlockBuilder, "index" );
          basicBlockBuilder->CreateStore( sizeAdapter->llvmConst( context, 0 ), indexLValue );
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
          m_memberAdapter->llvmStore( basicBlockBuilder, dstMemberLValue, srcMemberRValue );
          llvm::Value *newIndexRValue = basicBlockBuilder->CreateAdd( indexRValue, sizeAdapter->llvmConst( context, 1 ) );
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
          llvm::Value *lengthRValue = sizeAdapter->llvmConst( context, m_length );
          llvm::Value *indexLValue = sizeAdapter->llvmAlloca( basicBlockBuilder, "index" );
          basicBlockBuilder->CreateStore( sizeAdapter->llvmConst( context, 0 ), indexLValue );
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
          llvm::Value *newIndexRValue = basicBlockBuilder->CreateAdd( indexRValue, sizeAdapter->llvmConst( context, 1 ) );
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
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__ConstIndex", ExprType( m_memberAdapter, USAGE_RVALUE ), params, false, 0, true );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *arrayRValue = functionBuilder[0];
          llvm::Value *indexRValue = functionBuilder[1];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          llvm::BasicBlock *inRangeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "inRange" );
          llvm::BasicBlock *outOfRangeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "outOfRange" );

          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *lengthRValue = sizeAdapter->llvmConst( context, m_length );
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
          ExprValue errorExprValue( constStringAdapter, USAGE_RVALUE, context, constStringAdapter->llvmConst( basicBlockBuilder, errorMsg ) );
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
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__NonConstIndex", ExprType( m_memberAdapter, USAGE_LVALUE ), params, false, 0, true );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *arrayLValue = functionBuilder[0];
          llvm::Value *indexRValue = functionBuilder[1];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          llvm::BasicBlock *inRangeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "inRange" );
          llvm::BasicBlock *outOfRangeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "outOfRange" );

          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *lengthRValue = sizeAdapter->llvmConst( context, m_length );
          llvm::Value *inRangeCond = basicBlockBuilder->CreateICmpULT( indexRValue, lengthRValue );
          basicBlockBuilder->CreateCondBr( inRangeCond, inRangeBB, outOfRangeBB );
          
          basicBlockBuilder->SetInsertPoint( inRangeBB );
          llvm::Value *arrayData = basicBlockBuilder->CreateConstGEP2_32( arrayLValue, 0, 0 );
          llvm::Value *memberLValue = basicBlockBuilder->CreateGEP( arrayData, indexRValue );
          basicBlockBuilder->CreateRet( memberLValue );
          
          basicBlockBuilder->SetInsertPoint( outOfRangeBB );
          std::string errorMsg = "KL: "+getUserName()+" index out-of-bounds";
          ExprValue errorExprValue( constStringAdapter, USAGE_RVALUE, context, constStringAdapter->llvmConst( basicBlockBuilder, errorMsg ) );
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
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *sizeRValue = llvm::ConstantInt::get( sizeAdapter->llvmRType( context ), m_length, false );
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
        params.push_back( FunctionParam( "thisRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( sizeAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *sizeRValue = llvm::ConstantInt::get( sizeAdapter->llvmRType( context ), m_length, false );
          basicBlockBuilder->CreateRet( sizeRValue );
        }
      }
      
      {
        std::string name = methodOverloadName( "dataSize", this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "thisRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( sizeAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *sizeRValue = sizeAdapter->llvmConst( context, m_length );
          llvm::Value *memberSizeRValue = sizeAdapter->llvmConst( context, m_memberAdapter->getDesc()->getAllocSize() );
          llvm::Value *dataSizeRValue = basicBlockBuilder->CreateMul( sizeRValue, memberSizeRValue );
          basicBlockBuilder->CreateRet( dataSizeRValue );
        }
      }
      
      {
        std::string name = methodOverloadName( "data", this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "thisRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( dataAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *thisRValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreatePointerCast( thisRValue, dataAdapter->llvmRType( context ) ) );
        }
      }
    }
    
    llvm::Constant *FixedArrayAdapter::llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      std::vector<llvm::Constant *> elementDefaultRValues;
      elementDefaultRValues.resize( m_length, m_memberAdapter->llvmDefaultValue( basicBlockBuilder ) );
      return llvm::ConstantArray::get( (llvm::ArrayType const *)llvmRawType( basicBlockBuilder.getContext() ), elementDefaultRValues );
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
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__ConstIndex", ExprType( m_memberAdapter, USAGE_RVALUE ), params, false, 0, true );
      return basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), arrayRValue, indexRValue );
    }


    llvm::Value *FixedArrayAdapter::llvmNonConstIndexOp( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *arrayLValue, llvm::Value *indexRValue ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_LVALUE ) );
      params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__NonConstIndex", ExprType( m_memberAdapter, USAGE_LVALUE ), params, false, 0, true );
      return basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), arrayLValue, indexRValue );
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
    
    void FixedArrayAdapter::llvmStore( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const
    {
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "dstLValue", this, CG::USAGE_LVALUE ) );
      params.push_back( FunctionParam( "srcRValue", this, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__Store", ExprType(), params, false );
      basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), dstLValue, srcRValue );
    }
  };
};
