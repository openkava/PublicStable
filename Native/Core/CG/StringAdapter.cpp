#include "StringAdapter.h"
#include "BooleanAdapter.h"
#include "ByteAdapter.h"
#include "IntegerAdapter.h"
#include "SizeAdapter.h"
#include "ScalarAdapter.h"
#include "OpaqueAdapter.h"
#include "Manager.h"
#include "ModuleBuilder.h"
#include "FunctionBuilder.h"
#include "BasicBlockBuilder.h"
#include "OverloadNames.h"

#include <Fabric/Core/RT/StringDesc.h>
#include <Fabric/Core/RT/StringImpl.h>

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Intrinsics.h>
#include <llvm/GlobalVariable.h>

namespace Fabric
{
  namespace CG
  {
    StringAdapter::StringAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::StringDesc> const &stringDesc )
      : Adapter( manager, stringDesc, 0 )
      , m_stringDesc( stringDesc )
    {
      llvm::Type const *llvmSizeTy = llvmSizeType();
      
      std::vector<llvm::Type const *> memberLLVMTypes;
      memberLLVMTypes.push_back( llvmSizeTy ); // refCount
      memberLLVMTypes.push_back( llvmSizeTy ); // allocSize
      memberLLVMTypes.push_back( llvmSizeTy ); // length
      memberLLVMTypes.push_back( llvm::ArrayType::get( llvm::Type::getInt8Ty( getLLVMContext() ), 0 ) );
      m_implType = llvm::StructType::get( getLLVMContext(), memberLLVMTypes );
      
      setLLVMType( m_implType->getPointerTo() );
    }
    
    llvm::Value *StringAdapter::llvmCallCast( BasicBlockBuilder &basicBlockBuilder, RC::ConstHandle<Adapter> const &adapter, llvm::Value *lValue ) const
    {
      FABRIC_ASSERT( lValue->getType() == adapter->llvmLType() );

      llvm::Type const *int8PtrTy = basicBlockBuilder->getInt8PtrTy();
      
      std::vector< llvm::Type const * > argTypes;
      argTypes.push_back( int8PtrTy );
      argTypes.push_back( int8PtrTy );
      llvm::FunctionType const *funcType = llvm::FunctionType::get( llvmRType(), argTypes, false );
      llvm::Constant *func = basicBlockBuilder.getModuleBuilder()->getOrInsertFunction( "__String__Cast", funcType );

      std::vector< llvm::Value * > args;
      args.push_back( basicBlockBuilder->CreatePointerCast( lValue, int8PtrTy ) );
      args.push_back( adapter->llvmAdapterPtr( basicBlockBuilder ) );
      return basicBlockBuilder->CreateCall( func, args.begin(), args.end() );
    }
    
    void StringAdapter::llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const
    {
      if ( moduleBuilder.contains( getCodeName(), buildFunctions ) )
        return;
      
      moduleBuilder->addTypeName( getCodeName(), llvmRawType() );
      moduleBuilder->addTypeName( getCodeName() + "Bits", m_implType );

      RC::ConstHandle<BooleanAdapter> booleanAdapter = getManager()->getBooleanAdapter();
      RC::ConstHandle<IntegerAdapter> integerAdapter = getManager()->getIntegerAdapter();
      RC::ConstHandle<SizeAdapter> sizeAdapter = getManager()->getSizeAdapter();
      RC::ConstHandle<OpaqueAdapter> dataAdapter = getManager()->getDataAdapter();

      llvm::Function *reportFunction;
      {
        std::vector< llvm::Type const * > argTypes;
        argTypes.push_back( llvm::Type::getInt8PtrTy( getLLVMContext() ) );
        argTypes.push_back( sizeAdapter->llvmRType() );
        llvm::FunctionType const *funcType = llvm::FunctionType::get( llvm::Type::getVoidTy( getLLVMContext() ), argTypes, false );
        
        llvm::AttributeWithIndex AWI[1];
        AWI[0] = llvm::AttributeWithIndex::get( ~0u, llvm::Attribute::InlineHint | llvm::Attribute::NoUnwind );
        llvm::AttrListPtr attrListPtr = llvm::AttrListPtr::get( AWI, 1 );
        
        reportFunction = llvm::cast<llvm::Function>( moduleBuilder->getOrInsertFunction( "report", funcType, attrListPtr ) ); 
      }
      
      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "string", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__String__Report", ExprType(), params, false );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *rValue = functionBuilder[0];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          llvm::BasicBlock *notNullBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "notNull" );
          llvm::BasicBlock *doneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "done" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *isNotNullRValue = basicBlockBuilder->CreateIsNotNull( rValue );
          basicBlockBuilder->CreateCondBr( isNotNullRValue, notNullBB, doneBB );
          
          basicBlockBuilder->SetInsertPoint( notNullBB );
          llvm::Value *dataLValue = basicBlockBuilder->CreateConstGEP2_32( rValue, 0, 3 );
          llvm::Value *dataRValue = basicBlockBuilder->CreateConstGEP2_32( dataLValue, 0, 0 );
          llvm::Value *lengthLValue = basicBlockBuilder->CreateConstGEP2_32( rValue, 0, 2 );
          llvm::Value *lengthRValue = sizeAdapter->llvmLValueToRValue( basicBlockBuilder, lengthLValue );
          basicBlockBuilder->CreateCall2( reportFunction, dataRValue, lengthRValue );
          basicBlockBuilder->CreateBr( doneBB );
          
          basicBlockBuilder->SetInsertPoint( doneBB );
          basicBlockBuilder->CreateRetVoid();
        }
      }

      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "string", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__String__Retain", ExprType(), params, false );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *rValue = functionBuilder[0];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          llvm::BasicBlock *addRefBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "addRef" );
          llvm::BasicBlock *doneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "done" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          basicBlockBuilder->CreateCondBr( basicBlockBuilder->CreateIsNotNull( rValue ), addRefBB, doneBB );
          
          basicBlockBuilder->SetInsertPoint( addRefBB );
          llvm::Value *refCountLValue = basicBlockBuilder->CreateConstGEP2_32( rValue, 0, 0 );
          llvm::Value *one = sizeAdapter->llvmConst( 1 );
          static const size_t numIntrinsicTypes = 2;
          llvm::Type const *intrinsicTypes[numIntrinsicTypes] =
          {
            one->getType(),
            refCountLValue->getType()
          };
          llvm::Function *intrinsic = llvm::Intrinsic::getDeclaration( basicBlockBuilder.getModuleBuilder(), llvm::Intrinsic::atomic_load_add, intrinsicTypes, numIntrinsicTypes );
          FABRIC_ASSERT( intrinsic );
          basicBlockBuilder->CreateCall2( intrinsic, refCountLValue, one );
          basicBlockBuilder->CreateBr( doneBB );
          
          basicBlockBuilder->SetInsertPoint( doneBB );
          basicBlockBuilder->CreateRetVoid();
        }
      }

      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "string", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__String__Release", ExprType(), params, false );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *rValue = functionBuilder[0];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          llvm::BasicBlock *nonNullBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "nonNull" );
          llvm::BasicBlock *freeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "free" );
          llvm::BasicBlock *doneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "done" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          basicBlockBuilder->CreateCondBr( basicBlockBuilder->CreateIsNotNull( rValue ), nonNullBB, doneBB );
          
          basicBlockBuilder->SetInsertPoint( nonNullBB );
          llvm::Value *refCountLValue = basicBlockBuilder->CreateConstGEP2_32( rValue, 0, 0 );
          llvm::Value *one = sizeAdapter->llvmConst( 1 );
          static const size_t numIntrinsicTypes = 2;
          llvm::Type const *intrinsicTypes[numIntrinsicTypes] =
          {
            one->getType(),
            refCountLValue->getType()
          };
          llvm::Function *intrinsic = llvm::Intrinsic::getDeclaration( basicBlockBuilder.getModuleBuilder(), llvm::Intrinsic::atomic_load_sub, intrinsicTypes, numIntrinsicTypes );
          FABRIC_ASSERT( intrinsic );
          llvm::Value *oldRefCountRValue = basicBlockBuilder->CreateCall2( intrinsic, refCountLValue, one );
          llvm::Value *shouldFreeRValue = basicBlockBuilder->CreateICmpEQ( oldRefCountRValue, one );
          basicBlockBuilder->CreateCondBr( shouldFreeRValue, freeBB, doneBB );
          
          basicBlockBuilder->SetInsertPoint( freeBB );
          llvmCallFree( basicBlockBuilder, rValue );
          basicBlockBuilder->CreateBr( doneBB );
          
          basicBlockBuilder->SetInsertPoint( doneBB );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "string", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__String__RefCount", ExprType( sizeAdapter, USAGE_RVALUE ), params, false );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *stringRValue = functionBuilder[0];

          llvm::BasicBlock *entryBB = functionBuilder.createBasicBlock( "entry" );
          llvm::BasicBlock *nonNullBB = functionBuilder.createBasicBlock( "nonNull" );
          llvm::BasicBlock *nullBB = functionBuilder.createBasicBlock( "null" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *isNull = basicBlockBuilder->CreateIsNull( stringRValue );
          basicBlockBuilder->CreateCondBr( isNull, nullBB, nonNullBB );
          
          basicBlockBuilder->SetInsertPoint( nonNullBB );
          llvm::Value *lengthPtr = basicBlockBuilder->CreateConstGEP2_32( stringRValue, 0, 0 );
          llvm::Value *length = basicBlockBuilder->CreateLoad( lengthPtr );
          basicBlockBuilder->CreateRet( length );
          
          basicBlockBuilder->SetInsertPoint( nullBB );
          llvm::Value *zeroLength = llvm::ConstantInt::get( sizeAdapter->llvmRType(), 0 );
          basicBlockBuilder->CreateRet( zeroLength );
        }
      }
      
      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "string", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__String__Length", ExprType( sizeAdapter, USAGE_RVALUE ), params, false );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *stringRValue = functionBuilder[0];

          llvm::BasicBlock *entryBB = functionBuilder.createBasicBlock( "entry" );
          llvm::BasicBlock *nonNullBB = functionBuilder.createBasicBlock( "nonNull" );
          llvm::BasicBlock *nullBB = functionBuilder.createBasicBlock( "null" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *isNull = basicBlockBuilder->CreateIsNull( stringRValue );
          basicBlockBuilder->CreateCondBr( isNull, nullBB, nonNullBB );
          
          basicBlockBuilder->SetInsertPoint( nonNullBB );
          llvm::Value *lengthPtr = basicBlockBuilder->CreateConstGEP2_32( stringRValue, 0, 2 );
          llvm::Value *length = basicBlockBuilder->CreateLoad( lengthPtr );
          basicBlockBuilder->CreateRet( length );
          
          basicBlockBuilder->SetInsertPoint( nullBB );
          llvm::Value *zeroLength = llvm::ConstantInt::get( sizeAdapter->llvmRType(), 0 );
          basicBlockBuilder->CreateRet( zeroLength );
        }
      }

      {
        // [pzion 20110202] Cast string to boolean
        std::string name = constructOverloadName( booleanAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "booleanLValue", booleanAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "stringRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *booleanLValue = functionBuilder[0];
          llvm::Value *stringRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *lengthRValue = llvmCallLength( basicBlockBuilder, stringRValue );
          llvm::Value *booleanRValue = basicBlockBuilder->CreateICmpNE( lengthRValue, llvm::ConstantInt::get( lengthRValue->getType(), 0 ) );
          basicBlockBuilder->CreateStore( booleanRValue, booleanLValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      llvm::Function *assignAddFunction = 0;
      {
        std::vector< llvm::Type const * > argTypes;
        argTypes.push_back( llvmLType() );
        argTypes.push_back( llvmRType() );
        llvm::FunctionType const *funcType = llvm::FunctionType::get( llvm::Type::getVoidTy( moduleBuilder.getLLVMContext() ), argTypes, false );
        llvm::Constant *func = moduleBuilder->getOrInsertFunction( "__String__Append", funcType ); 

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
          args.push_back( lhsLValue );
          args.push_back( rhsRValue );
          basicBlockBuilder->CreateCall( func, args.begin(), args.end() );
          basicBlockBuilder->CreateRetVoid();
          assignAddFunction = functionBuilder.getLLVMFunction();
        }
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
          llvm::Value *resultLValue = llvmAlloca( basicBlockBuilder, "result" );
          llvmInit( basicBlockBuilder, resultLValue );
          llvmRetain( basicBlockBuilder, lhsRValue );
          llvmAssign( basicBlockBuilder, resultLValue, lhsRValue );
          basicBlockBuilder->CreateCall2( assignAddFunction, resultLValue, rhsRValue );
          llvm::Value *resultRValue = basicBlockBuilder->CreateLoad( resultLValue );
          basicBlockBuilder->CreateRet( resultRValue );
        }
      }
      
      {
        std::string name = methodOverloadName( "refCount", this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "selfRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( sizeAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *selfRValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *lengthRValue = llvmCallRefCount( basicBlockBuilder, selfRValue );
          basicBlockBuilder->CreateRet( lengthRValue );
        }
      }
      
      {
        std::string name = methodOverloadName( "length", this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "selfRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( sizeAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *selfRValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *lengthRValue = llvmCallLength( basicBlockBuilder, selfRValue );
          basicBlockBuilder->CreateRet( lengthRValue );
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
          llvm::Value *lengthRValue = llvmCallLength( basicBlockBuilder, selfRValue );
          llvm::Value *dataSizeRValue = basicBlockBuilder->CreateAdd( lengthRValue, llvm::ConstantInt::get( lengthRValue->getType(), 1 ) );
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
          llvm::BasicBlock *nullBB = functionBuilder.createBasicBlock( "null" );
          llvm::BasicBlock *nonNullBB = functionBuilder.createBasicBlock( "nonNull" );
          basicBlockBuilder->CreateCondBr( basicBlockBuilder->CreateIsNull( selfRValue ), nullBB, nonNullBB );
          
          basicBlockBuilder->SetInsertPoint( nullBB );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreatePointerCast( selfRValue, dataAdapter->llvmRType() ) );
          
          basicBlockBuilder->SetInsertPoint( nonNullBB );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreatePointerCast( basicBlockBuilder->CreateConstGEP1_32( basicBlockBuilder->CreateConstGEP2_32( basicBlockBuilder->CreateStructGEP( selfRValue, 3 ), 0, 0 ), 0 ), dataAdapter->llvmRType() ) );
        }
      }
      
      {
        std::string name = methodOverloadName( "compare", this, this );
        std::vector<FunctionParam> params;
        params.push_back( FunctionParam( "selfRValue", this, USAGE_RVALUE ) );
        params.push_back( FunctionParam( "otherRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( integerAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *selfRValue = functionBuilder[0];
          llvm::Value *otherRValue = functionBuilder[1];
          
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          llvm::BasicBlock *entryBB = functionBuilder.createBasicBlock( "entry" );
          llvm::BasicBlock *checkPointersBB = functionBuilder.createBasicBlock( "checkPointers" );
          llvm::BasicBlock *selfIsNullBB = functionBuilder.createBasicBlock( "selfIsNull" );
          llvm::BasicBlock *selfIsNotNullBB = functionBuilder.createBasicBlock( "selfIsNotNull" );
          llvm::BasicBlock *shallowBB = functionBuilder.createBasicBlock( "shallow" );
          llvm::BasicBlock *deepBB = functionBuilder.createBasicBlock( "deep" );
          llvm::BasicBlock *checkBB = functionBuilder.createBasicBlock( "check" );
          llvm::BasicBlock *checkSelfLengthBB = functionBuilder.createBasicBlock( "checkSelfLength" );
          llvm::BasicBlock *verifyOtherLengthBB = functionBuilder.createBasicBlock( "verifyOtherLength" );
          llvm::BasicBlock *checkOtherLengthBB = functionBuilder.createBasicBlock( "checkOtherLength" );
          llvm::BasicBlock *checkCharsBB = functionBuilder.createBasicBlock( "checkChars" );
          llvm::BasicBlock *checkCharsLTBB = functionBuilder.createBasicBlock( "checkCharsLT" );
          llvm::BasicBlock *checkCharsGTBB = functionBuilder.createBasicBlock( "checkCharsGT" );
          llvm::BasicBlock *nextBB = functionBuilder.createBasicBlock( "next" );
          llvm::BasicBlock *eqBB = functionBuilder.createBasicBlock( "eq" );
          llvm::BasicBlock *ltBB = functionBuilder.createBasicBlock( "lt" );
          llvm::BasicBlock *gtBB = functionBuilder.createBasicBlock( "gt" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          basicBlockBuilder->CreateBr( checkPointersBB );
          
          basicBlockBuilder->SetInsertPoint( checkPointersBB );
          basicBlockBuilder->CreateCondBr(
            basicBlockBuilder->CreateIsNull(selfRValue),
            selfIsNullBB,
            selfIsNotNullBB
            );
            
          basicBlockBuilder->SetInsertPoint( selfIsNullBB );
          basicBlockBuilder->CreateCondBr(
            basicBlockBuilder->CreateIsNull(otherRValue),
            eqBB,
            ltBB
            );
            
          basicBlockBuilder->SetInsertPoint( selfIsNotNullBB );
          basicBlockBuilder->CreateCondBr(
            basicBlockBuilder->CreateIsNull(otherRValue),
            gtBB,
            shallowBB
            );

          basicBlockBuilder->SetInsertPoint( shallowBB );
          basicBlockBuilder->CreateCondBr(
            basicBlockBuilder->CreateICmpEQ( selfRValue, otherRValue ),
            eqBB,
            deepBB
            );
          
          basicBlockBuilder->SetInsertPoint( deepBB );
          llvm::Value *selfLengthRValue = basicBlockBuilder->CreateLoad( basicBlockBuilder->CreateStructGEP( selfRValue, 2, "selfLengthPtr" ), "selfLength" );
          llvm::Value *selfCStrRValue = basicBlockBuilder->CreateConstGEP2_32( basicBlockBuilder->CreateStructGEP( selfRValue, 3 ), 0, 0, "selfCStr" );
          llvm::Value *otherLengthRValue = basicBlockBuilder->CreateLoad( basicBlockBuilder->CreateStructGEP( otherRValue, 2, "otherLengthPtr" ), "otherLength" );
          llvm::Value *otherCStrRValue = basicBlockBuilder->CreateConstGEP2_32( basicBlockBuilder->CreateStructGEP( otherRValue, 3 ), 0, 0, "otherCStr" );
          llvm::Value *indexLValue = basicBlockBuilder->CreateAlloca( sizeAdapter->llvmRType(), sizeAdapter->llvmConst(1), "indexPtr" );
          basicBlockBuilder->CreateStore( sizeAdapter->llvmConst(0), indexLValue );
          basicBlockBuilder->CreateBr( checkBB );
          
          basicBlockBuilder->SetInsertPoint( checkBB );
          llvm::Value *indexRValue = basicBlockBuilder->CreateLoad( indexLValue, "index" );
          basicBlockBuilder->CreateBr( checkSelfLengthBB );
          
          basicBlockBuilder->SetInsertPoint( checkSelfLengthBB );
          basicBlockBuilder->CreateCondBr(
            basicBlockBuilder->CreateICmpULT( indexRValue, selfLengthRValue ),
            checkOtherLengthBB,
            verifyOtherLengthBB
            );
          
          basicBlockBuilder->SetInsertPoint( verifyOtherLengthBB );
          basicBlockBuilder->CreateCondBr(
            basicBlockBuilder->CreateICmpULT( indexRValue, otherLengthRValue ),
            ltBB,
            eqBB
            );
          
          basicBlockBuilder->SetInsertPoint( checkOtherLengthBB );
          basicBlockBuilder->CreateCondBr(
            basicBlockBuilder->CreateICmpULT( indexRValue, otherLengthRValue ),
            checkCharsBB,
            gtBB
            );
            
          basicBlockBuilder->SetInsertPoint( checkCharsBB );
          llvm::Value *selfCharPtr = basicBlockBuilder->CreateGEP( selfCStrRValue, indexRValue, "selfCharPtr" );
          llvm::Value *selfChar = basicBlockBuilder->CreateLoad( selfCharPtr, "selfChar" );
          llvm::Value *otherCharPtr = basicBlockBuilder->CreateGEP( otherCStrRValue, indexRValue, "otherCharPtr" );
          llvm::Value *otherChar = basicBlockBuilder->CreateLoad( otherCharPtr, "otherChar" );
          basicBlockBuilder->CreateBr( checkCharsLTBB );
          
          basicBlockBuilder->SetInsertPoint( checkCharsLTBB );
          basicBlockBuilder->CreateCondBr( basicBlockBuilder->CreateICmpULT( selfChar, otherChar ), ltBB, checkCharsGTBB );

          basicBlockBuilder->SetInsertPoint( checkCharsGTBB );
          basicBlockBuilder->CreateCondBr( basicBlockBuilder->CreateICmpUGT( selfChar, otherChar ), gtBB, nextBB );

          basicBlockBuilder->SetInsertPoint( nextBB );
          basicBlockBuilder->CreateStore(
            basicBlockBuilder->CreateAdd(
              indexRValue,
              sizeAdapter->llvmConst(1)
              ),
            indexLValue
            );
          basicBlockBuilder->CreateBr( checkBB );
          
          basicBlockBuilder->SetInsertPoint( eqBB );
          basicBlockBuilder->CreateRet( integerAdapter->llvmConst(0) );
          
          basicBlockBuilder->SetInsertPoint( ltBB );
          basicBlockBuilder->CreateRet( integerAdapter->llvmConst(-1) );
          
          basicBlockBuilder->SetInsertPoint( gtBB );
          basicBlockBuilder->CreateRet( integerAdapter->llvmConst(1) );
        }
      }
      
      {
        std::string name = binOpOverloadName( BIN_OP_EQ, this, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "lhsRValue", this, USAGE_RVALUE ) );
        params.push_back( FunctionParam( "rhsRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( booleanAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *lhsRValue = functionBuilder[0];
          llvm::Value *rhsRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet(
            basicBlockBuilder->CreateICmpEQ(
              llvmCallCompare( basicBlockBuilder, lhsRValue, rhsRValue ),
              integerAdapter->llvmConst(0)
              )
            );
        }
      }
      
      {
        std::string name = binOpOverloadName( BIN_OP_NE, this, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "lhsRValue", this, USAGE_RVALUE ) );
        params.push_back( FunctionParam( "rhsRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( booleanAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *lhsRValue = functionBuilder[0];
          llvm::Value *rhsRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet(
            basicBlockBuilder->CreateICmpNE(
              llvmCallCompare( basicBlockBuilder, lhsRValue, rhsRValue ),
              integerAdapter->llvmConst(0)
              )
            );
        }
      }
      
      {
        std::string name = binOpOverloadName( BIN_OP_GT, this, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "lhsRValue", this, USAGE_RVALUE ) );
        params.push_back( FunctionParam( "rhsRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( booleanAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *lhsRValue = functionBuilder[0];
          llvm::Value *rhsRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet(
            basicBlockBuilder->CreateICmpSGT(
              llvmCallCompare( basicBlockBuilder, lhsRValue, rhsRValue ),
              integerAdapter->llvmConst(0)
              )
            );
        }
      }
      
      {
        std::string name = binOpOverloadName( BIN_OP_GE, this, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "lhsRValue", this, USAGE_RVALUE ) );
        params.push_back( FunctionParam( "rhsRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( booleanAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *lhsRValue = functionBuilder[0];
          llvm::Value *rhsRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet(
            basicBlockBuilder->CreateICmpSGE(
              llvmCallCompare( basicBlockBuilder, lhsRValue, rhsRValue ),
              integerAdapter->llvmConst(0)
              )
            );
        }
      }
      
      {
        std::string name = binOpOverloadName( BIN_OP_LT, this, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "lhsRValue", this, USAGE_RVALUE ) );
        params.push_back( FunctionParam( "rhsRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( booleanAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *lhsRValue = functionBuilder[0];
          llvm::Value *rhsRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet(
            basicBlockBuilder->CreateICmpSLT(
              llvmCallCompare( basicBlockBuilder, lhsRValue, rhsRValue ),
              integerAdapter->llvmConst(0)
              )
            );
        }
      }
      
      {
        std::string name = binOpOverloadName( BIN_OP_LE, this, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "lhsRValue", this, USAGE_RVALUE ) );
        params.push_back( FunctionParam( "rhsRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( booleanAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *lhsRValue = functionBuilder[0];
          llvm::Value *rhsRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet(
            basicBlockBuilder->CreateICmpSLE(
              llvmCallCompare( basicBlockBuilder, lhsRValue, rhsRValue ),
              integerAdapter->llvmConst(0)
              )
            );
        }
      }
    }

    void StringAdapter::llvmInit( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *lValue ) const
    {
      basicBlockBuilder->CreateStore( llvm::ConstantPointerNull::get( m_implType->getPointerTo() ), lValue ); 
    }
    
    void *StringAdapter::Cast( void const *lValue, Adapter const *adapter )
    {
      std::string dataAdapter = adapter->toString( lValue );

      void *bits = 0;
      RT::StringImpl::SetValue( dataAdapter.data(), dataAdapter.length(), &bits );
      return bits;
    }
    
    void StringAdapter::Append( void *dstLValue, void const *srcRValue )
    {
      RT::StringImpl::Append( dstLValue, &srcRValue );
    }
    
    void StringAdapter::llvmReport( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "string", this, USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__String__Report", ExprType(), params, false );
      basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), rValue );
    }
    
    void *StringAdapter::llvmResolveExternalFunction( std::string const &functionName ) const
    {
      if ( functionName == "__String__Append" )
        return (void *)&StringAdapter::Append;
      else if ( functionName == "__String__Cast" )
        return (void *)&StringAdapter::Cast;
      else return 0;
    }

    void StringAdapter::llvmRetain( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "string", this, USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__String__Retain", ExprType(), params, false );
      basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), rValue );
    }

    void StringAdapter::llvmDefaultAssign( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const
    {
      llvm::Value *oldDstRValue = llvmLValueToRValue( basicBlockBuilder, dstLValue );
      basicBlockBuilder->CreateStore( srcRValue, dstLValue );
      llvmRelease( basicBlockBuilder, oldDstRValue );
    }

    void StringAdapter::llvmRelease( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "string", this, USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__String__Release", ExprType(), params, false );
      basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), rValue );
    }
    
    llvm::Value *StringAdapter::llvmCallRefCount( BasicBlockBuilder &basicBlockBuilder, llvm::Value *stringRValue ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = getManager()->getSizeAdapter();

      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "string", this, USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__String__RefCount", ExprType( sizeAdapter, USAGE_RVALUE ), params, false );
      return basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), stringRValue );
    }
    
    llvm::Value *StringAdapter::llvmCallLength( BasicBlockBuilder &basicBlockBuilder, llvm::Value *stringRValue ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = getManager()->getSizeAdapter();

      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "string", this, USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__String__Length", ExprType( sizeAdapter, USAGE_RVALUE ), params, false );
      return basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), stringRValue );
    }
    
    llvm::Value *StringAdapter::llvmCallCompare( BasicBlockBuilder &basicBlockBuilder, llvm::Value *lhsRValue, llvm::Value *rhsRValue ) const
    {
      RC::ConstHandle<IntegerAdapter> integerAdapter = getManager()->getIntegerAdapter();

      std::vector<FunctionParam> params;
      params.push_back( FunctionParam( "lhs", this, USAGE_RVALUE ) );
      params.push_back( FunctionParam( "rhs", this, USAGE_RVALUE ) );
      std::string name = methodOverloadName( "compare", this, this );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), name, ExprType( integerAdapter, USAGE_RVALUE ), params, false );
      return basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), lhsRValue, rhsRValue );
    }
    
    std::string StringAdapter::toString( void const *data ) const
    {
      char const *stringData = m_stringDesc->getValueData( data );
      size_t stringLength = m_stringDesc->getValueLength( data );
      return _( stringData, stringLength, SIZE_MAX, '"' );
    }
    
    llvm::Constant *StringAdapter::llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      return llvm::ConstantPointerNull::get( m_implType->getPointerTo() );
    }
  };
};
