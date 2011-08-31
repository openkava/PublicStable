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

#include <Fabric/Core/RT/VariableArrayDesc.h>

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Intrinsics.h>

namespace Fabric
{
  namespace CG
  {
    VariableArrayAdapter::VariableArrayAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::VariableArrayDesc> const &variableArrayDesc )
      : ArrayAdapter( manager, variableArrayDesc, 0 )
      , m_variableArrayDesc( variableArrayDesc )
      , m_memberAdapter( manager->getAdapter( variableArrayDesc->getMemberDesc() ) )
    {
    }
    
    llvm::Type const *VariableArrayAdapter::buildLLVMRawType( RC::Handle<Context> const &context ) const
    {
      llvm::Type const *llvmSizeTy = llvmSizeType( context );
      
      std::vector<llvm::Type const *> memberLLVMTypes;
      memberLLVMTypes.push_back( llvmSizeTy ); // refCount
      memberLLVMTypes.push_back( llvmSizeTy ); // allocNumMembers
      memberLLVMTypes.push_back( llvmSizeTy ); // numMembers
      memberLLVMTypes.push_back( llvm::ArrayType::get( m_memberAdapter->llvmRawType( context ), 0 ) );
      llvm::Type const *implType = llvm::StructType::get( context->getLLVMContext(), memberLLVMTypes, true );
      
      return implType->getPointerTo();
    }
    
    llvm::Type const *VariableArrayAdapter::getLLVMImplType( RC::Handle<Context> const &context ) const
    {
      return static_cast<llvm::PointerType const *>( llvmRawType( context ) )->getElementType();
    }

    void VariableArrayAdapter::llvmCompileToModule( ModuleBuilder &moduleBuilder ) const
    {
      if ( moduleBuilder.haveCompiledToModule( getCodeName() ) )
        return;
      
      RC::Handle<Context> context = moduleBuilder.getContext();
      
      llvm::Type const *implType = getLLVMImplType( context );

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
      moduleBuilder->addTypeName( getCodeName() + "Bits", implType );
      
      static const bool buildFunctions = true;

      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__Retain", ExprType(), params, false );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *rValue = functionBuilder[0];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          llvm::BasicBlock *nonNullBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "nonNull" );
          llvm::BasicBlock *doneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "done" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          basicBlockBuilder->CreateCondBr( basicBlockBuilder->CreateIsNotNull( rValue ), nonNullBB, doneBB );
          
          basicBlockBuilder->SetInsertPoint( nonNullBB );
          llvm::Value *refCountLValue = basicBlockBuilder->CreateConstGEP2_32( rValue, 0, 0 );
          llvm::Value *one = sizeAdapter->llvmConst( context, 1 );
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
        params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__Release", ExprType(), params, false );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *rValue = functionBuilder[0];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );
          llvm::BasicBlock *nonNullBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "nonNull" );
          llvm::BasicBlock *freeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "free" );
          llvm::BasicBlock *loopCheckBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "loopCheck" );
          llvm::BasicBlock *loopExecBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "loopExec" );
          llvm::BasicBlock *loopDoneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "loopDone" );
          llvm::BasicBlock *doneBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "done" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          basicBlockBuilder->CreateCondBr( basicBlockBuilder->CreateIsNotNull( rValue ), nonNullBB, doneBB );
          
          basicBlockBuilder->SetInsertPoint( nonNullBB );
          llvm::Value *refCountLValue = basicBlockBuilder->CreateConstGEP2_32( rValue, 0, 0 );
          llvm::Value *one = sizeAdapter->llvmConst( context, 1 );
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
          llvm::Value *numMembers = basicBlockBuilder->CreateLoad( basicBlockBuilder->CreateConstGEP2_32( rValue, 0, 2 ) );
          llvm::Value *members = basicBlockBuilder->CreateConstGEP2_32( basicBlockBuilder->CreateConstGEP2_32( rValue, 0, 3 ), 0, 0 );
          llvm::Value *indexPtr = sizeAdapter->llvmAlloca( basicBlockBuilder, "index" );
          basicBlockBuilder->CreateStore( sizeAdapter->llvmConst( context, 0 ), indexPtr );
          basicBlockBuilder->CreateBr( loopCheckBB );
          
          basicBlockBuilder->SetInsertPoint( loopCheckBB );
          basicBlockBuilder->CreateCondBr( basicBlockBuilder->CreateICmpEQ( basicBlockBuilder->CreateLoad(indexPtr), numMembers ), loopDoneBB, loopExecBB );
          
          basicBlockBuilder->SetInsertPoint( loopExecBB );
          llvm::Value *index = basicBlockBuilder->CreateLoad( indexPtr );
          llvm::Value *memberLValue = basicBlockBuilder->CreateGEP( members, index );
          llvm::Value *memberRValue = m_memberAdapter->llvmLValueToRValue( basicBlockBuilder, memberLValue );
          m_memberAdapter->llvmRelease( basicBlockBuilder, memberRValue );
          basicBlockBuilder->CreateStore( basicBlockBuilder->CreateAdd( index, sizeAdapter->llvmConst( context, 1 ) ), indexPtr );
          basicBlockBuilder->CreateBr( loopCheckBB );    
          
          basicBlockBuilder->SetInsertPoint( loopDoneBB );
          llvmCallFree( basicBlockBuilder, rValue );
          basicBlockBuilder->CreateBr( doneBB );
          
          basicBlockBuilder->SetInsertPoint( doneBB );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__Size", ExprType( sizeAdapter, USAGE_RVALUE ), params, false );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          
          llvm::Value *arrayRValue = functionBuilder[0];

          llvm::BasicBlock *entryBB = functionBuilder.createBasicBlock( "entry" );
          llvm::BasicBlock *nonNullBB = functionBuilder.createBasicBlock( "nonNull" );
          llvm::BasicBlock *nullBB = functionBuilder.createBasicBlock( "null" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *isNull = basicBlockBuilder->CreateIsNull( arrayRValue );
          basicBlockBuilder->CreateCondBr( isNull, nullBB, nonNullBB );
          
          basicBlockBuilder->SetInsertPoint( nonNullBB );
          llvm::Value *lengthPtr = basicBlockBuilder->CreateConstGEP2_32( arrayRValue, 0, 2 );
          llvm::Value *length = basicBlockBuilder->CreateLoad( lengthPtr );
          basicBlockBuilder->CreateRet( length );
          
          basicBlockBuilder->SetInsertPoint( nullBB );
          llvm::Value *zeroLength = llvm::ConstantInt::get( sizeAdapter->llvmRType( context ), 0 );
          basicBlockBuilder->CreateRet( zeroLength );
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
          llvm::BasicBlock *notNullBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "notNull" );
          llvm::BasicBlock *inRangeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "inRange" );
          llvm::BasicBlock *outOfRangeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "outOfRange" );

          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *isNullCond = basicBlockBuilder->CreateIsNull( arrayRValue );
          basicBlockBuilder->CreateCondBr( isNullCond, outOfRangeBB, notNullBB );
          
          basicBlockBuilder->SetInsertPoint( notNullBB );
          llvm::Value *lengthLValue = basicBlockBuilder->CreateConstGEP2_32( arrayRValue, 0, 2 );
          llvm::Value *lengthRValue = basicBlockBuilder->CreateLoad( lengthLValue );
          llvm::Value *inRangeCond = basicBlockBuilder->CreateICmpULT( indexRValue, lengthRValue );
          basicBlockBuilder->CreateCondBr( inRangeCond, inRangeBB, outOfRangeBB );
          
          basicBlockBuilder->SetInsertPoint( inRangeBB );
          llvm::Value *memberData = basicBlockBuilder->CreateConstGEP2_32( basicBlockBuilder->CreateConstGEP2_32( arrayRValue, 0, 3 ), 0, 0 );
          llvm::Value *memberLValue = basicBlockBuilder->CreateGEP( memberData, indexRValue );
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
          llvm::BasicBlock *notNullBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "notNull" );
          llvm::BasicBlock *inRangeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "inRange" );
          llvm::BasicBlock *outOfRangeBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "outOfRange" );
          llvm::BasicBlock *nonUniqueBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "nonUnique" );
          llvm::BasicBlock *uniqueBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "unique" );

          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *arrayRValue = basicBlockBuilder->CreateLoad( arrayLValue );
          llvm::Value *isNullCond = basicBlockBuilder->CreateIsNull( arrayRValue );
          basicBlockBuilder->CreateCondBr( isNullCond, outOfRangeBB, notNullBB );
          
          basicBlockBuilder->SetInsertPoint( notNullBB );
          llvm::Value *lengthLValue = basicBlockBuilder->CreateConstGEP2_32( arrayRValue, 0, 2 );
          llvm::Value *lengthRValue = basicBlockBuilder->CreateLoad( lengthLValue );
          llvm::Value *inRangeCond = basicBlockBuilder->CreateICmpULT( indexRValue, lengthRValue );
          basicBlockBuilder->CreateCondBr( inRangeCond, inRangeBB, outOfRangeBB );
          
          basicBlockBuilder->SetInsertPoint( inRangeBB );
          llvm::Value *refCount = basicBlockBuilder->CreateLoad( basicBlockBuilder->CreateStructGEP( arrayRValue, 0 ) );
          basicBlockBuilder->CreateCondBr( basicBlockBuilder->CreateICmpUGT( refCount, llvm::ConstantInt::get( sizeAdapter->llvmRType( context ), 1 ) ), nonUniqueBB, uniqueBB );
          
          basicBlockBuilder->SetInsertPoint( nonUniqueBB );
          std::vector< llvm::Type const * > argTypes;
          argTypes.push_back( basicBlockBuilder->getInt8PtrTy() );
          argTypes.push_back( llvmLType( context ) );
          llvm::FunctionType const *funcType = llvm::FunctionType::get( basicBlockBuilder->getVoidTy(), argTypes, false );
          llvm::Constant *func = basicBlockBuilder.getModuleBuilder()->getOrInsertFunction( "__"+getCodeName()+"__Split", funcType ); 

          std::vector< llvm::Value * > args;
          args.push_back( llvmAdapterPtr( basicBlockBuilder ) );
          args.push_back( arrayLValue );
          basicBlockBuilder->CreateCall( func, args.begin(), args.end() );
          basicBlockBuilder->CreateBr( uniqueBB );
          
          basicBlockBuilder->SetInsertPoint( uniqueBB );
          arrayRValue = basicBlockBuilder->CreateLoad( arrayLValue );
          llvm::Value *memberData = basicBlockBuilder->CreateConstGEP2_32( basicBlockBuilder->CreateStructGEP( arrayRValue, 3 ), 0, 0 );
          llvm::Value *memberLValue = basicBlockBuilder->CreateGEP( memberData, indexRValue );
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
          llvm::Value *stringRValue = stringAdapter->llvmCallCast( basicBlockBuilder, this, arrayLValue );
          stringAdapter->llvmAssign( basicBlockBuilder, stringLValue, stringRValue );
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
        std::string name = methodOverloadName( "push", this, m_memberAdapter );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "thisLValue", this, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "memberRValue", m_memberAdapter, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_LVALUE ), params );
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
          llvm::Value *newElementLValue = llvmNonConstIndexOp( basicBlockBuilder, thisLValue, oldSize );
          m_memberAdapter->llvmRetain( basicBlockBuilder, memberRValue );
          m_memberAdapter->llvmAssign( basicBlockBuilder, newElementLValue, memberRValue );
          basicBlockBuilder->CreateRet( thisLValue );
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
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_LVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *thisLValue = functionBuilder[0];
          llvm::Value *newSizeRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvmCallResize( basicBlockBuilder, thisLValue, newSizeRValue );
          basicBlockBuilder->CreateRet( thisLValue );
        }
      }
      
      {
        // [pzion 20110205] FIXME this version should be removed once we have automatic casting
        std::string name = methodOverloadName( "resize", this, integerAdapter );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "thisLValue", this, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "integerRValue", integerAdapter, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_LVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *thisLValue = functionBuilder[0];
          llvm::Value *integerRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *newSizeRValue = sizeAdapter->llvmCast( basicBlockBuilder, ExprValue( integerAdapter, USAGE_RVALUE, context, integerRValue ) );
          llvmCallResize( basicBlockBuilder, thisLValue, newSizeRValue );
          basicBlockBuilder->CreateRet( thisLValue );
        }
      }
      
      {
        std::string name = methodOverloadName( "size", this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "thisRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( sizeAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *thisRValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *sizeRValue = llvmCallSize( basicBlockBuilder, thisRValue );
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
          llvm::Value *thisRValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *sizeRValue = llvmCallSize( basicBlockBuilder, thisRValue );
          llvm::Value *memberSizeRValue = sizeAdapter->llvmConst( context, m_memberAdapter->getDesc()->getSize() );
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
          llvm::BasicBlock *nullBB = functionBuilder.createBasicBlock( "null" );
          llvm::BasicBlock *nonNullBB = functionBuilder.createBasicBlock( "nonNull" );
          basicBlockBuilder->CreateCondBr( basicBlockBuilder->CreateIsNull( thisRValue ), nullBB, nonNullBB );
          basicBlockBuilder->SetInsertPoint( nullBB );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreatePointerCast( thisRValue, dataAdapter->llvmRType( context ) ) );
          basicBlockBuilder->SetInsertPoint( nonNullBB );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreatePointerCast( basicBlockBuilder->CreateConstGEP1_32( basicBlockBuilder->CreateConstGEP2_32( basicBlockBuilder->CreateStructGEP( thisRValue, 3 ), 0, 0 ), 0 ), dataAdapter->llvmRType( context ) ) );
        }
      }
    }
    
    void *VariableArrayAdapter::llvmResolveExternalFunction( std::string const &functionName ) const
    {
      if ( functionName == "__"+getCodeName()+"__Resize" )
        return (void *)&VariableArrayAdapter::Resize;
      else if ( functionName == "__"+getCodeName()+"__Pop" )
        return (void *)&VariableArrayAdapter::Pop;
      else if ( functionName == "__"+getCodeName()+"__Split" )
        return (void *)&VariableArrayAdapter::Split;
      else if ( functionName == "__"+getCodeName()+"__Append" )
        return (void *)&VariableArrayAdapter::Append;
      else return 0;
    }
    
    void VariableArrayAdapter::Resize( VariableArrayAdapter const *inst, void *dst, size_t newSize )
    {
      inst->m_variableArrayDesc->setNumMembers( dst, newSize );
    }

    llvm::Value *VariableArrayAdapter::llvmConstIndexOp( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *arrayRValue, llvm::Value *indexRValue ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
      params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__ConstIndex", ExprType( m_memberAdapter, USAGE_RVALUE ), params, false, 0, true );
      return basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), arrayRValue, indexRValue );
    }


    llvm::Value *VariableArrayAdapter::llvmNonConstIndexOp( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *exprLValue, llvm::Value *indexRValue ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();

      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_LVALUE ) );
      params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__NonConstIndex", ExprType( m_memberAdapter, USAGE_LVALUE ), params, false, 0, true );
      return basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), exprLValue, indexRValue );
    }
    
    void VariableArrayAdapter::Split( VariableArrayAdapter const *inst, void *data )
    {
      inst->m_variableArrayDesc->split( data );
    }
    
    void VariableArrayAdapter::Append( VariableArrayAdapter const *inst, void *dstLValue, void const *srcRValue )
    {
      inst->m_variableArrayDesc->append( dstLValue, &srcRValue );
    }
    
    void VariableArrayAdapter::Pop( VariableArrayAdapter const *inst, void *dst, void *result )
    {
      inst->m_variableArrayDesc->pop( dst, result );
    }
    
    void VariableArrayAdapter::llvmInit( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *lValue ) const
    {
      basicBlockBuilder->CreateStore( llvm::ConstantPointerNull::get( static_cast<llvm::PointerType const *>( llvmRawType( basicBlockBuilder.getContext() ) ) ), lValue ); 
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
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__Size", ExprType( sizeAdapter, USAGE_RVALUE ), params, false );
      return basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), arrayRValue );
    }
    
    void VariableArrayAdapter::llvmRelease( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__Release", ExprType(), params, false );
      basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), rValue );
    }

    void VariableArrayAdapter::llvmDefaultAssign( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const
    {
      llvm::Value *oldDstRValue = llvmLValueToRValue( basicBlockBuilder, dstLValue );
      basicBlockBuilder->CreateStore( srcRValue, dstLValue );
      llvmRelease( basicBlockBuilder, oldDstRValue );
    }

    void VariableArrayAdapter::llvmRetain( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__Retain", ExprType(), params, false );
      basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), rValue );
    }
    
    llvm::Constant *VariableArrayAdapter::llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      return llvm::ConstantPointerNull::get( static_cast<llvm::PointerType const *>( llvmRawType( basicBlockBuilder.getContext() ) ) );
    }
  };
};
