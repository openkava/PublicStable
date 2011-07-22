/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_BASIC_BLOCK_BUILDER_H
#define _FABRIC_RT_BASIC_BLOCK_BUILDER_H

#include <Fabric/Base/RC/Handle.h>

#include <llvm/Support/IRBuilder.h>

namespace llvm
{
  class LLVMContext;
  class Module;
  class Function;
  class Value;
};

namespace Fabric
{
  namespace RT
  {
    class Desc;
  };
  
  namespace CG
  {
    class Manager;
    class Scope;
    class ModuleBuilder;
    class FunctionBuilder;
    class FunctionSymbol;
    class Adapter;
    
    class BasicBlockBuilder
    {
    public:
    
      BasicBlockBuilder( FunctionBuilder &functionBuilder );
      BasicBlockBuilder( BasicBlockBuilder &parentBasicBlockBuilder, Scope &scope );
      ~BasicBlockBuilder();
      
      llvm::IRBuilder<> *operator ->();
      
      Scope const &getScope() const;
      Scope &getScope();
      
      FunctionBuilder &getFunctionBuilder();
      ModuleBuilder &getModuleBuilder();
      
      RC::ConstHandle<Manager> getManager() const;
      RC::Handle<Manager> getManager();
      llvm::LLVMContext &getLLVMContext();

      RC::ConstHandle<Adapter> maybeGetAdapter( std::string const &userName ) const;
      RC::ConstHandle<Adapter> getAdapter( std::string const &userName ) const;

      RC::ConstHandle<FunctionSymbol> maybeGetFunction( std::string const &entryName ) const;

      RC::ConstHandle<RT::Desc> getStrongerTypeOrNone( RC::ConstHandle<RT::Desc> const &lhsDesc, RC::ConstHandle<RT::Desc> const &rhsDesc ) const;

    private:
    
      FunctionBuilder &m_functionBuilder;
      BasicBlockBuilder *m_parentBasicBlockBuilder;
      llvm::IRBuilder<> m_irBuilder;
      Scope &m_scope;
    };
  };
};

#endif //_FABRIC_RT_BASIC_BLOCK_BUILDER_H
