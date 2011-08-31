#include "FloatAdapter.h"
#include "BooleanAdapter.h"
#include "ByteAdapter.h"
#include "IntegerAdapter.h"
#include "SizeAdapter.h"
#include "StringAdapter.h"
#include "OpaqueAdapter.h"
#include "Manager.h"
#include "ModuleBuilder.h"
#include "FunctionBuilder.h"
#include "BasicBlockBuilder.h"
#include "OverloadNames.h"

#include <Fabric/Core/RT/FloatDesc.h>
#include <Fabric/Core/RT/FloatImpl.h>
#include <Fabric/Core/Util/Format.h>

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Intrinsics.h>

namespace Fabric
{
  namespace CG
  {
    FloatAdapter::FloatAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::FloatDesc> const &floatDesc )
      : SimpleAdapter( manager, floatDesc )
      , m_floatDesc( floatDesc )
    {
    }
    
    llvm::Type const *FloatAdapter::buildLLVMRawType( RC::Handle<Context> const &context ) const
    {
      llvm::Type const *result = 0;
      switch ( m_floatDesc->getSize() )
      {
        case 4:
          result = llvm::Type::getFloatTy( context->getLLVMContext() );
          break;
        case 8:
          result = llvm::Type::getDoubleTy( context->getLLVMContext() );
          break;
        default:
          FABRIC_ASSERT( false );
          result = 0;
          break;
      }
      return result;
    }

    void FloatAdapter::llvmCompileToModule( ModuleBuilder &moduleBuilder ) const
    {
      if ( moduleBuilder.haveCompiledToModule( getCodeName() ) )
        return;
      
      RC::Handle<Context> context = moduleBuilder.getContext();
      
      RC::ConstHandle<BooleanAdapter> booleanAdapter = getManager()->getBooleanAdapter();
      booleanAdapter->llvmCompileToModule( moduleBuilder );
      RC::ConstHandle<ByteAdapter> byteAdapter = getManager()->getByteAdapter();
      byteAdapter->llvmCompileToModule( moduleBuilder );
      RC::ConstHandle<IntegerAdapter> integerAdapter = getManager()->getIntegerAdapter();
      integerAdapter->llvmCompileToModule( moduleBuilder );
      RC::ConstHandle<SizeAdapter> sizeAdapter = getManager()->getSizeAdapter();
      sizeAdapter->llvmCompileToModule( moduleBuilder );
      RC::ConstHandle<StringAdapter> stringAdapter = getManager()->getStringAdapter();
      stringAdapter->llvmCompileToModule( moduleBuilder );
      RC::ConstHandle<OpaqueAdapter> dataAdapter = getManager()->getDataAdapter();
      dataAdapter->llvmCompileToModule( moduleBuilder );
      
      static const bool buildFunctions = true;
      
      {
        std::string name = constructOverloadName( booleanAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "booleanLValue", booleanAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "scalarRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *booleanLValue = functionBuilder[0];
          llvm::Value *scalarRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *booleanRValue = basicBlockBuilder->CreateFCmpONE( scalarRValue, llvmConst( context, 0.0 ) );
          booleanAdapter->llvmAssign( basicBlockBuilder, booleanLValue, booleanRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = constructOverloadName( byteAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "byteLValue", byteAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "scalarRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *byteLValue = functionBuilder[0];
          llvm::Value *scalarRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *byteRValue = basicBlockBuilder->CreateFPToUI( scalarRValue, byteAdapter->llvmRType( context ) );
          byteAdapter->llvmAssign( basicBlockBuilder, byteLValue, byteRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = constructOverloadName( integerAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "integerLValue", integerAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "scalarRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *integerLValue = functionBuilder[0];
          llvm::Value *scalarRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *integerRValue = basicBlockBuilder->CreateFPToSI( scalarRValue, integerAdapter->llvmRType( context ) );
          integerAdapter->llvmAssign( basicBlockBuilder, integerLValue, integerRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = constructOverloadName( sizeAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "sizeLValue", sizeAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "scalarRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *sizeLValue = functionBuilder[0];
          llvm::Value *scalarRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *sizeRValue = basicBlockBuilder->CreateFPToUI( scalarRValue, sizeAdapter->llvmRType( context ) );
          sizeAdapter->llvmAssign( basicBlockBuilder, sizeLValue, sizeRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      if ( m_floatDesc->getSize() != 4 )
      {
        RC::ConstHandle<FloatAdapter> fp32Adapter = getManager()->getFP32Adapter();
        
        std::string name = constructOverloadName( fp32Adapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "fp32LValue", fp32Adapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "scalarRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *fp32LValue = functionBuilder[0];
          llvm::Value *scalarRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *fp32RValue = basicBlockBuilder->CreateFPTrunc( scalarRValue, fp32Adapter->llvmRType( context ) );
          sizeAdapter->llvmAssign( basicBlockBuilder, fp32LValue, fp32RValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      if ( m_floatDesc->getSize() != 8 )
      {
        RC::ConstHandle<FloatAdapter> fp64Adapter = getManager()->getFP64Adapter();
        
        std::string name = constructOverloadName( fp64Adapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "fp64LValue", fp64Adapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "scalarRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *fp64LValue = functionBuilder[0];
          llvm::Value *scalarRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *fp64RValue = basicBlockBuilder->CreateFPExt( scalarRValue, fp64Adapter->llvmRType( context ) );
          sizeAdapter->llvmAssign( basicBlockBuilder, fp64LValue, fp64RValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = constructOverloadName( stringAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "stringLValue", stringAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "scalarRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *stringLValue = functionBuilder[0];
          llvm::Value *scalarRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *scalarLValue = llvmRValueToLValue( basicBlockBuilder, scalarRValue );
          llvm::Value *stringRValue = stringAdapter->llvmCallCast( basicBlockBuilder, this, scalarLValue );
          stringAdapter->llvmAssign( basicBlockBuilder, stringLValue, stringRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = uniOpOverloadName( UNI_OP_POS, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "rValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *rValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet( rValue );
        }
      }
      
      {
        std::string name = uniOpOverloadName( UNI_OP_NEG, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "rValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *rValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *negRValue = basicBlockBuilder->CreateFNeg( rValue );
          basicBlockBuilder->CreateRet( negRValue );
        }
      }
      
      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = internalFunctionNameForType("sin");
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, false, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          static const size_t numIntrinsicTypes = 1;
          llvm::Type const *intrinsicTypes[numIntrinsicTypes] =
          {
            llvmRType( context )
          };
          llvm::Function *intrinsic = llvm::Intrinsic::getDeclaration( moduleBuilder, llvm::Intrinsic::sin, intrinsicTypes, numIntrinsicTypes );
          FABRIC_ASSERT( intrinsic );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreateCall( intrinsic, x ) );
        }
      }

      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = internalFunctionNameForType("cos");
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, false, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          static const size_t numIntrinsicTypes = 1;
          llvm::Type const *intrinsicTypes[numIntrinsicTypes] =
          {
            llvmRType( context )
          };
          llvm::Function *intrinsic = llvm::Intrinsic::getDeclaration( moduleBuilder, llvm::Intrinsic::cos, intrinsicTypes, numIntrinsicTypes );
          FABRIC_ASSERT( intrinsic );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreateCall( intrinsic, x ) );
        }
      }

      {
        std::vector< FunctionParam > tanfParams;
        tanfParams.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        FunctionBuilder tanfFunctionBuilder( moduleBuilder, externalFunctionNameForType("tan"), ExprType( this, USAGE_RVALUE ), tanfParams, true );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = internalFunctionNameForType("tan");
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, false, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreateCall( tanfFunctionBuilder.getLLVMFunction(), x ) );
        }
      }

      {
        std::vector< FunctionParam > logfParams;
        logfParams.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        FunctionBuilder logfFunctionBuilder( moduleBuilder, externalFunctionNameForType("log"), ExprType( this, USAGE_RVALUE ), logfParams, true );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = internalFunctionNameForType("log");
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, false, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreateCall( logfFunctionBuilder.getLLVMFunction(), x ) );
        }
      }

      {
        std::vector< FunctionParam > acosfParams;
        acosfParams.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        FunctionBuilder acosfFunctionBuilder( moduleBuilder, externalFunctionNameForType("acos"), ExprType( this, USAGE_RVALUE ), acosfParams, true );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = internalFunctionNameForType("acos");
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, false, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreateCall( acosfFunctionBuilder.getLLVMFunction(), x ) );
        }
      }

      {
        std::vector< FunctionParam > asinfParams;
        asinfParams.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        FunctionBuilder asinfFunctionBuilder( moduleBuilder, externalFunctionNameForType("asin"), ExprType( this, USAGE_RVALUE ), asinfParams, true );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = internalFunctionNameForType("asin");
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, false, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreateCall( asinfFunctionBuilder.getLLVMFunction(), x ) );
        }
      }

      {
        std::vector< FunctionParam > atanfParams;
        atanfParams.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        FunctionBuilder atanfFunctionBuilder( moduleBuilder, externalFunctionNameForType("atan"), ExprType( this, USAGE_RVALUE ), atanfParams, true );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = internalFunctionNameForType("atan");
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, false, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreateCall( atanfFunctionBuilder.getLLVMFunction(), x ) );
        }
      }

      {
        std::vector< FunctionParam > atan2fParams;
        atan2fParams.push_back( FunctionParam( "y", this, USAGE_RVALUE ) );
        atan2fParams.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        FunctionBuilder atan2fFunctionBuilder( moduleBuilder, externalFunctionNameForType("atan2"), ExprType( this, USAGE_RVALUE ), atan2fParams, true );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "y", this, USAGE_RVALUE ) );
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = internalFunctionNameForType("atan2");
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, false, &name );
        if ( buildFunctions )
        {
          llvm::Value *y = functionBuilder[0];
          llvm::Value *x = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreateCall2( atan2fFunctionBuilder.getLLVMFunction(), y, x ) );
        }
      }

      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = internalFunctionNameForType("sqrt");
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, false, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          static const size_t numIntrinsicTypes = 1;
          llvm::Type const *intrinsicTypes[numIntrinsicTypes] =
          {
            llvmRType( context )
          };
          llvm::Function *intrinsic = llvm::Intrinsic::getDeclaration( moduleBuilder, llvm::Intrinsic::sqrt, intrinsicTypes, numIntrinsicTypes );
          FABRIC_ASSERT( intrinsic );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreateCall( intrinsic, x ) );
        }
      }

      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = internalFunctionNameForType("abs");
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, false, &name );
        if ( buildFunctions )
        {
          llvm::Type const *integerTypeOfSameWidth = 0;
          switch ( m_floatDesc->getSize() )
          {
            case 4:
              integerTypeOfSameWidth = llvm::Type::getInt32Ty( context->getLLVMContext() );
              break;
            case 8:
              integerTypeOfSameWidth = llvm::Type::getInt64Ty( context->getLLVMContext() );
              break;
            default:
              FABRIC_ASSERT( false );
              integerTypeOfSameWidth = 0;
              break;
          }
          
          llvm::Value *x = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *xAsInt = basicBlockBuilder->CreateBitCast( x, integerTypeOfSameWidth );
          llvm::Value *zero = llvm::ConstantInt::get( integerTypeOfSameWidth, 0 );
          llvm::Value *notZero = basicBlockBuilder->CreateNot( zero );
          llvm::Value *mask = basicBlockBuilder->CreateLShr( notZero, llvm::ConstantInt::get( integerTypeOfSameWidth, 1 ) );
          llvm::Value *maskedXAsInt = basicBlockBuilder->CreateAnd( xAsInt, mask );
          llvm::Value *maskedX = basicBlockBuilder->CreateBitCast( maskedXAsInt, llvmRType( context ) );
          basicBlockBuilder->CreateRet( maskedX );
        }
      }

      {
        std::vector< FunctionParam > roundfParams;
        roundfParams.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        FunctionBuilder roundfFunctionBuilder( moduleBuilder, externalFunctionNameForType("round"), ExprType( this, USAGE_RVALUE ), roundfParams, true );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = internalFunctionNameForType("round");
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, false, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreateCall( roundfFunctionBuilder.getLLVMFunction(), x ) );
        }
      }
      
      {
        std::vector< FunctionParam > ceilfParams;
        ceilfParams.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        FunctionBuilder ceilfFunctionBuilder( moduleBuilder, externalFunctionNameForType("ceil"), ExprType( this, USAGE_RVALUE ), ceilfParams, true );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = internalFunctionNameForType("ceil");
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, false, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreateCall( ceilfFunctionBuilder.getLLVMFunction(), x ) );
        }
      }
      
      {
        std::vector< FunctionParam > floorfParams;
        floorfParams.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        FunctionBuilder floorfFunctionBuilder( moduleBuilder, externalFunctionNameForType("floor"), ExprType( this, USAGE_RVALUE ), floorfParams, true );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = internalFunctionNameForType("floor");
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, false, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreateCall( floorfFunctionBuilder.getLLVMFunction(), x ) );
        }
      }
      
      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        params.push_back( FunctionParam( "y", this, USAGE_RVALUE ) );
        std::string name = internalFunctionNameForType("pow");
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, false, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          llvm::Value *y = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          static const size_t numIntrinsicTypes = 1;
          llvm::Type const *intrinsicTypes[numIntrinsicTypes] =
          {
            llvmRType( context )
          };
          llvm::Function *intrinsic = llvm::Intrinsic::getDeclaration( moduleBuilder, llvm::Intrinsic::pow, intrinsicTypes, numIntrinsicTypes );
          FABRIC_ASSERT( intrinsic );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreateCall2( intrinsic, x, y ) );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateFAdd( lhsRValue, rhsRValue );
          basicBlockBuilder->CreateRet( resultRValue );
        }
      }
      
      {
        std::string name = binOpOverloadName( BIN_OP_SUB, this, this );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateFSub( lhsRValue, rhsRValue );
          basicBlockBuilder->CreateRet( resultRValue );
        }
      }
      
      {
        std::string name = binOpOverloadName( BIN_OP_MUL, this, this );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateFMul( lhsRValue, rhsRValue );
          basicBlockBuilder->CreateRet( resultRValue );
        }
      }
      
      {
        std::string name = binOpOverloadName( BIN_OP_DIV, this, this );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateFDiv( lhsRValue, rhsRValue );
          basicBlockBuilder->CreateRet( resultRValue );
        }
      }
      
      {
        std::string name = binOpOverloadName( BIN_OP_MOD, this, this );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateFRem( lhsRValue, rhsRValue );
          basicBlockBuilder->CreateRet( resultRValue );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateFCmpOEQ( lhsRValue, rhsRValue );
          basicBlockBuilder->CreateRet( resultRValue );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateFCmpONE( lhsRValue, rhsRValue );
          basicBlockBuilder->CreateRet( resultRValue );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateFCmpOGT( lhsRValue, rhsRValue );
          basicBlockBuilder->CreateRet( resultRValue );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateFCmpOGE( lhsRValue, rhsRValue );
          basicBlockBuilder->CreateRet( resultRValue );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateFCmpOLT( lhsRValue, rhsRValue );
          basicBlockBuilder->CreateRet( resultRValue );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateFCmpOLE( lhsRValue, rhsRValue );
          basicBlockBuilder->CreateRet( resultRValue );
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
          llvm::Value *dataSizeRValue = llvm::ConstantInt::get( sizeAdapter->llvmRType( context ), getDesc()->getSize() );
          basicBlockBuilder->CreateRet( dataSizeRValue );
        }
      }
      
      {
        std::string name = methodOverloadName( "data", this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "thisLValue", this, USAGE_LVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( dataAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *thisLValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreatePointerCast( thisLValue, dataAdapter->llvmRType( context ) ) );
        }
      }
    }
    
    llvm::Constant *FloatAdapter::llvmConst( RC::Handle<Context> const &context, double value ) const
    {
      return llvm::ConstantFP::get( llvmRawType( context ), value );
    }
    
    llvm::Constant *FloatAdapter::llvmConst( RC::Handle<Context> const &context, std::string const &valueString ) const
    {
      return llvm::ConstantFP::get( llvmRawType( context ), valueString );
    }
    
    llvm::Constant *FloatAdapter::llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      return llvm::ConstantFP::get( llvmRType( basicBlockBuilder.getContext() ), 0.0 );
    }
    
    std::string FloatAdapter::toString( void const *data ) const
    {
      return m_floatDesc->toString( data );
    }

    std::string FloatAdapter::internalFunctionNameForType( std::string const &externalFunctionName ) const
    {
      switch ( m_floatDesc->getSize() )
      {
        case 4: return externalFunctionName;
        case 8: return externalFunctionName + "64";
        default:
          FABRIC_ASSERT( false );
          return "";
      }
    }

    std::string FloatAdapter::externalFunctionNameForType( std::string const &externalFunctionName ) const
    {
      switch ( m_floatDesc->getSize() )
      {
        case 4: return "fp32_" + externalFunctionName;
        case 8: return "fp64_" + externalFunctionName;
        default:
          FABRIC_ASSERT( false );
          return "";
      }
    }
  };
};
