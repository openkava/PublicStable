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
    class Context;
    class Location;
    class Manager;
    
    class ModuleBuilder
    {
    public:
    
      ModuleBuilder( RC::Handle<Manager> const &manager, RC::Handle<Context> const &context, llvm::Module *module );
      
      operator llvm::Module *();
      llvm::Module *operator ->();
      
      RC::Handle<Manager> getManager();
      
      RC::Handle<Context> getContext();
      
      ModuleScope &getScope();
      
      bool contains( std::string const &codeName, bool buildFunctions );
      
      void addFunction( std::string const &entryName, RC::ConstHandle<FunctionSymbol> const &functionSymbol, std::string const *friendlyName = 0 );
      RC::ConstHandle<FunctionSymbol> maybeGetFunction( std::string const &entryName ) const;

      RC::ConstHandle<Adapter> maybeGetAdapter( std::string const &userName ) const;
      RC::ConstHandle<Adapter> getAdapter( std::string const &userName, CG::Location const &location );
      
    private:
    
      typedef std::map< std::string, RC::ConstHandle<FunctionSymbol> > Functions;
    
      RC::Handle<Manager> m_manager;     
      RC::Handle<Context> m_context; 
      llvm::Module *m_module;
      ModuleScope m_moduleScope;
      std::set< std::pair<std::string, bool> > m_contained;
      Functions m_functions;
    };
  };
};

#endif //_FABRIC_CG_MODULE_BUILDER_H
