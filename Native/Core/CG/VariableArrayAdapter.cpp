/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "VariableArrayAdapter.h"
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

#include <Fabric/Core/CG/CompileOptions.h>
#include <Fabric/Core/RT/VariableArrayDesc.h>
#include <Fabric/Core/RT/VariableArrayImpl.h>
#include <Fabric/Core/RT/Impl.h>

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Intrinsics.h>

namespace Fabric
{
  namespace CG
  {
    VariableArrayAdapter::VariableArrayAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::VariableArrayDesc> const &variableArrayDesc )
      : ArrayAdapter( manager, variableArrayDesc, FL_PASS_BY_REFERENCE )
      , m_variableArrayDesc( variableArrayDesc )
      , m_variableArrayImpl( variableArrayDesc->getImpl() )
      , m_memberAdapter( manager->getAdapter( variableArrayDesc->getMemberDesc() ) )
    {
    }
    
    llvm::Type const *VariableArrayAdapter::buildLLVMRawType( RC::Handle<Context> const &context ) const
    {
      llvm::Type const *llvmSizeTy = llvmSizeType( context );
      
      std::vector<llvm::Type const *> memberLLVMTypes;
      memberLLVMTypes.push_back( llvmSizeTy ); // allocNumMembers
      memberLLVMTypes.push_back( llvmSizeTy ); // numMembers
      memberLLVMTypes.push_back( m_memberAdapter->llvmRawType( context )->getPointerTo() );
      return llvm::StructType::get( context->getLLVMContext(), memberLLVMTypes, true );
    }
    
    void VariableArrayAdapter::llvmCompileToModule( ModuleBuilder &moduleBuilder ) const
    {
      if ( moduleBuilder.haveCompiledToModule( getCodeName() ) )
        return;
      
      RC::Handle<Context> context = moduleBuilder.getContext();
      
      llvm::Type const *implType = llvmRawType( context );

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
      
      moduleBuilder->addTypeName( getCodeName(), implType );
      
      static const bool buildFunctions = true;
      bool const guarded = moduleBuilder.getCompileOptions()->getGuarded();
      
      {
        std::string name = methodOverloadName( "size", this );
        std::vector<FunctionParam> params;
        params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( sizeAdapter, USAGE_RVALUE ), params, false );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          
          llvm::Value *arrayRValue = functionBuilder[0];

          llvm::BasicBlock *entryBB = functionBuilder.createBasicBlock( "entry" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *arrayLValue = llvmRValueToLValue( basicBlockBuilder, arrayRValue );
          llvm::Value *sizeLValue = basicBlockBuilder->CreateStructGEP( arrayLValue, SizeIndex );
          llvm::Value *sizeRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, sizeLValue );
          basicBlockBuilder->CreateRet( sizeRValue );
        }
      }
      
      {
        std::vector<FunctionParam> params;
        params.push_back( FunctionParam( "dstLValue", this, CG::USAGE_LVALUE ) );
        params.push_back( FunctionParam( "srcRValue", this, CG::USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__DefaultAssign", ExprType(), params, false, 0, true );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *dstLValue = functionBuilder[0];
          llvm::Value *srcRValue = functionBuilder[1];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          llvm::BasicBlock *disposeCheckBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "disposeCheckBB" );
          llvm::BasicBlock *disposeNextBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "disposeNextBB" );
          llvm::BasicBlock *disposeDoneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "disposeDoneBB" );
          llvm::BasicBlock *zeroBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "zeroBB" );
          llvm::BasicBlock *nonZeroBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "nonZeroBB" );
          llvm::BasicBlock *freeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "freeBB" );
          llvm::BasicBlock *reallocBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "reallocBB" );
          llvm::BasicBlock *mallocBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "mallocBB" );
          llvm::BasicBlock *allocDoneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "allocDoneBB" );
          llvm::BasicBlock *copyCheckBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "copyCheckBB" );
          llvm::BasicBlock *copyNextBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "copyNextBB" );
          llvm::BasicBlock *copyDoneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "copyDoneBB" );
          llvm::BasicBlock *doneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "doneBB" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *srcLValue = llvmRValueToLValue( basicBlockBuilder, srcRValue );
          llvm::Value *dstSizeLValue = basicBlockBuilder->CreateStructGEP( dstLValue, SizeIndex );
          llvm::Value *dstSizeRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, dstSizeLValue );
          llvm::Value *srcSizeLValue = basicBlockBuilder->CreateStructGEP( srcLValue, SizeIndex );
          llvm::Value *srcSizeRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, srcSizeLValue );
          llvm::Value *dstAllocSizeLValue = basicBlockBuilder->CreateStructGEP( dstLValue, AllocSizeIndex );
          llvm::Value *dstMemberDatasLValue = basicBlockBuilder->CreateStructGEP( dstLValue, MemberDatasIndex );
          llvm::Value *dstMemberDatasRValue = basicBlockBuilder->CreateLoad( dstMemberDatasLValue );
          llvm::Value *indexLValue = sizeAdapter->llvmAlloca( basicBlockBuilder, "indexLValue" );
          sizeAdapter->llvmDefaultAssign( basicBlockBuilder, indexLValue, srcSizeRValue );
          basicBlockBuilder->CreateBr( disposeCheckBB );
          
          basicBlockBuilder->SetInsertPoint( disposeCheckBB );
          llvm::Value *indexRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, indexLValue );
          basicBlockBuilder->CreateCondBr(
            basicBlockBuilder->CreateICmpULT( indexRValue, dstSizeRValue ),
            disposeNextBB,
            disposeDoneBB
            );
          
          basicBlockBuilder->SetInsertPoint( disposeNextBB );
          llvm::Value *dstMemberLValue = basicBlockBuilder->CreateGEP( dstMemberDatasRValue, indexRValue );
          m_memberAdapter->llvmDispose( basicBlockBuilder, dstMemberLValue );
          sizeAdapter->llvmDefaultAssign(
            basicBlockBuilder,
            indexLValue,
            basicBlockBuilder->CreateAdd(
              indexRValue,
              sizeAdapter->llvmConst( context, 1 )
              )
            );
          basicBlockBuilder->CreateBr( disposeCheckBB );
          
          basicBlockBuilder->SetInsertPoint( disposeDoneBB );
          sizeAdapter->llvmDefaultAssign( basicBlockBuilder, dstSizeLValue, srcSizeRValue );
          basicBlockBuilder->CreateCondBr(
            basicBlockBuilder->CreateICmpEQ( srcSizeRValue, sizeAdapter->llvmConst( context, 0 ) ),
            zeroBB,
            nonZeroBB
            );
            
          basicBlockBuilder->SetInsertPoint( zeroBB );
          sizeAdapter->llvmDefaultAssign( basicBlockBuilder, dstAllocSizeLValue, sizeAdapter->llvmConst( context, 0 ) );
          basicBlockBuilder->CreateCondBr(
            basicBlockBuilder->CreateIsNotNull( dstMemberDatasRValue ),
            freeBB,
            doneBB
            );
           
          basicBlockBuilder->SetInsertPoint( freeBB );
          llvmCallFree(
            basicBlockBuilder,
            dstMemberDatasRValue
            );
          basicBlockBuilder->CreateStore(
            llvm::ConstantPointerNull::get( static_cast<llvm::PointerType const *>( dstMemberDatasRValue->getType() ) ), dstMemberDatasLValue );
          basicBlockBuilder->CreateBr( doneBB );
           
          basicBlockBuilder->SetInsertPoint( nonZeroBB );
          llvm::Value *dstAllocSizeRValue = llvmCallMax(
            basicBlockBuilder,
            llvmCallNextPowTwoMinusOne(
              basicBlockBuilder,
              srcSizeRValue
              ),
            sizeAdapter->llvmConst( context, 15 )
            );
          sizeAdapter->llvmDefaultAssign( basicBlockBuilder, dstAllocSizeLValue, dstAllocSizeRValue );
          llvm::Value *allocSizeRValue = basicBlockBuilder->CreateMul(
            dstAllocSizeRValue, 
            sizeAdapter->llvmConst( context, m_memberAdapter->getDesc()->getAllocSize() )
            );
          basicBlockBuilder->CreateCondBr(
            basicBlockBuilder->CreateIsNotNull( dstMemberDatasRValue ),
            reallocBB,
            mallocBB
            );
          
          basicBlockBuilder->SetInsertPoint( reallocBB );
          llvm::Value *reallocDstMemberDatasRValue = basicBlockBuilder->CreatePointerCast(
            llvmCallRealloc(
              basicBlockBuilder,
              basicBlockBuilder->CreatePointerCast(
                dstMemberDatasRValue,
                basicBlockBuilder->getInt8PtrTy()
                ),
              allocSizeRValue
              ),
            dstMemberDatasRValue->getType()
            );
          basicBlockBuilder->CreateStore( dstMemberDatasRValue, dstMemberDatasLValue );
          basicBlockBuilder->CreateBr( allocDoneBB );
          
          basicBlockBuilder->SetInsertPoint( mallocBB );
          llvm::Value *mallocDstMemberDatasRValue = basicBlockBuilder->CreatePointerCast(
            llvmCallMalloc(
              basicBlockBuilder,
              allocSizeRValue
              ),
            dstMemberDatasRValue->getType()
            );
          basicBlockBuilder->CreateBr( allocDoneBB );
          
          basicBlockBuilder->SetInsertPoint( allocDoneBB );
          llvm::PHINode *dstMemberDatasPHIRValue = basicBlockBuilder->CreatePHI( reallocDstMemberDatasRValue->getType() );
          dstMemberDatasPHIRValue->addIncoming( reallocDstMemberDatasRValue, reallocBB );
          dstMemberDatasPHIRValue->addIncoming( mallocDstMemberDatasRValue, mallocBB );
          dstMemberDatasRValue = dstMemberDatasPHIRValue;
          basicBlockBuilder->CreateStore( dstMemberDatasRValue, dstMemberDatasLValue );
          sizeAdapter->llvmDefaultAssign( basicBlockBuilder, indexLValue, sizeAdapter->llvmConst( context, 0 ) );
          llvm::Value *srcMemberDatasLValue = basicBlockBuilder->CreateStructGEP( srcLValue, MemberDatasIndex );
          llvm::Value *srcMemberDatasRValue = basicBlockBuilder->CreateLoad( srcMemberDatasLValue );
          basicBlockBuilder->CreateBr( copyCheckBB );
          
          basicBlockBuilder->SetInsertPoint( copyCheckBB );
          indexRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, indexLValue );
          basicBlockBuilder->CreateCondBr(
            basicBlockBuilder->CreateICmpULT( indexRValue, srcSizeRValue ),
            copyNextBB,
            copyDoneBB
            );
          
          basicBlockBuilder->SetInsertPoint( copyNextBB );
          llvm::Value *srcMemberLValue = basicBlockBuilder->CreateGEP( srcMemberDatasRValue, indexRValue );
          llvm::Value *srcMemberRValue = m_memberAdapter->llvmLValueToRValue( basicBlockBuilder, srcMemberLValue );
          dstMemberLValue = basicBlockBuilder->CreateGEP( dstMemberDatasRValue, indexRValue );
          m_memberAdapter->llvmInit( basicBlockBuilder, dstMemberLValue );
          m_memberAdapter->llvmDefaultAssign( basicBlockBuilder, dstMemberLValue, srcMemberRValue );
          sizeAdapter->llvmDefaultAssign(
            basicBlockBuilder,
            indexLValue,
            basicBlockBuilder->CreateAdd(
              indexRValue,
              sizeAdapter->llvmConst( context, 1 )
              )
            );
          basicBlockBuilder->CreateBr( copyCheckBB );
          
          basicBlockBuilder->SetInsertPoint( copyDoneBB );
          basicBlockBuilder->CreateBr( doneBB );
          
          basicBlockBuilder->SetInsertPoint( doneBB );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
        params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
        if ( guarded )
          params.push_back( FunctionParam( "errorDesc", constStringAdapter, CG::USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__ConstIndex", ExprType( m_memberAdapter, USAGE_RVALUE ), params, false, 0, true );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *arrayRValue = functionBuilder[0];
          llvm::Value *indexRValue = functionBuilder[1];
          llvm::Value *errorDescRValue;
          if ( guarded )
            errorDescRValue = functionBuilder[2];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          llvm::BasicBlock *inRangeBB;
          llvm::BasicBlock *outOfRangeBB;
          if ( guarded )
          {
            inRangeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "inRange" );
            outOfRangeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "outOfRange" );
          }

          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *sizeRValue;
          if ( guarded )
          {
            sizeRValue = llvmCallSize( basicBlockBuilder, arrayRValue );
            llvm::Value *inRangeCond = basicBlockBuilder->CreateICmpULT( indexRValue, sizeRValue );
            basicBlockBuilder->CreateCondBr( inRangeCond, inRangeBB, outOfRangeBB );
            
            basicBlockBuilder->SetInsertPoint( inRangeBB );
          }
          basicBlockBuilder->CreateRet( llvmConstIndexOp_NoCheck( basicBlockBuilder, arrayRValue, indexRValue ) );
          
          if ( guarded )
          {
            basicBlockBuilder->SetInsertPoint( outOfRangeBB );
            llvmThrowOutOfRangeException(
              basicBlockBuilder,
              "index",
              constStringAdapter,
              stringAdapter,
              sizeAdapter,
              indexRValue,
              sizeRValue,
              errorDescRValue
              );
            llvm::Value *defaultRValue = m_memberAdapter->llvmDefaultRValue( basicBlockBuilder );
            basicBlockBuilder->CreateRet( defaultRValue );
          }
        }
      }

      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "array", this, CG::USAGE_LVALUE ) );
        params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
        if ( guarded )
          params.push_back( FunctionParam( "errorDesc", constStringAdapter, CG::USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__NonConstIndex", ExprType( m_memberAdapter, USAGE_LVALUE ), params, false, 0, true );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *arrayLValue = functionBuilder[0];
          llvm::Value *indexRValue = functionBuilder[1];
          llvm::Value *errorDescRValue;
          if ( guarded )
            errorDescRValue = functionBuilder[2];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          llvm::BasicBlock *inRangeBB;
          llvm::BasicBlock *outOfRangeBB;
          if ( guarded )
          {
            inRangeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "inRange" );
            outOfRangeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "outOfRange" );
          }

          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *sizeRValue;
          if ( guarded )
          {
            llvm::Value *arrayRValue = llvmLValueToRValue( basicBlockBuilder, arrayLValue );
            sizeRValue = llvmCallSize( basicBlockBuilder, arrayRValue );
            llvm::Value *inRangeCond = basicBlockBuilder->CreateICmpULT( indexRValue, sizeRValue );
            basicBlockBuilder->CreateCondBr( inRangeCond, inRangeBB, outOfRangeBB );
            
            basicBlockBuilder->SetInsertPoint( inRangeBB );
          }
          basicBlockBuilder->CreateRet( llvmNonConstIndexOp_NoCheck( basicBlockBuilder, arrayLValue, indexRValue ) );
          
          if ( guarded )
          {
            basicBlockBuilder->SetInsertPoint( outOfRangeBB );
            llvmThrowOutOfRangeException(
              basicBlockBuilder,
              "index",
              constStringAdapter,
              stringAdapter,
              sizeAdapter,
              indexRValue,
              sizeRValue,
              errorDescRValue
              );
            llvm::Constant *defaultLValue = m_memberAdapter->llvmDefaultLValue( basicBlockBuilder );
            basicBlockBuilder->CreateRet( defaultLValue );
          }
        }
      }
      
      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
        params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__ConstIndexNoCheck", ExprType( m_memberAdapter, USAGE_RVALUE ), params, false, 0, true );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *arrayRValue = functionBuilder[0];
          llvm::Value *indexRValue = functionBuilder[1];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );

          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *memberLValue = llvmNonConstIndexOp_NoCheck( basicBlockBuilder, arrayRValue, indexRValue );
          llvm::Value *memberRValue = m_memberAdapter->llvmLValueToRValue( basicBlockBuilder, memberLValue );
          basicBlockBuilder->CreateRet( memberRValue );
        }
      }

      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "array", this, CG::USAGE_LVALUE ) );
        params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__NonConstIndexNoCheck", ExprType( m_memberAdapter, USAGE_LVALUE ), params, false, 0, true );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *arrayLValue = functionBuilder[0];
          llvm::Value *indexRValue = functionBuilder[1];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );

          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *memberDatasLValue = basicBlockBuilder->CreateStructGEP( arrayLValue, MemberDatasIndex );
          llvm::Value *memberDatasRValue = basicBlockBuilder->CreateLoad( memberDatasLValue );
          llvm::Value *memberLValue = basicBlockBuilder->CreateGEP( memberDatasRValue, indexRValue );
          basicBlockBuilder->CreateRet( memberLValue );
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
          llvm::Value *sizeRValue = llvmCallSize( basicBlockBuilder, arrayRValue );
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
          stringAdapter->llvmCallCast( basicBlockBuilder, this, arrayLValue, stringLValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      llvm::Function *assignAddFunction;
      {
        std::vector< llvm::Type const * > argTypes;
        argTypes.push_back( llvm::Type::getInt8PtrTy( context->getLLVMContext() ) );
        argTypes.push_back( llvmLType( context ) );
        argTypes.push_back( llvmRType( context ) );
        llvm::FunctionType const *funcType = llvm::FunctionType::get( llvm::Type::getVoidTy( context->getLLVMContext() ), argTypes, false );
        llvm::Constant *func = moduleBuilder->getOrInsertFunction( "__"+getCodeName()+"__Append", funcType ); 

        std::string name = methodOverloadName( assignOpMethodName( ASSIGN_OP_ADD ), this, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "lhsLValue", this, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "rhsRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *lhsLValue = functionBuilder[0];
          llvm::Value *rhsRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          std::vector< llvm::Value * > args;
          args.push_back( llvmAdapterPtr( basicBlockBuilder ) );
          args.push_back( lhsLValue );
          args.push_back( rhsRValue );
          basicBlockBuilder->CreateCall( func, args.begin(), args.end() );
          basicBlockBuilder->CreateRetVoid();
        }
        assignAddFunction = functionBuilder.getLLVMFunction();
      }
      
      {
        std::string name = binOpOverloadName( BIN_OP_ADD, this, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "lhsRValue", this, USAGE_RVALUE ) );
        params.push_back( FunctionParam( "rhsRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *lhsRValue = functionBuilder[0];
          llvm::Value *rhsRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *resultLValue = functionBuilder.getScope().llvmGetReturnLValue();
          llvmAssign( basicBlockBuilder, resultLValue, lhsRValue );
          basicBlockBuilder->CreateCall2( assignAddFunction, resultLValue, rhsRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = methodOverloadName( "swap", this, sizeAdapter, sizeAdapter );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "thisLValue", this, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "lhsRValue", sizeAdapter, USAGE_RVALUE ) );
        params.push_back( FunctionParam( "rhsRValue", sizeAdapter, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *thisLValue = functionBuilder[0];
          llvm::Value *lhsRValue = functionBuilder[1];
          llvm::Value *rhsRValue = functionBuilder[2];

          llvm::BasicBlock *entryBB = functionBuilder.createBasicBlock( "entry" );
          llvm::BasicBlock *unequalBB = functionBuilder.createBasicBlock( "unequal" );
          llvm::BasicBlock *doneBB = functionBuilder.createBasicBlock( "done" );

          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( entryBB );
          basicBlockBuilder->CreateCondBr(
            basicBlockBuilder->CreateICmpEQ( lhsRValue, rhsRValue ),
            doneBB,
            unequalBB
            );

          basicBlockBuilder->SetInsertPoint( unequalBB );
          llvm::Value *tmpLValue = m_memberAdapter->llvmAlloca( basicBlockBuilder, "tmp" );
          m_memberAdapter->llvmInit( basicBlockBuilder, tmpLValue );
          llvm::Value *lhsMemberLValue = llvmNonConstIndexOp( basicBlockBuilder, thisLValue, lhsRValue, 0 );
          llvm::Value *lhsMemberRValue = m_memberAdapter->llvmLValueToRValue( basicBlockBuilder, lhsMemberLValue );
          llvm::Value *rhsMemberLValue = llvmNonConstIndexOp( basicBlockBuilder, thisLValue, rhsRValue, 0 );
          llvm::Value *rhsMemberRValue = m_memberAdapter->llvmLValueToRValue( basicBlockBuilder, rhsMemberLValue );
          m_memberAdapter->llvmDefaultAssign( basicBlockBuilder, tmpLValue, lhsMemberRValue );
          m_memberAdapter->llvmDefaultAssign( basicBlockBuilder, lhsMemberLValue, rhsMemberRValue );
          m_memberAdapter->llvmDefaultAssign( basicBlockBuilder, rhsMemberLValue, m_memberAdapter->llvmLValueToRValue( basicBlockBuilder, tmpLValue ) );
          m_memberAdapter->llvmDispose( basicBlockBuilder, tmpLValue );
          basicBlockBuilder->CreateBr( doneBB );

          basicBlockBuilder->SetInsertPoint( doneBB );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = methodOverloadName( "push", this, m_memberAdapter );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "thisLValue", this, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "memberRValue", m_memberAdapter, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *thisLValue = functionBuilder[0];
          llvm::Value *memberRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *thisRValue = llvmLValueToRValue( basicBlockBuilder, thisLValue );
          llvm::Value *oldSize = llvmCallSize( basicBlockBuilder, thisRValue );
          llvm::Value *newSize = basicBlockBuilder->CreateAdd( oldSize, sizeAdapter->llvmConst( context, 1 ) );
          llvmCallResize( basicBlockBuilder, thisLValue, newSize );
          llvm::Value *newElementLValue = llvmNonConstIndexOp( basicBlockBuilder, thisLValue, oldSize, 0 );
          m_memberAdapter->llvmAssign( basicBlockBuilder, newElementLValue, memberRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = methodOverloadName( "pop", this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "thisLValue", this, USAGE_LVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( m_memberAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          ReturnInfo const &returnInfo = functionBuilder.getScope().getReturnInfo();
          llvm::Value *thisLValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
        
          llvm::Value *returnLValue;
          if( !returnInfo.usesReturnLValue() )
          {
            returnLValue = m_memberAdapter->llvmAlloca( basicBlockBuilder, "result" );
            m_memberAdapter->llvmInit( basicBlockBuilder, returnLValue );
          }
          else
          {
            returnLValue = returnInfo.getReturnLValue();
          }
          llvmCallPop( basicBlockBuilder, thisLValue, returnLValue );
          
          if( returnInfo.usesReturnLValue() )
          {
            basicBlockBuilder->CreateRetVoid( );
          }
          else
          {
            llvm::Value *returnRValue = m_memberAdapter->llvmLValueToRValue( basicBlockBuilder, returnLValue );
            basicBlockBuilder->CreateRet( returnRValue );
          }
        }
      }
      
      {
        std::string name = methodOverloadName( "resize", this, sizeAdapter );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "thisLValue", this, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "newSizeRValue", sizeAdapter, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *thisLValue = functionBuilder[0];
          llvm::Value *newSizeRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvmCallResize( basicBlockBuilder, thisLValue, newSizeRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        // [pzion 20110205] FIXME this version should be removed once we have automatic casting
        std::string name = methodOverloadName( "resize", this, integerAdapter );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "thisLValue", this, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "integerRValue", integerAdapter, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *thisLValue = functionBuilder[0];
          llvm::Value *integerRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *newSizeRValue = sizeAdapter->llvmCast( basicBlockBuilder, ExprValue( integerAdapter, USAGE_RVALUE, context, integerRValue ) );
          llvmCallResize( basicBlockBuilder, thisLValue, newSizeRValue );
           basicBlockBuilder->CreateRetVoid();
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
            llvm::Value *sizeRValue = llvmCallSize( basicBlockBuilder, selfRValue );
            llvm::Value *memberSizeRValue = sizeAdapter->llvmConst( context, m_memberAdapter->getDesc()->getAllocSize() );
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
            llvm::Value *selfLValue = llvmRValueToLValue( basicBlockBuilder, selfRValue );
            llvm::Value *memberDatasLValue = basicBlockBuilder->CreateStructGEP( selfLValue, MemberDatasIndex );
            llvm::Value *memberDatasRValue = basicBlockBuilder->CreateLoad( memberDatasLValue );
            basicBlockBuilder->CreateRet(
              basicBlockBuilder->CreatePointerCast( memberDatasRValue, dataAdapter->llvmRType( context ) )
              );
          }
        }
      }
    }
    
    void *VariableArrayAdapter::llvmResolveExternalFunction( std::string const &functionName ) const
    {
      if ( functionName == "__"+getCodeName()+"__Pop" )
        return (void *)&VariableArrayAdapter::Pop;
      else if ( functionName == "__"+getCodeName()+"__Append" )
        return (void *)&VariableArrayAdapter::Append;
      else if ( functionName == "__"+getCodeName()+"__Resize" )
        return (void *)&VariableArrayAdapter::Resize;
      else return ArrayAdapter::llvmResolveExternalFunction( functionName );
    }

    llvm::Value *VariableArrayAdapter::llvmConstIndexOp(
      CG::BasicBlockBuilder &basicBlockBuilder,
      llvm::Value *arrayRValue,
      llvm::Value *indexRValue,
      CG::Location const *location
      ) const
    {
      bool const guarded = basicBlockBuilder.getModuleBuilder().getCompileOptions()->getGuarded();
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();
      RC::ConstHandle<ConstStringAdapter> constStringAdapter = basicBlockBuilder.getManager()->getConstStringAdapter();
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
      params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
      if ( guarded )
        params.push_back( FunctionParam( "errorDesc", constStringAdapter, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__ConstIndex", ExprType( m_memberAdapter, USAGE_RVALUE ), params, false, 0, true );
      std::vector<llvm::Value *> args;
      args.push_back( arrayRValue );
      args.push_back( indexRValue );
      if ( guarded )
        args.push_back( llvmLocationConstStringRValue( basicBlockBuilder, constStringAdapter, location ) );
      return basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), args.begin(), args.end() );
    }

    llvm::Value *VariableArrayAdapter::llvmNonConstIndexOp(
      CG::BasicBlockBuilder &basicBlockBuilder,
      llvm::Value *exprLValue,
      llvm::Value *indexRValue,
      CG::Location const *location
      ) const
    {
      bool const guarded = basicBlockBuilder.getModuleBuilder().getCompileOptions()->getGuarded();
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();
      RC::ConstHandle<ConstStringAdapter> constStringAdapter = basicBlockBuilder.getManager()->getConstStringAdapter();
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_LVALUE ) );
      params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
      if ( guarded )
        params.push_back( FunctionParam( "errorDesc", constStringAdapter, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__NonConstIndex", ExprType( m_memberAdapter, USAGE_LVALUE ), params, false, 0, true );
      std::vector<llvm::Value *> args;
      args.push_back( exprLValue );
      args.push_back( indexRValue );
      if ( guarded )
        args.push_back( llvmLocationConstStringRValue( basicBlockBuilder, constStringAdapter, location ) );
      return basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), args.begin(), args.end() );
    }

    llvm::Value *VariableArrayAdapter::llvmConstIndexOp_NoCheck( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *arrayRValue, llvm::Value *indexRValue ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
      params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__ConstIndexNoCheck", ExprType( m_memberAdapter, USAGE_RVALUE ), params, false, 0, true );
      return basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), arrayRValue, indexRValue );
    }

    llvm::Value *VariableArrayAdapter::llvmNonConstIndexOp_NoCheck( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *exprLValue, llvm::Value *indexRValue ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();

      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_LVALUE ) );
      params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__NonConstIndexNoCheck", ExprType( m_memberAdapter, USAGE_LVALUE ), params, false, 0, true );
      return basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), exprLValue, indexRValue );
    }
    
    void VariableArrayAdapter::Append( VariableArrayAdapter const *inst, void *dstLValue, void const *srcRValue )
    {
      inst->m_variableArrayDesc->append( dstLValue, srcRValue );
    }
    
    void VariableArrayAdapter::Pop( VariableArrayAdapter const *inst, void *dst, void *result )
    {
      inst->m_variableArrayDesc->pop( dst, result );
    }
    
    void VariableArrayAdapter::Resize( VariableArrayAdapter const *inst, void *dst, size_t newSize )
    {
      inst->m_variableArrayDesc->setNumMembers( dst, newSize );
    }

    void VariableArrayAdapter::llvmCallResize( BasicBlockBuilder &basicBlockBuilder, llvm::Value *arrayLValue, llvm::Value *newSize ) const
    {
      RC::Handle<Context> context = basicBlockBuilder.getContext();
      
      RC::ConstHandle<SizeAdapter> sizeAdapter = getManager()->getSizeAdapter();
      
      std::vector< llvm::Type const * > argTypes;
      argTypes.push_back( basicBlockBuilder->getInt8PtrTy() );
      argTypes.push_back( llvmLType( context ) );
      argTypes.push_back( sizeAdapter->llvmRType( context ) );
      llvm::FunctionType const *funcType = llvm::FunctionType::get( basicBlockBuilder->getVoidTy(), argTypes, false );
      
      llvm::AttributeWithIndex AWI[1];
      AWI[0] = llvm::AttributeWithIndex::get( ~0u, llvm::Attribute::InlineHint | llvm::Attribute::NoUnwind );
      llvm::AttrListPtr attrListPtr = llvm::AttrListPtr::get( AWI, 1 );
      
      llvm::Function *func = llvm::cast<llvm::Function>( basicBlockBuilder.getModuleBuilder()->getOrInsertFunction( "__"+getCodeName()+"__Resize", funcType, attrListPtr ) ); 

      std::vector< llvm::Value * > args;
      args.push_back( llvmAdapterPtr( basicBlockBuilder ) );
      args.push_back( arrayLValue );
      args.push_back( newSize );
      basicBlockBuilder->CreateCall( func, args.begin(), args.end() );
    }

    void VariableArrayAdapter::llvmCallPop( BasicBlockBuilder &basicBlockBuilder, llvm::Value *arrayLValue, llvm::Value *memberLValue ) const
    {
      RC::Handle<Context> context = basicBlockBuilder.getContext();
      
      std::vector< llvm::Type const * > argTypes;
      argTypes.push_back( basicBlockBuilder->getInt8PtrTy() );
      argTypes.push_back( llvmLType( context ) );
      argTypes.push_back( m_memberAdapter->llvmLType( context ) );
      llvm::FunctionType const *funcType = llvm::FunctionType::get( basicBlockBuilder->getVoidTy(), argTypes, false );
      
      llvm::AttributeWithIndex AWI[4];
      AWI[0] = llvm::AttributeWithIndex::get( 1, llvm::Attribute::NoCapture );
      AWI[1] = llvm::AttributeWithIndex::get( 2, llvm::Attribute::NoCapture );
      AWI[2] = llvm::AttributeWithIndex::get( 3, llvm::Attribute::NoCapture );
      AWI[3] = llvm::AttributeWithIndex::get( ~0u, llvm::Attribute::InlineHint | llvm::Attribute::NoUnwind );
      llvm::AttrListPtr attrListPtr = llvm::AttrListPtr::get( AWI, 4 );
      
      llvm::Function *func = llvm::cast<llvm::Function>( basicBlockBuilder.getModuleBuilder()->getOrInsertFunction( "__"+getCodeName()+"__Pop", funcType, attrListPtr ) ); 

      std::vector< llvm::Value * > args;
      args.push_back( llvmAdapterPtr( basicBlockBuilder ) );
      args.push_back( arrayLValue );
      args.push_back( memberLValue );
      basicBlockBuilder->CreateCall( func, args.begin(), args.end() );
    }
    
    llvm::Value *VariableArrayAdapter::llvmCallSize( BasicBlockBuilder &basicBlockBuilder, llvm::Value *arrayRValue ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();
      std::string name = methodOverloadName( "size", this );
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), name, ExprType( sizeAdapter, USAGE_RVALUE ), params, false );
      return basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), arrayRValue );
    }

    void VariableArrayAdapter::llvmDisposeImpl( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *lValue ) const
    {
      RC::Handle<Context> context = basicBlockBuilder.getContext();
      RC::ConstHandle<SizeAdapter> sizeAdapter = getManager()->getSizeAdapter();
      
      CG::FunctionBuilder &functionBuilder = basicBlockBuilder.getFunctionBuilder();
      llvm::BasicBlock *checkBB = functionBuilder.createBasicBlock( "vaDisposeCheck" );
      llvm::BasicBlock *nextBB = functionBuilder.createBasicBlock( "vaDisposeNext" );
      llvm::BasicBlock *doneBB = functionBuilder.createBasicBlock( "vaDisposeDone" );

      llvm::Value *sizeLValue = basicBlockBuilder->CreateStructGEP( lValue, SizeIndex );
      llvm::Value *sizeRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, sizeLValue );
      llvm::Value *memberDatasLValue = basicBlockBuilder->CreateStructGEP( lValue, MemberDatasIndex );
      llvm::Value *memberDatasRValue = basicBlockBuilder->CreateLoad( memberDatasLValue );
      llvm::Value *indexLValue = sizeAdapter->llvmAlloca( basicBlockBuilder, "index" );
      sizeAdapter->llvmInit( basicBlockBuilder, indexLValue );
      basicBlockBuilder->CreateBr( checkBB );
      
      basicBlockBuilder->SetInsertPoint( checkBB );
      llvm::Value *indexRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, indexLValue );
      basicBlockBuilder->CreateCondBr(
        basicBlockBuilder->CreateICmpULT( indexRValue, sizeRValue ),
        nextBB,
        doneBB
        );
        
      basicBlockBuilder->SetInsertPoint( nextBB );
      llvm::Value *memberLValue = basicBlockBuilder->CreateGEP( memberDatasRValue, indexRValue );
      m_memberAdapter->llvmDispose( basicBlockBuilder, memberLValue );
      llvm::Value *nextIndexRValue = basicBlockBuilder->CreateAdd( indexRValue, sizeAdapter->llvmConst( context, 1 ) );
      sizeAdapter->llvmDefaultAssign( basicBlockBuilder, indexLValue, nextIndexRValue );
      basicBlockBuilder->CreateBr( checkBB );
        
      basicBlockBuilder->SetInsertPoint( doneBB );
      llvmCallFree( basicBlockBuilder, memberDatasRValue );
    }

    void VariableArrayAdapter::llvmDefaultAssign( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const
    {
      std::vector<FunctionParam> params;
      params.push_back( FunctionParam( "dstLValue", this, CG::USAGE_LVALUE ) );
      params.push_back( FunctionParam( "srcRValue", this, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__DefaultAssign", ExprType(), params, false, 0, true );
      basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), dstLValue, srcRValue );
    }
    
    llvm::Constant *VariableArrayAdapter::llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      RC::Handle<Context> context = basicBlockBuilder.getContext();
      RC::ConstHandle<SizeAdapter> sizeAdapter = getManager()->getSizeAdapter();
      std::vector<llvm::Constant *> memberDefaultRValues;
      memberDefaultRValues.push_back( sizeAdapter->llvmConst( context, 0 ) );
      memberDefaultRValues.push_back( sizeAdapter->llvmConst( context, 0 ) );
      memberDefaultRValues.push_back( llvm::ConstantPointerNull::get( static_cast<llvm::PointerType const *>( m_memberAdapter->llvmLType( context ) ) ) );
      return llvm::ConstantStruct::get( context->getLLVMContext(), memberDefaultRValues, true );
    }
      
    llvm::Constant *VariableArrayAdapter::llvmDefaultRValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      return llvmDefaultLValue( basicBlockBuilder );
    }

    llvm::Constant *VariableArrayAdapter::llvmDefaultLValue( BasicBlockBuilder &basicBlockBuilder ) const
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
  };
};
