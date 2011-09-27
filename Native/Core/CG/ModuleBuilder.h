/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
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
      
      bool haveCompiledToModule( std::string const &codeName );
      
      void addFunction( std::string const &entryName, RC::ConstHandle<FunctionSymbol> const &functionSymbol, std::string const *friendlyName = 0 );
      RC::ConstHandle<FunctionSymbol> maybeGetFunction( std::string const &entryName ) const;

      RC::ConstHandle<Adapter> maybeGetAdapter( std::string const &userName ) const;
      RC::ConstHandle<Adapter> getAdapter( std::string const &userName, CG::Location const &location );
      
      std::string getDestructorName( std::string const &typeName )
      {
        return m_destructorNames[typeName];
      }
      void setDestructorName( std::string const &typeName, std::string const &destructorName )
      {
        m_destructorNames[typeName] = destructorName;
      }
      
    private:
    
      typedef std::map< std::string, RC::ConstHandle<FunctionSymbol> > Functions;
    
      RC::Handle<Manager> m_manager;     
      RC::Handle<Context> m_context; 
      llvm::Module *m_module;
      ModuleScope m_moduleScope;
      std::set<std::string> m_havePreparedModule;
      std::set<std::string> m_haveCompiledToModule;
      Functions m_functions;
      std::map< std::string, std::string > m_destructorNames;
    };
  };
};

#endif //_FABRIC_CG_MODULE_BUILDER_H
