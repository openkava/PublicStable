/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "Manager.h"
#include "Inst.h"

#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/AST/GlobalList.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/IO/Dir.h>
#include <Fabric/Base/JSON/Object.h>

namespace Fabric
{
  namespace Plug
  {
    RC::Handle<Manager> Manager::Instance()
    {
      static RC::Handle<Manager> instance;
      if ( !instance )
        instance = new Manager();
      return instance;
    }
    
    Manager::Manager()
      : m_loaded( false )
      //, m_fabricSDKSOLibHandle( invalidSOLibHandle )
    {
#if 0
      try
      {
        m_fabricSDKSOLibHandle = SOLibOpen( "FabricSDK", m_fabricSDKSOLibResolvedName, true, m_pluginDirs );
      }
      catch ( Exception e )
      {
        FABRIC_LOG( "%s", ("Warning: Unable to load "+_(m_fabricSDKSOLibResolvedName)+", plugin support will not be available").c_str() );
      }
#endif      
    }
    
    void Manager::loadBuiltInPlugins( std::vector<std::string> const &pluginDirs, RC::Handle<CG::Manager> const &cgManager )
    {
      if ( !m_loaded )
      {
#if 0
        std::string pluginPath = "";
        for ( size_t i=0; i<pluginDirs.size(); ++i )
        {
          if ( pluginPath.length() > 0 )
            pluginPath.append( ":" );
          pluginPath.append( pluginDirs[i] );
        }
        FABRIC_LOG( "Plugin path is "+_(pluginPath) );
#endif
   
        //if ( m_fabricSDKSOLibHandle != invalidSOLibHandle )
        {
          for ( size_t i=0; i<pluginDirs.size(); ++i )
          {
            RC::ConstHandle<IO::Dir> pluginsDir;
            try
            {
              pluginsDir = IO::Dir::Create( 0, pluginDirs[i], false );
            }
            catch ( Exception e )
            {
              FABRIC_LOG( "Warning: unable to open plugins directory "+_(pluginDirs[i]) );
              continue;
            }
            
            std::vector<std::string> files = pluginsDir->getFiles();
            for ( size_t i=0; i<files.size(); ++i )
            {
              std::string const &filename = files[i];
              size_t length = filename.length();
              if ( length > 9 && filename.substr( length-9, 9 ) == ".fpm.json" )
              {
                std::string fpmContents = pluginsDir->getFileContents( filename );
                try
                {
                  registerPlugin( filename.substr( 0, length-9 ), fpmContents, pluginDirs, cgManager );
                  FABRIC_LOG( "Loaded extension " + _(filename.c_str()) );
                }
                catch ( Exception e )
                {
                  FABRIC_LOG( "Extension '%s/%s': %s", pluginsDir->getFullPath().c_str(), filename.c_str(), e.getDesc().c_str() );
                }
              }
            }
          }
        }
        
        m_loaded = true;
      }
    }
    
    Manager::~Manager()
    {
      //if ( m_fabricSDKSOLibHandle )
      //  SOLibClose( m_fabricSDKSOLibHandle, m_fabricSDKSOLibResolvedName );
    }
    
    RC::ConstHandle<Inst> Manager::registerPlugin( std::string const &name, std::string const &jsonDesc, std::vector<std::string> const &pluginDirs, RC::Handle<CG::Manager> const &cgManager )
    {
      RC::Handle<Inst> result;
      
      NameToInstMap::const_iterator it = m_nameToInstMap.find( name );
      if ( it != m_nameToInstMap.end() )
      {
        result = it->second;
        if ( result->getJSONDesc() != jsonDesc )
          throw Exception( "a different plugin named "+_(name)+" is already registered" );
      }
      else
      {
        result = Inst::Create( name, jsonDesc, pluginDirs, cgManager );
        CG::Diagnostics diagnostics;
        result->getAST()->registerTypes( cgManager->getRTManager(), diagnostics );
        m_nameToInstMap.insert( NameToInstMap::value_type( name, result ) );
      }
      
      return result;
    }
      
    RC::ConstHandle<AST::GlobalList> Manager::getAST() const
    {
      RC::ConstHandle<AST::GlobalList> result;
      for ( NameToInstMap::const_iterator it=m_nameToInstMap.begin(); it!=m_nameToInstMap.end(); ++it )
        result = AST::GlobalList::Create( result, it->second->getAST() );
      return result;
    }
    
    void *Manager::llvmResolveExternalFunction( std::string const &name ) const
    {
      void *result = 0;
      for ( NameToInstMap::const_iterator it=m_nameToInstMap.begin(); it!=m_nameToInstMap.end(); ++it )
      {
        result = it->second->llvmResolveExternalFunction( name );
        if ( result )
          break;
      }
      return result;
    }

    RC::Handle<JSON::Object> Manager::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      for ( NameToInstMap::const_iterator it=m_nameToInstMap.begin(); it!=m_nameToInstMap.end(); ++it )
        result->set( it->first, it->second->jsonDesc() );
      return result;
    }
  };
};
