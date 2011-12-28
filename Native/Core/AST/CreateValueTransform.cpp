/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/AST/CreateValueTransform.h>
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
    FABRIC_AST_NODE_IMPL( CreateValueTransform );
    
    RC::ConstHandle<CreateValueTransform> CreateValueTransform::Create(
      CG::Location const &location,
      RC::ConstHandle<Expr> const &input,
      std::string const &operatorName,
      RC::ConstHandle<Expr> const &shared
      )
    {
      return new CreateValueTransform( location, input, operatorName, shared );
    }
    
    CreateValueTransform::CreateValueTransform(
      CG::Location const &location,
      RC::ConstHandle<Expr> const &input,
      std::string const &operatorName,
      RC::ConstHandle<Expr> const &shared
      )
      : Expr( location )
      , m_input( input )
      , m_operatorName( operatorName )
      , m_shared( shared )
    {
    }
    
    void CreateValueTransform::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const
    {
      Expr::appendJSONMembers( jsonObjectGenerator, includeLocation );
      m_input->appendJSON( jsonObjectGenerator.makeMember( "input" ), includeLocation );
      jsonObjectGenerator.makeMember( "operatorName" ).makeString( m_operatorName );
      m_shared->appendJSON( jsonObjectGenerator.makeMember( "shared" ), includeLocation );
    }
    
    void CreateValueTransform::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      m_input->registerTypes( cgManager, diagnostics );
      if ( m_shared )
        m_shared->registerTypes( cgManager, diagnostics );
    }
    
    RC::ConstHandle<CG::Adapter> CreateValueTransform::getType( CG::BasicBlockBuilder &basicBlockBuilder ) const
    {
      return basicBlockBuilder.getManager()->getValueProducerOf( m_input->getType( basicBlockBuilder ) );
    }
    
    CG::ExprValue CreateValueTransform::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
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
      
      RC::ConstHandle<CG::Symbol> operatorSymbol = basicBlockBuilder.getScope().get( m_operatorName );
      if ( !operatorSymbol )
        throw CG::Error( getLocation(), _(m_operatorName) + ": operator not found" );
      if ( !operatorSymbol->isFunction() )
        throw CG::Error( getLocation(), _(m_operatorName) + ": not an operator" );
      RC::ConstHandle<CG::FunctionSymbol> operator_ = RC::ConstHandle<CG::FunctionSymbol>::StaticCast( operatorSymbol );
      std::vector<CG::FunctionParam> const &operatorParams = operator_->getParams();
      CG::ExprValue inputExprRValue = m_input->buildExprValue( basicBlockBuilder, CG::USAGE_RVALUE, lValueErrorDesc );
      llvm::Value *resultLValue = valueProducerAdapter->llvmAlloca( basicBlockBuilder, "result" );
      valueProducerAdapter->llvmInit( basicBlockBuilder, resultLValue );

      if ( !m_shared )
      {
        if ( operatorParams.size() != 1
          || operatorParams[0].getAdapter() != valueAdapter
          || operatorParams[0].getUsage() != CG::USAGE_LVALUE
          )
          throw CG::Error( getLocation(), "operator must take parameters: io " + valueAdapter->getUserName() + " value" );
      
        std::vector<llvm::Type const *> argTypes;
        argTypes.push_back( llvm::Type::getInt8PtrTy( llvmContext ) );
        argTypes.push_back( valueProducerAdapter->llvmLType( context ) );
        argTypes.push_back( valueProducerAdapter->llvmLType( context ) );
        llvm::FunctionType const *funcType = llvm::FunctionType::get( llvm::Type::getVoidTy( llvmContext ), argTypes, false );
        llvm::Constant *func = basicBlockBuilder.getModuleBuilder()->getOrInsertFunction( "__MR_CreateValueTransform_1", funcType );
        
        basicBlockBuilder->CreateCall3(
          func,
          basicBlockBuilder->CreateBitCast(
            operator_->getLLVMFunction(),
            llvm::Type::getInt8PtrTy( llvmContext )
            ),
          inputExprRValue.getValue(),
          resultLValue
          );
      }
      else
      {
        RC::ConstHandle<CG::Adapter> sharedAdapter_ = m_shared->getType( basicBlockBuilder );
        if ( !RT::isValueProducer( sharedAdapter_->getType() ) )
          throw CG::Error( getLocation(), "shared must be a value producer" );
        RC::ConstHandle<CG::ValueProducerAdapter> sharedValueProducerAdapter = RC::ConstHandle<CG::ValueProducerAdapter>::StaticCast( sharedAdapter_ );
        RC::ConstHandle<CG::Adapter> sharedAdapter = sharedValueProducerAdapter->getValueAdapter();
        CG::ExprValue sharedExprRValue = m_shared->buildExprValue( basicBlockBuilder, CG::USAGE_RVALUE, lValueErrorDesc );
        
        if ( operatorParams.size() != 2
          || operatorParams[0].getAdapter() != valueAdapter
          || operatorParams[0].getUsage() != CG::USAGE_LVALUE
          || operatorParams[1].getAdapter() != sharedAdapter
          || operatorParams[1].getUsage() != CG::USAGE_RVALUE
          )
          throw CG::Error( getLocation(), "operator must take parameters: io " + valueAdapter->getUserName() + " value, in " + sharedAdapter->getUserName() + " shared" );
      
        std::vector<llvm::Type const *> argTypes;
        argTypes.push_back( llvm::Type::getInt8PtrTy( llvmContext ) );
        argTypes.push_back( valueProducerAdapter->llvmLType( context ) );
        argTypes.push_back( sharedValueProducerAdapter->llvmLType( context ) );
        argTypes.push_back( valueProducerAdapter->llvmLType( context ) );
        llvm::FunctionType const *funcType = llvm::FunctionType::get( llvm::Type::getVoidTy( llvmContext ), argTypes, false );
        llvm::Constant *func = basicBlockBuilder.getModuleBuilder()->getOrInsertFunction( "__MR_CreateValueTransform_2", funcType );
        
        basicBlockBuilder->CreateCall4(
          func,
          basicBlockBuilder->CreateBitCast(
            operator_->getLLVMFunction(),
            llvm::Type::getInt8PtrTy( llvmContext )
            ),
          inputExprRValue.getValue(),
          sharedExprRValue.getValue(),
          resultLValue
          );
      }
      
      return CG::ExprValue(
        valueProducerAdapter,
        CG::USAGE_RVALUE,
        context,
        valueProducerAdapter->llvmLValueToRValue( basicBlockBuilder, resultLValue )
        );
    }
  }
}
