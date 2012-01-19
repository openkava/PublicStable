/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/AST/CreateValueCache.h>
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
    FABRIC_AST_NODE_IMPL( CreateValueCache );
    
    RC::ConstHandle<CreateValueCache> CreateValueCache::Create(
      CG::Location const &location,
      RC::ConstHandle<Expr> const &input
      )
    {
      return new CreateValueCache( location, input );
    }
    
    CreateValueCache::CreateValueCache(
      CG::Location const &location,
      RC::ConstHandle<Expr> const &input
      )
      : Expr( location )
      , m_input( input )
    {
    }
    
    void CreateValueCache::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const
    {
      Expr::appendJSONMembers( jsonObjectGenerator, includeLocation );
      m_input->appendJSON( jsonObjectGenerator.makeMember( "input" ), includeLocation );
    }
    
    void CreateValueCache::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      m_input->registerTypes( cgManager, diagnostics );
    }
    
    RC::ConstHandle<CG::Adapter> CreateValueCache::getType( CG::BasicBlockBuilder &basicBlockBuilder ) const
    {
      return basicBlockBuilder.getManager()->getValueProducerOf( m_input->getType( basicBlockBuilder ) );
    }
    
    CG::ExprValue CreateValueCache::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      if ( usage == CG::USAGE_LVALUE )
        throw Exception( "cannot be used as l-values" );

      RC::Handle<CG::Context> context = basicBlockBuilder.getContext();
      llvm::LLVMContext &llvmContext = context->getLLVMContext();
      
      RC::ConstHandle<CG::Adapter> inputAdapter = m_input->getType( basicBlockBuilder );
      if ( !RT::isValueProducer( inputAdapter->getType() ) )
        throw CG::Error( getLocation(), "input must be a value producer" );
      RC::ConstHandle<CG::ValueProducerAdapter> valueProducerAdapter = RC::ConstHandle<CG::ValueProducerAdapter>::StaticCast( inputAdapter );
      RC::ConstHandle<CG::Adapter> valueAdapter = valueProducerAdapter->getValueAdapter();
     
      CG::ExprValue inputExprRValue = m_input->buildExprValue( basicBlockBuilder, CG::USAGE_RVALUE, lValueErrorDesc );
      llvm::Value *resultLValue = valueProducerAdapter->llvmAlloca( basicBlockBuilder, "result" );

      valueProducerAdapter->llvmInit( basicBlockBuilder, resultLValue );
      basicBlockBuilder.getScope().put(
        CG::VariableSymbol::Create(
          CG::ExprValue(
            valueProducerAdapter,
            CG::USAGE_LVALUE,
            context,
            resultLValue
            )
          )
        );

      std::vector<llvm::Type const *> argTypes;
      argTypes.push_back( valueProducerAdapter->llvmLType( context ) );
      argTypes.push_back( llvm::Type::getInt8PtrTy( llvmContext ) );
      llvm::FunctionType const *funcType = llvm::FunctionType::get( llvm::Type::getVoidTy( llvmContext ), argTypes, false );
      llvm::Constant *func = basicBlockBuilder.getModuleBuilder()->getOrInsertFunction( "__MR_CreateValueCache", funcType );
      
      basicBlockBuilder->CreateCall2(
        func,
        valueProducerAdapter->llvmAdapterPtr( basicBlockBuilder ),
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
