/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/AST/CreateConstArray.h>
#include <Fabric/Core/CG/ArrayAdapter.h>
#include <Fabric/Core/CG/ArrayProducerAdapter.h>
#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Core/CG/Context.h>
#include <Fabric/Core/CG/IntegerAdapter.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/ArrayProducerAdapter.h>
#include <Fabric/Base/Util/SimpleString.h>

#include <llvm/Module.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( CreateConstArray );
    
    RC::ConstHandle<CreateConstArray> CreateConstArray::Create(
      CG::Location const &location,
      RC::ConstHandle<Expr> const &child
      )
    {
      return new CreateConstArray( location, child );
    }
    
    CreateConstArray::CreateConstArray(
      CG::Location const &location,
      RC::ConstHandle<Expr> const &child
      )
      : Expr( location )
      , m_child( child )
    {
    }
    
    void CreateConstArray::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const
    {
      Expr::appendJSONMembers( jsonObjectGenerator, includeLocation );
      m_child->appendJSON( jsonObjectGenerator.makeMember( "child" ), includeLocation );
    }
    
    void CreateConstArray::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      m_child->registerTypes( cgManager, diagnostics );
    }
    
    RC::ConstHandle<CG::Adapter> CreateConstArray::getType( CG::BasicBlockBuilder &basicBlockBuilder ) const
    {
      RC::ConstHandle<CG::Adapter> childAdapter = m_child->getType( basicBlockBuilder );
      if ( !RT::isArray( childAdapter->getType() ) )
        throw CG::Error( getLocation(), "parameter must be an array" );
      RC::ConstHandle<CG::ArrayAdapter> arrayAdapter = RC::ConstHandle<CG::ArrayAdapter>::StaticCast( childAdapter );
      RC::ConstHandle<CG::Adapter> elementAdapter = arrayAdapter->getMemberAdapter();
      RC::ConstHandle<CG::ArrayProducerAdapter> arrayProducerAdapter = basicBlockBuilder.getManager()->getArrayProducerOf( elementAdapter );
      return arrayProducerAdapter;
    }
    
    CG::ExprValue CreateConstArray::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      if ( usage == CG::USAGE_LVALUE )
        throw Exception( "cannot be used as l-values" );
      
      RC::ConstHandle<CG::Adapter> childAdapter = m_child->getType( basicBlockBuilder );
      if ( !RT::isArray( childAdapter->getType() ) )
        throw CG::Error( getLocation(), "parameter must be an array" );
      RC::ConstHandle<CG::ArrayAdapter> arrayAdapter = RC::ConstHandle<CG::ArrayAdapter>::StaticCast( childAdapter );
      RC::ConstHandle<CG::Adapter> elementAdapter = arrayAdapter->getMemberAdapter();
      RC::ConstHandle<CG::ArrayProducerAdapter> arrayProducerAdapter = basicBlockBuilder.getManager()->getArrayProducerOf( elementAdapter );
      
      RC::Handle<CG::Context> context = basicBlockBuilder.getContext();
      llvm::LLVMContext &llvmContext = context->getLLVMContext();
      
      std::vector<llvm::Type const *> argTypes;
      argTypes.push_back( llvm::Type::getInt8PtrTy( llvmContext ) );
      argTypes.push_back( llvm::Type::getInt8PtrTy( llvmContext ) );
      argTypes.push_back( llvm::Type::getInt8PtrTy( llvmContext ) );
      argTypes.push_back( arrayProducerAdapter->llvmLType( context ) );
      llvm::FunctionType const *funcType = llvm::FunctionType::get( llvm::Type::getVoidTy( llvmContext ), argTypes, false );
      llvm::Constant *func = basicBlockBuilder.getModuleBuilder()->getOrInsertFunction( "__MR_CreateConstArray", funcType );
      
      CG::ExprValue childExprRValue = m_child->buildExprValue( basicBlockBuilder, CG::USAGE_RVALUE, lValueErrorDesc );
      llvm::Value *childExprLValue = arrayAdapter->llvmRValueToLValue( basicBlockBuilder, childExprRValue.getValue() );
      llvm::Value *resultLValue = arrayProducerAdapter->llvmAlloca( basicBlockBuilder, "result" );
      arrayProducerAdapter->llvmInit( basicBlockBuilder, resultLValue );
      basicBlockBuilder.getScope().put(
        CG::VariableSymbol::Create(
          CG::ExprValue(
            arrayProducerAdapter,
            CG::USAGE_LVALUE,
            context,
            resultLValue
            )
          )
        );
      
      basicBlockBuilder->CreateCall4(
        func,
        arrayAdapter->llvmAdapterPtr( basicBlockBuilder ),
        basicBlockBuilder->CreatePointerCast(
          childExprLValue,
          llvm::Type::getInt8PtrTy( llvmContext )
          ),
        arrayProducerAdapter->llvmAdapterPtr( basicBlockBuilder ),
        resultLValue
        );
      
      return CG::ExprValue(
        arrayProducerAdapter,
        CG::USAGE_RVALUE,
        context,
        arrayProducerAdapter->llvmLValueToRValue( basicBlockBuilder, resultLValue )
        );
    }
  }
}
