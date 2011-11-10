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
      , m_isCopyOnWrite( m_variableArrayDesc->isCopyOnWrite() )
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
          llvm::BasicBlock *nonNullBB = functionBuilder.createBasicBlock( "nonNull" );
          llvm::BasicBlock *nullBB = functionBuilder.createBasicBlock( "null" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *bitsLValue = basicBlockBuilder->CreateLoad( arrayRValue );
          llvm::Value *isNull = basicBlockBuilder->CreateIsNull( bitsLValue );
          basicBlockBuilder->CreateCondBr( isNull, nullBB, nonNullBB );
          
          basicBlockBuilder->SetInsertPoint( nonNullBB );
          llvm::Value *sizeLValue = basicBlockBuilder->CreateStructGEP( bitsLValue, 2 );
          llvm::Value *sizeRValue= basicBlockBuilder->CreateLoad( sizeLValue );
          basicBlockBuilder->CreateRet( sizeRValue );
          
          basicBlockBuilder->SetInsertPoint( nullBB );
          basicBlockBuilder->CreateRet( sizeAdapter->llvmConst( context, 0 ) );
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
        FunctionBuilder functionBuilder( moduleBuilder, "__"+getCodeName()+"__ConstIndexNoCheckLValue", ExprType( m_memberAdapter, USAGE_LVALUE ), params, false, 0, true );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::Value *arrayRValue = functionBuilder[0];
          llvm::Value *indexRValue = functionBuilder[1];

          llvm::BasicBlock *entryBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "entry" );

          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *bitsLValue = basicBlockBuilder->CreateLoad( arrayRValue );
          llvm::Value *memberData = basicBlockBuilder->CreateConstGEP2_32( basicBlockBuilder->CreateStructGEP( bitsLValue, 3 ), 0, 0 );
          llvm::Value *memberLValue = basicBlockBuilder->CreateGEP( memberData, indexRValue );
          basicBlockBuilder->CreateRet( memberLValue );
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
          llvm::Value *memberLValue = llvmConstIndexOp_NoCheckLValue( basicBlockBuilder, arrayRValue, indexRValue );
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
          llvm::BasicBlock *nonUniqueBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "nonUnique" );
          llvm::BasicBlock *uniqueBB = basicBlockBuilder.getFunctionBuilder().createBasicBlock( "unique" );

          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *bitsLValue = basicBlockBuilder->CreateLoad( arrayLValue );
          llvm::Value *refCount = basicBlockBuilder->CreateLoad( basicBlockBuilder->CreateStructGEP( bitsLValue, 0 ) );
          basicBlockBuilder->CreateCondBr( basicBlockBuilder->CreateICmpUGT( refCount, sizeAdapter->llvmConst( context, 1 ) ), nonUniqueBB, uniqueBB );
          
          basicBlockBuilder->SetInsertPoint( nonUniqueBB );
          if ( m_isCopyOnWrite )
          {
            std::vector< llvm::Type const * > argTypes;
            argTypes.push_back( basicBlockBuilder->getInt8PtrTy() );
            argTypes.push_back( llvmLType( context ) );
            llvm::FunctionType const *funcType = llvm::FunctionType::get( basicBlockBuilder->getVoidTy(), argTypes, false );
            llvm::Constant *func = basicBlockBuilder.getModuleBuilder()->getOrInsertFunction( "__"+getCodeName()+"__Split", funcType ); 

            std::vector<llvm::Value *> args;
            args.push_back( llvmAdapterPtr( basicBlockBuilder ) );
            args.push_back( arrayLValue );
            basicBlockBuilder->CreateCall( func, args.begin(), args.end() );
          }
          basicBlockBuilder->CreateBr( uniqueBB );
          
          basicBlockBuilder->SetInsertPoint( uniqueBB );
          bitsLValue = basicBlockBuilder->CreateLoad( arrayLValue );
          llvm::Value *memberData = basicBlockBuilder->CreateConstGEP2_32( basicBlockBuilder->CreateStructGEP( bitsLValue, 3 ), 0, 0 );
          llvm::Value *memberLValue = basicBlockBuilder->CreateGEP( memberData, indexRValue );
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
            llvm::BasicBlock *nullBB = functionBuilder.createBasicBlock( "null" );
            llvm::BasicBlock *nonNullBB = functionBuilder.createBasicBlock( "nonNull" );
            llvm::Value *bitsLValue = basicBlockBuilder->CreateLoad( selfRValue );
            basicBlockBuilder->CreateCondBr( basicBlockBuilder->CreateIsNull( bitsLValue ), nullBB, nonNullBB );
            basicBlockBuilder->SetInsertPoint( nullBB );
            basicBlockBuilder->CreateRet( basicBlockBuilder->CreatePointerCast( bitsLValue, dataAdapter->llvmRType(context) ) );
            basicBlockBuilder->SetInsertPoint( nonNullBB );
            basicBlockBuilder->CreateRet(
              basicBlockBuilder->CreatePointerCast(
                basicBlockBuilder->CreateConstGEP1_32(
                  basicBlockBuilder->CreateConstGEP2_32(
                    basicBlockBuilder->CreateStructGEP( bitsLValue, 3 ),
                    0, 0
                    ),
                    0
                  ),
                  dataAdapter->llvmRType(context)
                )
              );
          }
        }
      }
    }
    
    void *VariableArrayAdapter::llvmResolveExternalFunction( std::string const &functionName ) const
    {
      if ( functionName == "__"+getCodeName()+"__Resize" )
        return (void *)&VariableArrayAdapter::Resize;
      else if ( functionName == "__"+getCodeName()+"__Pop" )
        return (void *)&VariableArrayAdapter::Pop;
      else if ( m_isCopyOnWrite && functionName == "__"+getCodeName()+"__Split" )
        return (void *)&VariableArrayAdapter::Split;
      else if ( functionName == "__"+getCodeName()+"__Append" )
        return (void *)&VariableArrayAdapter::Append;
      else return ArrayAdapter::llvmResolveExternalFunction( functionName );
    }
    
    void VariableArrayAdapter::Resize( VariableArrayAdapter const *inst, void *dst, size_t newSize )
    {
      inst->m_variableArrayDesc->setNumMembers( dst, newSize );
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

    llvm::Value *VariableArrayAdapter::llvmConstIndexOp_NoCheckLValue( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *arrayRValue, llvm::Value *indexRValue ) const
    {
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();
      std::vector< FunctionParam > params;
      params.push_back( FunctionParam( "array", this, CG::USAGE_RVALUE ) );
      params.push_back( FunctionParam( "index", sizeAdapter, CG::USAGE_RVALUE ) );
      FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), "__"+getCodeName()+"__ConstIndexNoCheckLValue", ExprType( m_memberAdapter, USAGE_LVALUE ), params, false, 0, true );
      return basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), arrayRValue, indexRValue );
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
    
    void VariableArrayAdapter::Split( VariableArrayAdapter const *inst, void *data )
    {
      FABRIC_ASSERT( inst->m_isCopyOnWrite );
      inst->m_variableArrayDesc->split( data );
    }
    
    void VariableArrayAdapter::Append( VariableArrayAdapter const *inst, void *dstLValue, void const *srcRValue )
    {
      inst->m_variableArrayDesc->append( dstLValue, srcRValue );
    }
    
    void VariableArrayAdapter::Pop( VariableArrayAdapter const *inst, void *dst, void *result )
    {
      inst->m_variableArrayDesc->pop( dst, result );
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
    
    void VariableArrayAdapter::llvmRetain( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *bitsLValue ) const
    {    
      RC::Handle<Context> context = basicBlockBuilder.getContext();
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();

      std::vector<llvm::Type const *> argTypes;
      argTypes.push_back( bitsLValue->getType() );
      llvm::FunctionType const *funcType = llvm::FunctionType::get( llvm::Type::getVoidTy( context->getLLVMContext() ), argTypes, false );
      
      llvm::AttributeWithIndex AWI[1];
      AWI[0] = llvm::AttributeWithIndex::get( ~0u, llvm::Attribute::InlineHint | llvm::Attribute::NoUnwind );
      llvm::AttrListPtr attrListPtr = llvm::AttrListPtr::get( AWI, 1 );

      std::string name = "__"+getCodeName()+"_Retain";
      llvm::Function *func = llvm::cast<llvm::Function>( basicBlockBuilder.getModuleBuilder()->getFunction( name ) );
      if ( !func )
      {
        ModuleBuilder &mb = basicBlockBuilder.getModuleBuilder();
        
        func = llvm::cast<llvm::Function>( mb->getOrInsertFunction( name, funcType, attrListPtr ) ); 
        func->setLinkage( llvm::GlobalValue::PrivateLinkage );
        
        FunctionBuilder fb( mb, funcType, func );
        llvm::Argument *bitsLValue = fb[0];
        bitsLValue->setName( "bitsLValue" );
        bitsLValue->addAttr( llvm::Attribute::NoCapture );
        
        BasicBlockBuilder bbb( fb );

        llvm::BasicBlock *entryBB = fb.createBasicBlock( "entry" );
        llvm::BasicBlock *nonNullBB = fb.createBasicBlock( "nonNull" );
        llvm::BasicBlock *doneBB = fb.createBasicBlock( "done" );
        
        bbb->SetInsertPoint( entryBB );
        bbb->CreateCondBr(
          bbb->CreateIsNotNull( bitsLValue ),
          nonNullBB,
          doneBB
          );
        
        bbb->SetInsertPoint( nonNullBB );
        llvm::Value *oneRValue = sizeAdapter->llvmConst( context, 1 );
        llvm::Value *refCountLValue = bbb->CreateStructGEP( bitsLValue, 0 );
        static const size_t numIntrinsicTypes = 2;
        llvm::Type const *intrinsicTypes[numIntrinsicTypes] =
        {
          oneRValue->getType(),
          refCountLValue->getType()
        };
        llvm::Function *intrinsic = llvm::Intrinsic::getDeclaration( mb, llvm::Intrinsic::atomic_load_add, intrinsicTypes, numIntrinsicTypes );
        FABRIC_ASSERT( intrinsic );
        bbb->CreateCall2(
          intrinsic,
          refCountLValue,
          oneRValue
          );
        bbb->CreateBr( doneBB );

        bbb->SetInsertPoint( doneBB );
        bbb->CreateRetVoid();
      }

      std::vector<llvm::Value *> args;
      args.push_back( bitsLValue );
      basicBlockBuilder->CreateCall( func, args.begin(), args.end() );
    }
    
    void VariableArrayAdapter::llvmRelease( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *bitsLValue ) const
    {
      RC::Handle<Context> context = basicBlockBuilder.getContext();
      RC::ConstHandle<SizeAdapter> sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();

      std::vector<llvm::Type const *> argTypes;
      argTypes.push_back( bitsLValue->getType() );
      llvm::FunctionType const *funcType = llvm::FunctionType::get( llvm::Type::getVoidTy( context->getLLVMContext() ), argTypes, false );
      
      llvm::AttributeWithIndex AWI[1];
      AWI[0] = llvm::AttributeWithIndex::get( ~0u, llvm::Attribute::InlineHint | llvm::Attribute::NoUnwind );
      llvm::AttrListPtr attrListPtr = llvm::AttrListPtr::get( AWI, 1 );

      ModuleBuilder &mb = basicBlockBuilder.getModuleBuilder();
        
      std::string name = "__"+getCodeName()+"_Release";
      llvm::Function *func = llvm::cast<llvm::Function>( mb->getFunction( name ) );
      if ( !func )
      {
        func = llvm::cast<llvm::Function>( mb->getOrInsertFunction( name, funcType, attrListPtr ) ); 
        func->setLinkage( llvm::GlobalValue::PrivateLinkage );
        
        FunctionBuilder fb( mb, funcType, func );
        llvm::Argument *bitsLValue = fb[0];
        bitsLValue->setName( "bitsLValue" );
        bitsLValue->addAttr( llvm::Attribute::NoCapture );
        
        BasicBlockBuilder bbb( fb );

        llvm::BasicBlock *entryBB = fb.createBasicBlock( "entry" );
        llvm::BasicBlock *nonNullBB = fb.createBasicBlock( "nonNull" );
        llvm::BasicBlock *freeBB = fb.createBasicBlock( "free" );
        llvm::BasicBlock *loopCheckBB = fb.createBasicBlock( "loopCheck" );
        llvm::BasicBlock *loopExecBB = fb.createBasicBlock( "loopExec" );
        llvm::BasicBlock *loopDoneBB = fb.createBasicBlock( "loopDone" );
        llvm::BasicBlock *doneBB = fb.createBasicBlock( "done" );

        bbb->SetInsertPoint( entryBB );
        bbb->CreateCondBr(
          bbb->CreateIsNotNull( bitsLValue ),
          nonNullBB,
          doneBB
          );

        bbb->SetInsertPoint( nonNullBB );
        llvm::Value *refCountLValue = bbb->CreateStructGEP( bitsLValue, 0 );
        llvm::Value *oneRValue = sizeAdapter->llvmConst( bbb.getContext(), 1 );
        static const size_t numIntrinsicTypes = 2;
        llvm::Type const *intrinsicTypes[numIntrinsicTypes] =
        {
          oneRValue->getType(),
          refCountLValue->getType()
        };
        llvm::Function *intrinsic = llvm::Intrinsic::getDeclaration( mb, llvm::Intrinsic::atomic_load_sub, intrinsicTypes, numIntrinsicTypes );
        FABRIC_ASSERT( intrinsic );
        llvm::Value *oldRefCountRValue = bbb->CreateCall2( intrinsic, refCountLValue, oneRValue );
        llvm::Value *shouldFreeRValue = bbb->CreateICmpEQ( oldRefCountRValue, oneRValue );
        bbb->CreateCondBr( shouldFreeRValue, freeBB, doneBB );

        bbb->SetInsertPoint( freeBB );
        llvm::Value *numMembers = bbb->CreateLoad( bbb->CreateStructGEP( bitsLValue, 2 ) );
        llvm::Value *members = bbb->CreateConstGEP2_32( bbb->CreateStructGEP( bitsLValue, 3 ), 0, 0 );
        llvm::Value *indexPtr = sizeAdapter->llvmAlloca( bbb, "index" );
        bbb->CreateStore( sizeAdapter->llvmConst( bbb.getContext(), 0 ), indexPtr );
        bbb->CreateBr( loopCheckBB );

        bbb->SetInsertPoint( loopCheckBB );
        bbb->CreateCondBr( bbb->CreateICmpEQ( bbb->CreateLoad(indexPtr), numMembers ), loopDoneBB, loopExecBB );

        bbb->SetInsertPoint( loopExecBB );
        llvm::Value *index = bbb->CreateLoad( indexPtr );
        llvm::Value *memberLValue = bbb->CreateGEP( members, index );
        llvm::Value *memberRValue = m_memberAdapter->llvmLValueToRValue( bbb, memberLValue );
        m_memberAdapter->llvmDispose( bbb, memberRValue );
        bbb->CreateStore( bbb->CreateAdd( index, sizeAdapter->llvmConst( bbb.getContext(), 1 ) ), indexPtr );
        bbb->CreateBr( loopCheckBB );

        bbb->SetInsertPoint( loopDoneBB );
        llvmCallFree( bbb, bitsLValue );
        bbb->CreateBr( doneBB );

        bbb->SetInsertPoint( doneBB );
        bbb->CreateRetVoid();
      }

      std::vector<llvm::Value *> args;
      args.push_back( bitsLValue );
      basicBlockBuilder->CreateCall( func, args.begin(), args.end() );
    }

    void VariableArrayAdapter::llvmDisposeImpl( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *lValue ) const
    {
      llvm::Value *bitsLValue = basicBlockBuilder->CreateLoad( lValue );
      llvmRelease( basicBlockBuilder, bitsLValue );
    }

    void VariableArrayAdapter::llvmDefaultAssign( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const
    {
      llvm::Value *oldBitsLValue = basicBlockBuilder->CreateLoad( dstLValue );
      llvm::Value *newBitsLValue = basicBlockBuilder->CreateLoad( srcRValue );
      llvmRetain( basicBlockBuilder, newBitsLValue );
      basicBlockBuilder->CreateStore( newBitsLValue, dstLValue );
      llvmRelease( basicBlockBuilder, oldBitsLValue );
    }

    void VariableArrayAdapter::llvmInit( BasicBlockBuilder &basicBlockBuilder, llvm::Value *lValue ) const
    {
      llvm::PointerType const *rawType = static_cast<llvm::PointerType const *>( llvmRawType( basicBlockBuilder.getContext() ) );
      basicBlockBuilder->CreateStore(
        llvm::ConstantPointerNull::get( rawType ),
        lValue
        );
    }
    
    llvm::Constant *VariableArrayAdapter::llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      return llvm::ConstantPointerNull::get( static_cast<llvm::PointerType const *>( llvmRawType( basicBlockBuilder.getContext() ) ) );
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
