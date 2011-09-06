/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_NPAPI_CONTEXT_H
#define _FABRIC_NPAPI_CONTEXT_H

#include <Fabric/Core/DG/Context.h>

#include <vector>
#include <map>

namespace Fabric
{
  namespace LIB
  {
    class Value;
    class Callback;
  };
  
  namespace NPAPI
  {
    class IOManager;
    class ViewPort;
    
    class Context : public DG::Context
    {
      typedef std::map< std::string, ViewPort * > ViewPorts;
      
    public:
    
      static RC::Handle<Context> Create( RC::Handle<IOManager> const &ioManager, std::vector<std::string> const &pluginDirs );
      
      void registerViewPort( std::string const &name, ViewPort *viewPort );
      void unregisterViewPort( std::string const &name, ViewPort *viewPort );
      
      virtual RC::ConstHandle<JSON::Value> jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      virtual RC::ConstHandle<JSON::Value> jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      virtual RC::Handle<JSON::Object> jsonDesc() const;
      
      RC::ConstHandle<JSON::Value> jsonRouteViewPorts( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      RC::Handle<JSON::Object> jsonDescViewPorts() const;

      RC::Handle<IOManager> getIOManager() const;

      std::string getUserFilePath( std::string const &defaultFilename, std::string const &extension );

    protected:
    
      Context( RC::Handle<IOManager> const &ioManager, std::vector<std::string> const &pluginDirs );
      ~Context();
      
    private:
      
      RC::Handle<IOManager> m_ioManager;
      ViewPorts m_viewPorts;
    };
  };
};

#endif
