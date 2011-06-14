#include "ModuleBuilder.h"
#include "Manager.h"

namespace Fabric
{
  namespace CG
  {
    ModuleBuilder::ModuleBuilder( RC::Handle<Manager> const &manager, llvm::Module *module )
      : m_manager( manager )
      , m_module( module )
    {
    }
    
    RC::Handle<Manager> ModuleBuilder::getManager()
    {
      return m_manager;
    }
    
    llvm::LLVMContext &ModuleBuilder::getLLVMContext()
    {
      return m_manager->getLLVMContext();
    }
    
    ModuleBuilder::operator llvm::Module *()
    {
      return m_module;
    }
    
    llvm::Module *ModuleBuilder::operator ->()
    {
      return m_module;
    }
      
    ModuleScope &ModuleBuilder::getScope()
    {
      return m_moduleScope;
    }
    
    bool ModuleBuilder::contains( std::string const &codeName )
    {
      bool insertResult = m_contained.insert( codeName ).second;
      return !insertResult;
    }
  };
};
