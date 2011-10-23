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
  namespace Util
  {
    class JSONArrayGenerator;
  };
  
  namespace LIB
  {
    class Value;
    class Callback;
  };

  namespace IO
  {
    class Dir;
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
      
      virtual void jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      virtual void jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      virtual void jsonDesc( Util::JSONGenerator &resultJG ) const;
      
      void jsonRouteViewPorts( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonDescViewPorts( Util::JSONGenerator &resultJG ) const;

      RC::Handle<IOManager> getIOManager() const;

      std::string queryUserFilePath( bool existingFile, std::string const &title, std::string const &defaultFilename, std::string const &extension ) const;

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
