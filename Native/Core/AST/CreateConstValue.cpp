/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/AST/CreateConstValue.h>
#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Core/CG/Context.h>
#include <Fabric/Core/CG/IntegerAdapter.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/ValueProducerAdapter.h>
#include <Fabric/Base/Util/SimpleString.h>

#include <llvm/Module.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( CreateConstValue );
    
    RC::ConstHandle<CreateConstValue> CreateConstValue::Create(
      CG::Location const &location,
      RC::ConstHandle<Expr> const &child
      )
    {
      return new CreateConstValue( location, child );
    }
    
    CreateConstValue::CreateConstValue(
      CG::Location const &location,
      RC::ConstHandle<Expr> const &child
      )
      : Expr( location )
      , m_child( child )
    {
    }
    
    void CreateConstValue::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const
    {
      Expr::appendJSONMembers( jsonObjectGenerator, includeLocation );
      m_child->appendJSON( jsonObjectGenerator.makeMember( "child" ), includeLocation );
    }
    
    void CreateConstValue::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      m_child->registerTypes( cgManager, diagnostics );
    }
    
    RC::ConstHandle<CG::Adapter> CreateConstValue::getType( CG::BasicBlockBuilder &basicBlockBuilder ) const
    {
      return basicBlockBuilder.getManager()->getValueProducerOf( m_child->getType( basicBlockBuilder ) );
    }
    
    CG::ExprValue CreateConstValue::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      if ( usage == CG::USAGE_LVALUE )
        throw Exception( "cannot be used as l-values" );
      
      RC::ConstHandle<CG::Adapter> valueAdapter = m_child->getType( basicBlockBuilder );
      RC::ConstHandle<CG::ValueProducerAdapter> valueProducerAdapter = basicBlockBuilder.getManager()->getValueProducerOf( valueAdapter );
      
      RC::Handle<CG::Context> context = basicBlockBuilder.getContext();
      llvm::LLVMContext &llvmContext = context->getLLVMContext();
      
      std::vector<llvm::Type const *> argTypes;
      argTypes.push_back( llvm::Type::getInt8PtrTy( llvmContext ) );
      argTypes.push_back( llvm::Type::getInt8PtrTy( llvmContext ) );
      argTypes.push_back( valueProducerAdapter->llvmLType( context ) );
      llvm::FunctionType const *funcType = llvm::FunctionType::get( llvm::Type::getVoidTy( llvmContext ), argTypes, false );
      llvm::Constant *func = basicBlockBuilder.getModuleBuilder()->getOrInsertFunction( "__MR_CreateConstValue", funcType );
      
      CG::ExprValue childExprRValue = m_child->buildExprValue( basicBlockBuilder, CG::USAGE_RVALUE, lValueErrorDesc );
      llvm::Value *childExprLValue = valueAdapter->llvmRValueToLValue( basicBlockBuilder, childExprRValue.getValue() );
      llvm::Value *resultLValue = valueProducerAdapter->llvmAlloca( basicBlockBuilder, "result" );
      valueProducerAdapter->llvmInit( basicBlockBuilder, resultLValue );
      
      basicBlockBuilder->CreateCall3(
        func,
        valueProducerAdapter->llvmAdapterPtr( basicBlockBuilder ),
        basicBlockBuilder->CreatePointerCast(
          childExprLValue,
          llvm::Type::getInt8PtrTy( llvmContext )
          ),
        resultLValue
        );
      
      return CG::ExprValue(
        valueProducerAdapter,
        CG::USAGE_RVALUE,
        context,
        valueProducerAdapter->llvmLValueToRValue( basicBlockBuilder, resultLValue )
        );
    }
  }
}
