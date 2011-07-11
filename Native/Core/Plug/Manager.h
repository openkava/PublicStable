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
    
      static RC::Handle<Manager> Create( DG::Context *dgContext, std::vector<std::string> const &pluginDirs )
      {
        return new Manager( dgContext, pluginDirs );
      }
      
      void loadBuiltInPlugins();
      
      NameToInstMap const &getRegisteredPlugins() const
      {
        return m_nameToInstMap;
      }
      
      RC::ConstHandle<Inst> registerPlugin( std::string const &name, std::string const &jsonDesc );
      
      void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder ) const;
      void *llvmResolveExternalFunction( std::string const &name ) const;

      virtual RC::Handle<JSON::Object> jsonDesc() const;

    protected:
    
      Manager( DG::Context *dgContext, std::vector<std::string> const &pluginDirs );
      ~Manager();
      
    private:
    
      DG::Context *m_dgContext; // [pzion 20110309] We are owned by the context
      std::vector<std::string> m_pluginDirs;
      std::string m_fabricSDKSOLibResolvedName;
      //SOLibHandle m_fabricSDKSOLibHandle;
      NameToInstMap m_nameToInstMap;
    };
  };
};

#endif //_FABRIC_PLUG_MANAGER_H
