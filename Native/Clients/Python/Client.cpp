/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Clients/Python/Client.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Core/Util/Log.h>
#include <Fabric/EDK/EDK.h>
#include <Fabric/Core/IO/Helpers.h>
#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Core/IO/Stream.h>
#include <Fabric/Core/Plug/Manager.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/DG/Context.h>

#include <vector>
#include <string>

namespace Fabric
{
  namespace Python
  {
    RC::Handle<Client> Client::Create()
    {
      std::vector<std::string> pluginPaths;
#if defined(FABRIC_OS_MACOSX)
      char const *home = getenv("HOME");
      if ( home && *home )
      {
        std::string homePath( home );
        pluginPaths.push_back( IO::JoinPath( homePath, "Library", "Fabric", "Exts" ) );
      }
      pluginPaths.push_back( "/Library/Fabric/Exts" );
#elif defined(FABRIC_OS_LINUX)
      char const *home = getenv("HOME");
      if ( home && *home )
      {
        std::string homePath( home );
        pluginPaths.push_back( IO::JoinPath( homePath, ".fabric", "Exts" ) );
      }
      pluginPaths.push_back( "/usr/lib/fabric/Exts" );
#elif defined(FABRIC_OS_WINDOWS)
      char const *appData = getenv("APPDATA");
      if ( appData && *appData )
      {
        std::string appDataDir(appData);
        pluginPaths.push_back( IO::JoinPath( appDataDir, "Fabric" , "Exts" ) );
      }
#endif

      CG::CompileOptions compileOptions;
      compileOptions.setGuarded( false );

      //RC::Handle<IO::Manager> ioManager = IOManager::Create();
      //RC::Handle<DG::Context> dgContext = DG::Context::Create( ioManager, pluginPaths, compileOptions, true, true );
      RC::Handle<DG::Context> dgContext = DG::Context::Create( NULL, pluginPaths, compileOptions, true, true );
#if defined(FABRIC_MODULE_OPENCL)
      OCL::registerTypes( dgContext->getRTManager() );
#endif

      Plug::Manager::Instance()->loadBuiltInPlugins( pluginPaths, dgContext->getCGManager(), DG::Context::GetCallbackStruct() );
      
      return Client::Create( dgContext );
    }

    RC::Handle<Client> Client::Create( RC::Handle<DG::Context> const &context )
    {
      return new Client( context );
    }
    
    Client::Client( RC::Handle<DG::Context> const &context )
      : DG::Client( context )
    {
    }
  }
};
