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
    
    RC::Handle<Adapter> ModuleBuilder::getAdapter( std::string const &userName )
    {
      return m_manager->getAdapter( userName );
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

    void ModuleBuilder::addFunction( std::string const &entryName, RC::ConstHandle<FunctionSymbol> const &functionSymbol, std::string const *friendlyName )
    {
      FABRIC_ASSERT( entryName.length() > 0 );
      
      std::pair< Functions::iterator, bool > insertResult = m_functions.insert( Functions::value_type( entryName, functionSymbol ) );
      if ( !insertResult.second )
      {
        RC::ConstHandle<FunctionSymbol> const &existingFunctionSymbol = insertResult.first->second;
        if ( existingFunctionSymbol->getLLVMFunction() != functionSymbol->getLLVMFunction() )
          throw Exception( "function with entry name " + _(entryName) + " already exists" );
      }
      
      if ( friendlyName )
        m_moduleScope.put( *friendlyName, functionSymbol );
    }
    
    RC::ConstHandle<FunctionSymbol> ModuleBuilder::maybeGetFunction( std::string const &entryName ) const
    {
      RC::ConstHandle<FunctionSymbol> result;
      Functions::const_iterator it = m_functions.find( entryName );
      if ( it != m_functions.end() )
        result = it->second;
      return result;
    }
  };
};
