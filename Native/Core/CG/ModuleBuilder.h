#ifndef _FABRIC_CG_MODULE_BUILDER_H
#define _FABRIC_CG_MODULE_BUILDER_H

#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Base/RC/Handle.h>

#include <set>

namespace llvm
{
  class LLVMContext;
  class Module;
};

namespace Fabric
{
  namespace CG
  {
    class Manager;
    
    class ModuleBuilder
    {
    public:
    
      ModuleBuilder( RC::Handle<Manager> const &manager, llvm::Module *module );
      
      operator llvm::Module *();
      llvm::Module *operator ->();
      
      RC::Handle<Manager> getManager();
      
      llvm::LLVMContext &getLLVMContext();
      
      ModuleScope &getScope();
      
      bool contains( std::string const &codeName );
      
    private:
    
      RC::Handle<Manager> m_manager;      
      llvm::Module *m_module;
      ModuleScope m_moduleScope;
      std::set<std::string> m_contained;
    };
  };
};

#endif //_FABRIC_CG_MODULE_BUILDER_H
