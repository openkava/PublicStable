#include "ScalarAdapter.h"
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

#include <Fabric/Core/RT/ScalarDesc.h>
#include <Fabric/Core/RT/ScalarImpl.h>
#include <Fabric/Core/Util/Format.h>

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Intrinsics.h>

namespace Fabric
{
  namespace CG
  {
    ScalarAdapter::ScalarAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::ScalarDesc> const &scalarDesc )
      : SimpleAdapter( manager, scalarDesc )
      , m_scalarDesc( scalarDesc )
    {
      setLLVMType( llvm::Type::getFloatTy( manager->getLLVMContext() ) );
    }

    void ScalarAdapter::llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const
    {
      if ( moduleBuilder.contains( getCodeName() ) )
        return;
      
      RC::ConstHandle<BooleanAdapter> booleanAdapter = getManager()->getBooleanAdapter();
      RC::ConstHandle<ByteAdapter> byteAdapter = getManager()->getByteAdapter();
      RC::ConstHandle<IntegerAdapter> integerAdapter = getManager()->getIntegerAdapter();
      RC::ConstHandle<SizeAdapter> sizeAdapter = getManager()->getSizeAdapter();
      RC::ConstHandle<StringAdapter> stringAdapter = getManager()->getStringAdapter();
      RC::ConstHandle<OpaqueAdapter> dataAdapter = getManager()->getDataAdapter();
      
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
          llvm::Value *booleanRValue = basicBlockBuilder->CreateFCmpONE( scalarRValue, llvmConst( 0.0 ) );
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
          llvm::Value *byteRValue = basicBlockBuilder->CreateFPToUI( scalarRValue, byteAdapter->llvmRType() );
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
          llvm::Value *integerRValue = basicBlockBuilder->CreateFPToSI( scalarRValue, integerAdapter->llvmRType() );
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
          llvm::Value *sizeRValue = basicBlockBuilder->CreateFPToUI( scalarRValue, sizeAdapter->llvmRType() );
          sizeAdapter->llvmAssign( basicBlockBuilder, sizeLValue, sizeRValue );
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
        std::string name = "sin";
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          static const size_t numIntrinsicTypes = 1;
          llvm::Type const *intrinsicTypes[numIntrinsicTypes] =
          {
            llvmRType()
          };
          llvm::Function *intrinsic = llvm::Intrinsic::getDeclaration( moduleBuilder, llvm::Intrinsic::sin, intrinsicTypes, numIntrinsicTypes );
          FABRIC_ASSERT( intrinsic );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreateCall( intrinsic, x ) );
        }
      }

      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = "cos";
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          static const size_t numIntrinsicTypes = 1;
          llvm::Type const *intrinsicTypes[numIntrinsicTypes] =
          {
            llvmRType()
          };
          llvm::Function *intrinsic = llvm::Intrinsic::getDeclaration( moduleBuilder, llvm::Intrinsic::cos, intrinsicTypes, numIntrinsicTypes );
          FABRIC_ASSERT( intrinsic );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreateCall( intrinsic, x ) );
        }
      }

      {
        std::vector< FunctionParam > tanfParams;
        tanfParams.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        FunctionBuilder tanfFunctionBuilder( moduleBuilder, "tanf", ExprType( this, USAGE_RVALUE ), tanfParams, false );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = "tan";
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, &name );
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
        FunctionBuilder logfFunctionBuilder( moduleBuilder, "logf", ExprType( this, USAGE_RVALUE ), logfParams, false );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = "log";
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, &name );
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
        FunctionBuilder acosfFunctionBuilder( moduleBuilder, "acosf", ExprType( this, USAGE_RVALUE ), acosfParams, false );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = "acos";
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, &name );
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
        FunctionBuilder asinfFunctionBuilder( moduleBuilder, "asinf", ExprType( this, USAGE_RVALUE ), asinfParams, false );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = "asin";
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, &name );
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
        FunctionBuilder atanfFunctionBuilder( moduleBuilder, "atanf", ExprType( this, USAGE_RVALUE ), atanfParams, false );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = "atan";
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, &name );
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
        FunctionBuilder atan2fFunctionBuilder( moduleBuilder, "atan2f", ExprType( this, USAGE_RVALUE ), atan2fParams, false );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "y", this, USAGE_RVALUE ) );
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = "atan2";
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, &name );
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
        std::string name = "sqrt";
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          static const size_t numIntrinsicTypes = 1;
          llvm::Type const *intrinsicTypes[numIntrinsicTypes] =
          {
            llvmRType()
          };
          llvm::Function *intrinsic = llvm::Intrinsic::getDeclaration( moduleBuilder, llvm::Intrinsic::sqrt, intrinsicTypes, numIntrinsicTypes );
          FABRIC_ASSERT( intrinsic );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreateCall( intrinsic, x ) );
        }
      }

      {
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = "abs";
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *xAsInt32 = basicBlockBuilder->CreateBitCast( x, integerAdapter->llvmRType() );
          llvm::Value *mask = integerAdapter->llvmConst( (int32_t)0x7FFFFFFF );
          llvm::Value *maskedXAsInt32 = basicBlockBuilder->CreateAnd( xAsInt32, mask );
          llvm::Value *maskedX = basicBlockBuilder->CreateBitCast( maskedXAsInt32, llvmRType() );
          basicBlockBuilder->CreateRet( maskedX );
        }
      }

      {
        std::vector< FunctionParam > roundfParams;
        roundfParams.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        FunctionBuilder roundfFunctionBuilder( moduleBuilder, "roundf", ExprType( this, USAGE_RVALUE ), roundfParams );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = "round";
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, &name );
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
        FunctionBuilder ceilfFunctionBuilder( moduleBuilder, "ceilf", ExprType( this, USAGE_RVALUE ), ceilfParams );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = "ceil";
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, &name );
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
        FunctionBuilder floorfFunctionBuilder( moduleBuilder, "floorf", ExprType( this, USAGE_RVALUE ), floorfParams, false );

        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "x", this, USAGE_RVALUE ) );
        std::string name = "floor";
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, &name );
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
        std::string name = "pow";
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params, &name );
        if ( buildFunctions )
        {
          llvm::Value *x = functionBuilder[0];
          llvm::Value *y = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          static const size_t numIntrinsicTypes = 1;
          llvm::Type const *intrinsicTypes[numIntrinsicTypes] =
          {
            llvmRType()
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
        params.push_back( FunctionParam( "selfRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( sizeAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *selfRValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *dataSizeRValue = llvm::ConstantInt::get( sizeAdapter->llvmRType(), getDesc()->getSize() );
          basicBlockBuilder->CreateRet( dataSizeRValue );
        }
      }
      
      {
        std::string name = methodOverloadName( "data", this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "selfLValue", this, USAGE_LVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( dataAdapter, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *selfLValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          basicBlockBuilder->CreateRet( basicBlockBuilder->CreatePointerCast( selfLValue, dataAdapter->llvmRType() ) );
        }
      }
    }
    
    llvm::Constant *ScalarAdapter::llvmConst( float value ) const
    {
      return llvm::ConstantFP::get( llvmRawType(), value );
    }
    
    llvm::Constant *ScalarAdapter::llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      return llvm::ConstantFP::get( llvmRType(), m_scalarDesc->getValue( m_scalarDesc->getDefaultData() ) );
    }
    
    std::string ScalarAdapter::toString( void const *data ) const
    {
      return _( m_scalarDesc->getValue( data ) );
    }
  };
};
