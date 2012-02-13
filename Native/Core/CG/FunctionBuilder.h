/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_CG_FUNCTION_BUILDER_H
#define _FABRIC_CG_FUNCTION_BUILDER_H

#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Base/RC/Handle.h>

#include <llvm/Argument.h>

namespace llvm
{
  class LLVMContext;
  class Module;
  class Function;
};

namespace Fabric
{
  namespace CG
  {
    class Manager;
    class ModuleBuilder;
    class PencilSymbol;
    
    class FunctionBuilder
    {
    public:
    
      static const size_t ExportSymbol = 0x01;
      static const size_t ReturnsStaticDataPtr = 0x02;
    
      FunctionBuilder( 
        ModuleBuilder &moduleBuilder,
        std::string const &pencilName,
        std::string const &symbolName,
        RC::ConstHandle<Adapter> const &returnAdapter, 
        ParamVector const &params,
        size_t flags
        );
      
      FunctionBuilder( 
        ModuleBuilder &moduleBuilder, 
        std::string const &pencilName,
        std::string const &symbolName,
        std::string const &paramLayout,
        size_t flags
        );
        
      FunctionBuilder(
        ModuleBuilder &moduleBuilder,
        llvm::FunctionType const *llvmFunctionType,
        llvm::Function *llvmFunction
        );
      
      ~FunctionBuilder();
      
      llvm::Function *getLLVMFunction();
      llvm::Function *operator ->();
      
      ModuleBuilder &getModuleBuilder();
      llvm::BasicBlock *createBasicBlock( std::string const &name );
      
      RC::Handle<Manager> getManager();

      RC::Handle<Context> getContext();
      
      llvm::Argument *operator[]( size_t index );
      
      FunctionScope &getScope();
      RC::ConstHandle<PencilSymbol> maybeGetPencil( std::string const &entryName ) const;

      RC::ConstHandle<Adapter> maybeGetAdapter( std::string const &userName ) const;
      RC::ConstHandle<Adapter> getAdapter( std::string const &userName, CG::Location const &location ) const;
      
      RC::ConstHandle<CG::PencilSymbol> getPencil() const;
      
    private:
    
      void build( 
        std::string const &pencilName, 
        std::string const &symbolName, 
        RC::ConstHandle<Adapter> const &returnAdapter, 
        ParamVector const &params, 
        size_t flags
        );
                
      ModuleBuilder &m_moduleBuilder;
      llvm::FunctionType const *m_llvmFunctionType;
      llvm::Function *m_llvmFunction;
      FunctionScope *m_functionScope;
      RC::ConstHandle<CG::PencilSymbol> m_pencil;
    };
  };
};

#endif //_FABRIC_CG_FUNCTION_BUILDER_H
