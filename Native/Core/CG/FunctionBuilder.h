#ifndef _FABRIC_CG_FUNCTION_BUILDER_H
#define _FABRIC_CG_FUNCTION_BUILDER_H

#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Base/RC/Handle.h>

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
    
    class FunctionBuilder
    {
    public:
    
      FunctionBuilder( 
        ModuleBuilder &moduleBuilder, 
        std::string const &entryName, 
        ExprType const &returnExprType, 
        std::vector< FunctionParam > const &params, 
        std::string const *friendlyName = 0,
        bool returnsStaticDataPtr = false
        );
      
      FunctionBuilder( 
        ModuleBuilder &moduleBuilder, 
        std::string const &entryName, 
        std::string const &paramLayout,
        std::string const *friendlyName = 0, 
        bool returnsStaticDataPtr = false
        );
      ~FunctionBuilder();
      
      llvm::Function *getLLVMFunction();
      llvm::Function *operator ->();
      
      ModuleBuilder &getModuleBuilder();
      llvm::BasicBlock *createBasicBlock( std::string const &name );
      
      RC::Handle<Manager> getManager();
      llvm::LLVMContext &getLLVMContext();
      
      llvm::Value *operator[]( size_t index );
      
      FunctionScope &getScope();
      RC::ConstHandle<FunctionSymbol> maybeGetFunction( std::string const &entryName ) const;
      
    private:
    
      void build( 
        std::string const &entryName, 
        ExprType const &returnExprType, 
        std::vector< FunctionParam > const &params, 
        std::string const *friendlyName, 
        bool returnsStaticDataPtr
        );
                
      ModuleBuilder &m_moduleBuilder;
      llvm::FunctionType const *m_llvmFunctionType;
      llvm::Function *m_llvmFunction;
      FunctionScope *m_functionScope;
    };
  };
};

#endif //_FABRIC_CG_FUNCTION_BUILDER_H
