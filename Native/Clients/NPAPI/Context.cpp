/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Clients/NPAPI/Context.h>
#include <Fabric/Clients/NPAPI/ViewPort.h>
#include <Fabric/Clients/NPAPI/IOManager.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/DG/NamedObject.h>
#include <Fabric/Core/DG/CodeManager.h>
#include <Fabric/Core/Plug/Manager.h>
#include <Fabric/Core/IO/Helpers.h>
#include <Fabric/Core/IO/Dir.h>
#include <Fabric/Base/JSON/Object.h>

namespace Fabric
{
  namespace NPAPI
  {
    RC::Handle<Context> Context::Create( RC::Handle<IOManager> const &ioManager, std::vector<std::string> const &pluginDirs )
    {
      return new NPAPI::Context( ioManager, pluginDirs );
    }
    
    Context::Context( RC::Handle<IOManager> const &ioManager, std::vector<std::string> const &pluginDirs )
      : DG::Context( ioManager, pluginDirs )
      , m_ioManager( ioManager )
    {
    }
    
    Context::~Context()
    {
      FABRIC_ASSERT( m_viewPorts.empty() );
    }

    void Context::registerViewPort( std::string const &name, ViewPort *viewPort )
    {
      FABRIC_CONFIRM( m_viewPorts.insert( ViewPorts::value_type( name, viewPort ) ).second );
    }
    
    void Context::unregisterViewPort( std::string const &name, ViewPort *viewPort )
    {
      ViewPorts::iterator it = m_viewPorts.find( name );
      FABRIC_ASSERT( it != m_viewPorts.end() );
      FABRIC_ASSERT( it->second == viewPort );
      m_viewPorts.erase( it );
    }

    RC::Handle<IOManager> Context::getIOManager() const
    {
      return m_ioManager;
    }

    void Context::jsonRoute(
      std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( dst.size() - dstOffset >= 1 && dst[dstOffset] == "VP" )
      {
        try
        {
          jsonRouteViewPorts( dst, dstOffset + 1, cmd, arg, resultJAG );
        }
        catch ( Exception e )
        {
          throw "'viewPorts': " + e;
        }
      }
      else return DG::Context::jsonRoute( dst, dstOffset, cmd, arg, resultJAG );
    }
    
    void Context::jsonExec(
      std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      DG::Context::jsonExec( cmd, arg, resultJAG );
    }

    void Context::jsonRouteViewPorts( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      if ( dst.size() - dstOffset == 1 )
      {
        std::string viewPortName = dst[dstOffset];
        ViewPorts::const_iterator it = m_viewPorts.find( viewPortName );
        if ( it == m_viewPorts.end() )
          throw Exception( "unroutable" );
        
        try
        {
          it->second->jsonExec( cmd, arg, resultJAG );
        }
        catch ( Exception e )
        {
          throw "command " + _(cmd) + ": " + e;
        }
      }
      else throw Exception( "unroutable" );
    }
    
    void Context::jsonDesc( Util::JSONGenerator &resultJG ) const
    {
      Util::JSONObjectGenerator resultJOG = resultJG.makeObject();
      Util::JSONGenerator memberJG = resultJOG.makeMember( "VP", 2 );
      jsonDescViewPorts( memberJG );
    }
    
    void Context::jsonDescViewPorts( Util::JSONGenerator &resultJG ) const
    {
      Util::JSONObjectGenerator resultJOG = resultJG.makeObject();
      for ( ViewPorts::const_iterator it=m_viewPorts.begin(); it!=m_viewPorts.end(); ++it )
      {
        Util::JSONGenerator viewPortJG = resultJOG.makeMember( it->first );
        it->second->jsonDesc( viewPortJG );
      }
    }

    std::string Context::queryUserFilePath( bool existingFile, std::string const &title, std::string const &defaultFilename, std::string const &extension ) const
    {
      if( m_viewPorts.empty() )
        throw Exception( "Error: viewport is not initialized" );

      return m_viewPorts.begin()->second->queryUserFilePath( existingFile, title, defaultFilename, extension );
    }
  };
};
