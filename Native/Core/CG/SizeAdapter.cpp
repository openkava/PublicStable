#include "SizeAdapter.h"
#include "BooleanAdapter.h"
#include "ByteAdapter.h"
#include "IntegerAdapter.h"
#include "SizeAdapter.h"
#include "FloatAdapter.h"
#include "ConstStringAdapter.h"
#include "StringAdapter.h"
#include "OpaqueAdapter.h"
#include "Manager.h"
#include "ModuleBuilder.h"
#include "FunctionBuilder.h"
#include "BasicBlockBuilder.h"
#include "OverloadNames.h"

#include <Fabric/Core/RT/NumericDesc.h>
#include <Fabric/Core/Util/Format.h>

namespace Fabric
{
  namespace CG
  {
    SizeAdapter::SizeAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::NumericDesc> const &sizeDesc )
      : SimpleAdapter( manager, sizeDesc )
      , m_sizeDesc( sizeDesc )
    {
      setLLVMType( llvmSizeType() );
    }
       
    void SizeAdapter::llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const
    {
      if ( moduleBuilder.contains( getCodeName(), buildFunctions ) )
        return;
      
      RC::ConstHandle<BooleanAdapter> booleanAdapter = getManager()->getBooleanAdapter();
      RC::ConstHandle<ByteAdapter> byteAdapter = getManager()->getByteAdapter();
      RC::ConstHandle<IntegerAdapter> integerAdapter = getManager()->getIntegerAdapter();
      RC::ConstHandle<FloatAdapter> scalarAdapter = getManager()->getFP32Adapter();
      RC::ConstHandle<StringAdapter> stringAdapter = getManager()->getStringAdapter();
      RC::ConstHandle<OpaqueAdapter> dataAdapter = getManager()->getDataAdapter();
      
      integerAdapter->llvmPrepareModule( moduleBuilder, buildFunctions );
      
      {
        std::string name = constructOverloadName( booleanAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "booleanLValue", booleanAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "sizeRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *booleanLValue = functionBuilder[0];
          llvm::Value *sizeRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *booleanRValue = basicBlockBuilder->CreateIsNotNull( sizeRValue );
          booleanAdapter->llvmAssign( basicBlockBuilder, booleanLValue, booleanRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = constructOverloadName( byteAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "byteLValue", byteAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "sizeRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *byteLValue = functionBuilder[0];
          llvm::Value *sizeRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *byteRValue = basicBlockBuilder->CreateTrunc( sizeRValue, byteAdapter->llvmRType() );
          byteAdapter->llvmAssign( basicBlockBuilder, byteLValue, byteRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = constructOverloadName( integerAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "integerLValue", integerAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "sizeRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *integerLValue = functionBuilder[0];
          llvm::Value *sizeRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *integerRValue = basicBlockBuilder->CreateTrunc( sizeRValue, integerAdapter->llvmRType() );
          integerAdapter->llvmAssign( basicBlockBuilder, integerLValue, integerRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = constructOverloadName( scalarAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "scalarLValue", scalarAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "sizeRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *scalarLValue = functionBuilder[0];
          llvm::Value *sizeRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *scalarRValue = basicBlockBuilder->CreateUIToFP( sizeRValue, scalarAdapter->llvmRType() );
          scalarAdapter->llvmAssign( basicBlockBuilder, scalarLValue, scalarRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      {
        std::string name = constructOverloadName( stringAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "stringLValue", stringAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "sizeRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *stringLValue = functionBuilder[0];
          llvm::Value *sizeRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *sizeLValue = llvmRValueToLValue( basicBlockBuilder, sizeRValue );
          llvm::Value *stringRValue = stringAdapter->llvmCallCast( basicBlockBuilder, this, sizeLValue );
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
        std::string name = uniOpOverloadName( UNI_OP_BIT_NOT, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "rValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *rValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *notRValue = basicBlockBuilder->CreateNot( rValue );
          basicBlockBuilder->CreateRet( notRValue );
        }
      }
      
      {
        std::string name = uniOpOverloadName( UNI_OP_PRE_INC, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "lValue", this, USAGE_LVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *lValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *preRValue = basicBlockBuilder->CreateLoad( lValue );
          llvm::Value *postRValue = basicBlockBuilder->CreateAdd( preRValue, llvm::ConstantInt::get( llvmRType(), 1, false ) );
          basicBlockBuilder->CreateStore( postRValue, lValue );
          basicBlockBuilder->CreateRet( postRValue );
        }
      }
      
      {
        std::string name = uniOpOverloadName( UNI_OP_PRE_DEC, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "lValue", this, USAGE_LVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *lValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *preRValue = basicBlockBuilder->CreateLoad( lValue );
          llvm::Value *postRValue = basicBlockBuilder->CreateSub( preRValue, llvm::ConstantInt::get( llvmRType(), 1, false ) );
          basicBlockBuilder->CreateStore( postRValue, lValue );
          basicBlockBuilder->CreateRet( postRValue );
        }
      }
      
      {
        std::string name = uniOpOverloadName( UNI_OP_POST_INC, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "lValue", this, USAGE_LVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *lValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *preRValue = basicBlockBuilder->CreateLoad( lValue );
          llvm::Value *postRValue = basicBlockBuilder->CreateAdd( preRValue, llvm::ConstantInt::get( llvmRType(), 1, false ) );
          basicBlockBuilder->CreateStore( postRValue, lValue );
          basicBlockBuilder->CreateRet( preRValue );
        }
      }
      
      {
        std::string name = uniOpOverloadName( UNI_OP_POST_DEC, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "lValue", this, USAGE_LVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          llvm::Value *lValue = functionBuilder[0];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *preRValue = basicBlockBuilder->CreateLoad( lValue );
          llvm::Value *postRValue = basicBlockBuilder->CreateSub( preRValue, llvm::ConstantInt::get( llvmRType(), 1, false ) );
          basicBlockBuilder->CreateStore( postRValue, lValue );
          basicBlockBuilder->CreateRet( preRValue );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateAdd( lhsRValue, rhsRValue );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateSub( lhsRValue, rhsRValue );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateMul( lhsRValue, rhsRValue );
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
          
          llvm::BasicBlock *entryBB = functionBuilder.createBasicBlock( "entry" );
          llvm::BasicBlock *nonZeroBB = functionBuilder.createBasicBlock( "nonZero" );
          llvm::BasicBlock *zeroBB = functionBuilder.createBasicBlock( "zero" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *isNonZeroRValue = basicBlockBuilder->CreateIsNotNull( rhsRValue );
          basicBlockBuilder->CreateCondBr( isNonZeroRValue, nonZeroBB, zeroBB );
          
          basicBlockBuilder->SetInsertPoint( nonZeroBB );
          llvm::Value *resultRValue = basicBlockBuilder->CreateUDiv( lhsRValue, rhsRValue );
          basicBlockBuilder->CreateRet( resultRValue );
          
          basicBlockBuilder->SetInsertPoint( zeroBB );
          std::string errorMsg = "KL: "+getUserName()+" division by zero";
          RC::ConstHandle<ConstStringAdapter> errorConstStringAdapter = getManager()->getConstStringAdapter( errorMsg.length() );
          ExprValue errorExprValue( errorConstStringAdapter, USAGE_RVALUE, errorConstStringAdapter->llvmConst( basicBlockBuilder, errorMsg ) );
          llvm::Value *errorStringRValue = stringAdapter->llvmCast( basicBlockBuilder, errorExprValue );
          stringAdapter->llvmReport( basicBlockBuilder, errorStringRValue );
          stringAdapter->llvmRelease( basicBlockBuilder, errorStringRValue );
          llvm::Value *defaultRValue = llvmDefaultRValue( basicBlockBuilder );
          basicBlockBuilder->CreateRet( defaultRValue );
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
          
          llvm::BasicBlock *entryBB = functionBuilder.createBasicBlock( "entry" );
          llvm::BasicBlock *nonZeroBB = functionBuilder.createBasicBlock( "nonZero" );
          llvm::BasicBlock *zeroBB = functionBuilder.createBasicBlock( "zero" );
          
          basicBlockBuilder->SetInsertPoint( entryBB );
          llvm::Value *isNonZeroRValue = basicBlockBuilder->CreateIsNotNull( rhsRValue );
          basicBlockBuilder->CreateCondBr( isNonZeroRValue, nonZeroBB, zeroBB );
          
          basicBlockBuilder->SetInsertPoint( nonZeroBB );
          llvm::Value *resultRValue = basicBlockBuilder->CreateURem( lhsRValue, rhsRValue );
          basicBlockBuilder->CreateRet( resultRValue );
          
          basicBlockBuilder->SetInsertPoint( zeroBB );
          std::string errorMsg = "KL: "+getUserName()+" division by zero";
          RC::ConstHandle<ConstStringAdapter> errorConstStringAdapter = getManager()->getConstStringAdapter( errorMsg.length() );
          ExprValue errorExprValue( errorConstStringAdapter, USAGE_RVALUE, errorConstStringAdapter->llvmConst( basicBlockBuilder, errorMsg ) );
          llvm::Value *errorStringRValue = stringAdapter->llvmCast( basicBlockBuilder, errorExprValue );
          stringAdapter->llvmReport( basicBlockBuilder, errorStringRValue );
          stringAdapter->llvmRelease( basicBlockBuilder, errorStringRValue );
          llvm::Value *defaultRValue = llvmDefaultRValue( basicBlockBuilder );
          basicBlockBuilder->CreateRet( defaultRValue );
        }
      }
      
      {
        std::string name = binOpOverloadName( BIN_OP_BIT_OR, this, this );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateOr( lhsRValue, rhsRValue );
          basicBlockBuilder->CreateRet( resultRValue );
        }
      }
      
      {
        std::string name = binOpOverloadName( BIN_OP_BIT_AND, this, this );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateAnd( lhsRValue, rhsRValue );
          basicBlockBuilder->CreateRet( resultRValue );
        }
      }
      
      {
        std::string name = binOpOverloadName( BIN_OP_BIT_XOR, this, this );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateXor( lhsRValue, rhsRValue );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateICmpEQ( lhsRValue, rhsRValue );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateICmpNE( lhsRValue, rhsRValue );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateICmpUGT( lhsRValue, rhsRValue );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateICmpUGE( lhsRValue, rhsRValue );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateICmpULT( lhsRValue, rhsRValue );
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
          llvm::Value *resultRValue = basicBlockBuilder->CreateICmpULE( lhsRValue, rhsRValue );
          basicBlockBuilder->CreateRet( resultRValue );
        }
      }
      
      {
        std::string name = methodOverloadName( "dataSize", this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "selfRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( this, USAGE_RVALUE ), params );
        if ( buildFunctions )
        {
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *dataSizeRValue = llvm::ConstantInt::get( llvmRType(), getDesc()->getSize() );
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

    llvm::Constant *SizeAdapter::llvmConst( size_t value ) const
    {
      return llvm::ConstantInt::get( llvmRawType(), value, false );
    }
    
    std::string SizeAdapter::toString( void const *data ) const
    {
      return m_sizeDesc->toString( data );
    }
  }; // namespace RT
}; // namespace FABRIC
