/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "Adapter.h"
#include "Context.h"
#include "IntegerAdapter.h"
#include "Manager.h"
#include "ModuleBuilder.h"
#include "FunctionBuilder.h"
#include "BasicBlockBuilder.h"
#include "OpTypes.h"
#include "OverloadNames.h"

#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/RT/Impl.h>
#include <Fabric/Core/RT/Manager.h>

#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Intrinsics.h>

namespace Fabric
{
  namespace CG
  {
    Adapter::Adapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::Desc> const &desc, Flags flags )
      : m_manager( manager )
      , m_desc( desc )
      , m_flags( flags )
      , m_codeName( desc->getImpl()->getCodeName() )
    {
    }
    
    RT::ImplType Adapter::getType() const
    {
      return m_desc->getType();
    }

    RC::ConstHandle<Manager> Adapter::getManager() const
    {
      return m_manager.makeStrong();
    }
    
    RC::ConstHandle<RT::Manager> Adapter::getRTManager() const
    {
      return getManager()->getRTManager();
    }
    
    RC::ConstHandle<RT::Desc> Adapter::getDesc() const
    {
      return m_desc;
    }
  
    RC::ConstHandle<RT::Impl> Adapter::getImpl() const
    {
      return getDesc()->getImpl();
    }
  
    std::string const &Adapter::getUserName() const
    {
      return getDesc()->getName();
    }
    
    llvm::Value *Adapter::llvmCallMalloc( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *size ) const
    {
      
      RC::ConstHandle< IntegerAdapter > integerAdapter = basicBlockBuilder.getManager()->getIntegerAdapter();

      std::vector< llvm::Type const * > argTypes;
      argTypes.push_back( integerAdapter->llvmRType( basicBlockBuilder.getContext() ) );
      llvm::FunctionType const *funcType = llvm::FunctionType::get( basicBlockBuilder->getInt8PtrTy(), argTypes, false );

      llvm::AttributeWithIndex AWI[1];
      AWI[0] = llvm::AttributeWithIndex::get( ~0u, llvm::Attribute::NoUnwind );
      llvm::AttrListPtr attrListPtr = llvm::AttrListPtr::get( AWI, 1 );

      llvm::Function *func = llvm::cast<llvm::Function>( basicBlockBuilder.getModuleBuilder()->getOrInsertFunction( "malloc", funcType, attrListPtr ) ); 
      
      return basicBlockBuilder->CreateCall( func, size );
    }
    
    llvm::Value *Adapter::llvmCallRealloc( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *data, llvm::Value *newSize ) const
    {
      RC::ConstHandle< IntegerAdapter > integerAdapter = basicBlockBuilder.getManager()->getIntegerAdapter();

      std::vector< llvm::Type const * > argTypes;
      argTypes.push_back( basicBlockBuilder->getInt8PtrTy() );
      argTypes.push_back( integerAdapter->llvmRType( basicBlockBuilder.getContext() ) );
      llvm::FunctionType const *funcType = llvm::FunctionType::get( basicBlockBuilder->getInt8PtrTy(), argTypes, false );

      llvm::AttributeWithIndex AWI[1];
      AWI[0] = llvm::AttributeWithIndex::get( ~0u, llvm::Attribute::NoUnwind );
      llvm::AttrListPtr attrListPtr = llvm::AttrListPtr::get( AWI, 1 );

      llvm::Function *func = llvm::cast<llvm::Function>( basicBlockBuilder.getModuleBuilder()->getOrInsertFunction( "realloc", funcType, attrListPtr ) ); 
      
      return basicBlockBuilder->CreateCall2( func, data, newSize );
    }
    
    void Adapter::llvmCallFree( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *data ) const
    {
      // RC::ConstHandle< IntegerAdapter > integerAdapter = basicBlockBuilder.getManager()->getIntegerAdapter();

      std::vector< llvm::Type const * > argTypes;
      argTypes.push_back( basicBlockBuilder->getInt8PtrTy() );
      llvm::FunctionType const *funcType = llvm::FunctionType::get( basicBlockBuilder->getVoidTy(), argTypes, false );

      llvm::AttributeWithIndex AWI[1];
      AWI[0] = llvm::AttributeWithIndex::get( ~0u, llvm::Attribute::NoUnwind );
      llvm::AttrListPtr attrListPtr = llvm::AttrListPtr::get( AWI, 1 );

      llvm::Function *func = llvm::cast<llvm::Function>( basicBlockBuilder.getModuleBuilder()->getOrInsertFunction( "free", funcType, attrListPtr ) ); 
      
      basicBlockBuilder->CreateCall( func, basicBlockBuilder->CreatePointerCast( data, basicBlockBuilder->getInt8PtrTy() ) );
    }
    
    void Adapter::llvmAssign( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const
    {
      // [pzion 20110204] Start by looking for operator overload
      std::string name = methodOverloadName( assignOpMethodName( ASSIGN_OP ), this, this );
      RC::ConstHandle<FunctionSymbol> functionSymbol = basicBlockBuilder.maybeGetFunction( name );
      if ( functionSymbol )
      {
        ExprValue dstExprValue = ExprValue( this, USAGE_LVALUE, basicBlockBuilder.getContext(), dstLValue );
        ExprValue srcExprValue = ExprValue( this, USAGE_RVALUE, basicBlockBuilder.getContext(), srcRValue );
        functionSymbol->llvmCreateCall( basicBlockBuilder, dstExprValue, srcExprValue );
      }
      else llvmDefaultAssign( basicBlockBuilder, dstLValue, srcRValue );
    }
    
    void Adapter::llvmStore( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const
    {
      llvm::Value *srcRawValue;
      if ( m_flags & FL_PASS_BY_REFERENCE )
        srcRawValue = basicBlockBuilder->CreateLoad( srcRValue );
      else srcRawValue = srcRValue;
      basicBlockBuilder->CreateStore( srcRawValue, dstLValue );
    }

    llvm::Value *Adapter::llvmCast( BasicBlockBuilder &basicBlockBuilder, ExprValue exprValue ) const
    {
      if ( !exprValue.getAdapter() )
        throw Exception( "function does not return a value" );
        
      if ( exprValue.getAdapter()->getDesc()->getImpl() == getDesc()->getImpl() )
      {
        switch ( exprValue.getUsage() )
        {
          case USAGE_RVALUE:
            return exprValue.getValue();
          case USAGE_LVALUE:
          {
            llvm::Value *rValue = llvmLValueToRValue( basicBlockBuilder, exprValue.getValue() );
            llvmRetain( basicBlockBuilder, rValue );
            return rValue;
          }
          default:
            FABRIC_ASSERT( false );
            return 0;
        }
      }
      else
      {
        std::string name = constructOverloadName( this, exprValue.getAdapter() );
        exprValue.getAdapter()->llvmCompileToModule( basicBlockBuilder.getModuleBuilder() );
        RC::ConstHandle<FunctionSymbol> functionSymbol = basicBlockBuilder.maybeGetFunction( name );
        if ( !functionSymbol )
          throw Exception( "no cast exists from " + exprValue.getTypeUserName() + " to " + getUserName() );
        
        llvm::Value *srcRValue = 0;
        switch ( exprValue.getUsage() )
        {
          case USAGE_RVALUE:
            srcRValue = exprValue.getValue();
            break;
          case USAGE_LVALUE:
            srcRValue = exprValue.getAdapter()->llvmLValueToRValue( basicBlockBuilder, exprValue.getValue() );
            exprValue.getAdapter()->llvmRetain( basicBlockBuilder, srcRValue );
            break;
          default:
            FABRIC_ASSERT( false );
            break;
        }
        
        ExprValue srcExprValue( exprValue.getAdapter(), USAGE_RVALUE, basicBlockBuilder.getContext(), srcRValue );
        llvm::Value *dstLValue = llvmAlloca( basicBlockBuilder, "castTo"+getUserName() );
        llvmInit( basicBlockBuilder, dstLValue );
        ExprValue dstExprValue( this, USAGE_LVALUE, basicBlockBuilder.getContext(), dstLValue );
        functionSymbol->llvmCreateCall( basicBlockBuilder, dstExprValue, srcExprValue );
        return llvmLValueToRValue( basicBlockBuilder, dstLValue );
      }
    }

    llvm::Type const *Adapter::llvmSizeType( RC::Handle<Context> const &context ) const
    {
      llvm::Type const *llvmSizeType;
      if ( sizeof(size_t) == sizeof(int32_t) )
        llvmSizeType = llvm::Type::getInt32Ty( context->getLLVMContext() );
      else if ( sizeof(size_t) == sizeof(int64_t) )
        llvmSizeType = llvm::Type::getInt64Ty( context->getLLVMContext() );
      else
      {
        FABRIC_ASSERT( false && "Unable to determin LLVM type for size_t" );
        llvmSizeType = 0;
      }
      return llvmSizeType;
    }

    llvm::Value *Adapter::llvmAlloca( BasicBlockBuilder &basicBlockBuilder, std::string const &name ) const
    {
      llvm::Instruction *allocaInst = new llvm::AllocaInst( llvmRawType( basicBlockBuilder.getContext() ) );
      allocaInst->setName( name );
      
      FunctionBuilder &functionBuilder = basicBlockBuilder.getFunctionBuilder();
      llvm::BasicBlock &entryBB = functionBuilder->getEntryBlock();
      llvm::BasicBlock::iterator it = entryBB.begin();
      while ( it != entryBB.end() )
      {
        if ( !llvm::isa<llvm::AllocaInst>(*it) )
          break;
        ++it;
      }
      entryBB.getInstList().insert( it, allocaInst );
      
      return allocaInst;
    }
    
    llvm::Value *Adapter::llvmLValueToRValue( BasicBlockBuilder &basicBlockBuilder, llvm::Value *lValue ) const
    {
      FABRIC_ASSERT( lValue->getType() == llvmLType( basicBlockBuilder.getContext() ) );
      llvm::Value *result;
      if ( m_flags & FL_PASS_BY_REFERENCE )
        result = lValue;
      else result = basicBlockBuilder->CreateLoad( lValue );
      FABRIC_ASSERT( result->getType() == llvmRType( basicBlockBuilder.getContext() ) );
      return result;
    }

    llvm::Value *Adapter::llvmRValueToLValue( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
      FABRIC_ASSERT( rValue->getType() == llvmRType( basicBlockBuilder.getContext() ) );
      llvm::Value *result;
      if ( m_flags & FL_PASS_BY_REFERENCE )
        result = rValue;
      else
      {
        llvm::Value *lValue = llvmAlloca( basicBlockBuilder, std::string(rValue->getName())+"_tempLValue" );
        basicBlockBuilder->CreateStore( rValue, lValue );
        result = lValue;
      }
      FABRIC_ASSERT( result->getType() == llvmLType( basicBlockBuilder.getContext() ) );
      return result;
    }

    llvm::Type const *Adapter::llvmRawType( RC::Handle<Context> const &context ) const
    {
      llvm::Type const *llvmRawType = context->getLLVMRawType( m_codeName );
      if ( !llvmRawType )
      {
        llvmRawType = buildLLVMRawType( context );
        context->setLLVMRawType( m_codeName, llvmRawType );
      }
      return llvmRawType;
    }

    llvm::Type const *Adapter::llvmLType( RC::Handle<Context> const &context ) const
    {
      return llvmRawType( context )->getPointerTo();
    }
    
    llvm::Type const *Adapter::llvmRType( RC::Handle<Context> const &context ) const
    {
      if ( (m_flags & FL_PASS_BY_REFERENCE) )
        return llvmRawType( context )->getPointerTo();
      else return llvmRawType( context );
    }

    llvm::Constant *Adapter::llvmDefaultRValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      return llvmDefaultValue( basicBlockBuilder );
    }
    
    llvm::Constant *Adapter::llvmDefaultLValue( BasicBlockBuilder &basicBlockBuilder ) const
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
    
    llvm::GlobalValue *Adapter::llvmAdapterGlobalValue( llvm::Module &module ) const
    {
      std::string name = "__" + getCodeName() + "__Adapter";

      llvm::GlobalVariable *llvmAdapterGlobalValue = module.getNamedGlobal( name );
      if( !llvmAdapterGlobalValue )
      {
        llvm::Type const *int8PtrTy = llvm::Type::getInt8PtrTy( module.getContext() );
        
        llvmAdapterGlobalValue = new llvm::GlobalVariable(
          module,
          int8PtrTy,
          true,
          llvm::GlobalValue::ExternalLinkage, // [pzion 20110324] This should be different, something like 'static', but I get errors.
          0,
          name
          );
      }

      return llvmAdapterGlobalValue;
    }
    
    llvm::Value *Adapter::llvmAdapterPtr( BasicBlockBuilder &basicBlockBuilder ) const
    {
      llvm::Type const *int8PtrTy = llvm::Type::getInt8PtrTy( basicBlockBuilder.getContext()->getLLVMContext() );
      return basicBlockBuilder->CreatePointerCast( llvmAdapterGlobalValue( *basicBlockBuilder.getModuleBuilder() ), int8PtrTy );
    }
      
    RC::ConstHandle<Adapter> Adapter::getAdapter( RC::ConstHandle<RT::Desc> const &desc ) const
    {
      return m_manager.makeStrong()->getAdapter( desc );
    }

    void Adapter::llvmInit( BasicBlockBuilder &basicBlockBuilder, llvm::Value *lValue ) const
    {
      llvm::Value *defaultRValue = llvmDefaultRValue( basicBlockBuilder );
      llvmRetain( basicBlockBuilder, defaultRValue );
      llvmStore( basicBlockBuilder, lValue, defaultRValue );
    }
  };
};
