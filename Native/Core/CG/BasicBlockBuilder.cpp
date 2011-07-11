/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "BasicBlockBuilder.h"
#include "FunctionBuilder.h"
#include "Scope.h"
#include "Manager.h"

namespace Fabric
{
  namespace CG
  {
    BasicBlockBuilder::BasicBlockBuilder( FunctionBuilder &functionBuilder )
      : m_functionBuilder( functionBuilder )
      , m_parentBasicBlockBuilder( 0 )
      , m_irBuilder( functionBuilder.getLLVMContext() )
      , m_scope( functionBuilder.getScope() )     
    {
    }
    
    BasicBlockBuilder::BasicBlockBuilder( BasicBlockBuilder &parentBasicBlockBuilder, Scope &scope )
      : m_functionBuilder( parentBasicBlockBuilder.getFunctionBuilder() )
      , m_parentBasicBlockBuilder( &parentBasicBlockBuilder )
      , m_irBuilder( parentBasicBlockBuilder.getLLVMContext() )
      , m_scope( scope )     
    {
      m_irBuilder.SetInsertPoint( parentBasicBlockBuilder.m_irBuilder.GetInsertBlock() );
    }
    
    BasicBlockBuilder::~BasicBlockBuilder()
    {
      if ( m_parentBasicBlockBuilder )
        m_parentBasicBlockBuilder->m_irBuilder.SetInsertPoint( m_irBuilder.GetInsertBlock() );
    }
    
    llvm::IRBuilder<> *BasicBlockBuilder::operator ->()
    {
      return &m_irBuilder;
    }
    
    Scope const &BasicBlockBuilder::getScope() const
    {
      return m_scope;
    }
    
    Scope &BasicBlockBuilder::getScope()
    {
      return m_scope;
    }
    
    FunctionBuilder &BasicBlockBuilder::getFunctionBuilder()
    {
      return m_functionBuilder;
    }
    
    ModuleBuilder &BasicBlockBuilder::getModuleBuilder()
    {
      return m_functionBuilder.getModuleBuilder();
    }
    
    RC::Handle<Manager> BasicBlockBuilder::getManager()
    {
      return m_functionBuilder.getManager();
    }
    
    RC::ConstHandle<Manager> BasicBlockBuilder::getManager() const
    {
      return m_functionBuilder.getManager();
    }
    
    llvm::LLVMContext &BasicBlockBuilder::getLLVMContext()
    {
      return m_functionBuilder.getLLVMContext();
    }

    RC::ConstHandle<FunctionSymbol> BasicBlockBuilder::maybeGetFunction( std::string const &entryName ) const
    {
      return m_functionBuilder.maybeGetFunction( entryName );
    }

    RC::ConstHandle<RT::Desc> BasicBlockBuilder::getStrongerTypeOrNone( RC::ConstHandle<RT::Desc> const &lhsDesc, RC::ConstHandle<RT::Desc> const &rhsDesc ) const
    {
      return getManager()->getStrongerTypeOrNone( lhsDesc, rhsDesc );
    }
  };
};
