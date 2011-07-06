#ifndef _FABRIC_CG_MODULE_BUILDER_H
#define _FABRIC_CG_MODULE_BUILDER_H

#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Base/RC/Handle.h>

#include <set>
#include <map>

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
      
      void addFunction( std::string const &entryName, RC::ConstHandle<FunctionSymbol> const &functionSymbol, std::string const *friendlyName = 0 );
      RC::ConstHandle<FunctionSymbol> maybeGetFunction( std::string const &entryName ) const;
      
    private:
    
      typedef std::map< std::string, RC::ConstHandle<FunctionSymbol> > Functions;
    
      RC::Handle<Manager> m_manager;      
      llvm::Module *m_module;
      ModuleScope m_moduleScope;
      std::set<std::string> m_contained;
      Functions m_functions;
    };
  };
};

#endif //_FABRIC_CG_MODULE_BUILDER_H
