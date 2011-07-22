/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_PLUG_MANAGER_H
#define _FABRIC_PLUG_MANAGER_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/ConstHandle.h>
#include <Fabric/Core/Util/UnorderedMap.h>
#include <Fabric/Core/Plug/Helpers.h>
 
namespace Fabric
{
  namespace JSON
  {
    class Object;
  };
  
  namespace RT
  {
    class Manager;
  };
  
  namespace CG
  {
    class Manager;
    class ModuleBuilder;
  };
  
  namespace DG
  {
    class Context;
  };
  
  namespace Plug
  {
    class Inst;
    
    class Manager : public RC::Object
    {
    public:
    
      typedef Util::UnorderedMap< std::string, RC::Handle<Inst> > NameToInstMap;
    
      static RC::Handle<Manager> Instance();
      
      void loadBuiltInPlugins( std::vector<std::string> const &pluginDirs );
      
      NameToInstMap const &getRegisteredPlugins() const
      {
        return m_nameToInstMap;
      }
      
      void registerTypes( RC::Handle<RT::Manager> const &rtManager ) const;
      void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder ) const;
      void *llvmResolveExternalFunction( std::string const &name ) const;

      virtual RC::Handle<JSON::Object> jsonDesc() const;

    protected:
    
      Manager();
      ~Manager();
      
      RC::ConstHandle<Inst> registerPlugin( std::string const &name, std::string const &jsonDesc, std::vector<std::string> const &pluginDirs );
      
    private:
    
      bool m_loaded;
      std::vector<std::string> m_pluginDirs;
      std::string m_fabricSDKSOLibResolvedName;
      //SOLibHandle m_fabricSDKSOLibHandle;
      NameToInstMap m_nameToInstMap;
    };
  };
};

#endif //_FABRIC_PLUG_MANAGER_H
