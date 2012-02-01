/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Clients/Python/Client.h>
#include <Fabric/Clients/Python/IOManager.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Base/JSON/Encoder.h>
#include <Fabric/EDK/EDK.h>
#include <Fabric/Core/IO/Helpers.h>
#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Core/IO/ResourceManager.h>
#include <Fabric/Core/IO/FileHandleManager.h>
#include <Fabric/Core/IO/FileHandleResourceProvider.h>
#include <Fabric/Core/Plug/Manager.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/MT/Util.h>

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

      RC::Handle<IO::Manager> ioManager = IOManager::Create( &Client::ScheduleAsyncUserCallback, NULL );
      RC::Handle<DG::Context> dgContext = DG::Context::Create( ioManager, pluginPaths, compileOptions, true, true );
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
      , m_closeMutex( "Python Client" )
      , m_closed( false )
      , m_scheduleAsyncMutex( "PyClient Schedule Async" )
    {
    }

    Client::~Client()
    {
      PassedStringMap::iterator it = m_passedStrings.begin();
      while ( it != m_passedStrings.end() )
      {
        m_passedStrings.erase( it );
        delete it->second;
        it++;
      }
      FABRIC_ASSERT( m_passedStrings.empty() );
    }

    void Client::setJSONNotifyCallback( void (*callback)(const char *) )
    {
      m_notifyCallback = callback;
      notifyInitialState();
    }

    void Client::notify( Util::SimpleString const &jsonEncodedNotifications ) const
    {
      m_notifyCallback( jsonEncodedNotifications.c_str() );
    }

    void Client::jsonExecAndAllocCStr( char const *data, size_t length, const char **str )
    {
      Util::SimpleString *jsonEncodedResults = new Util::SimpleString();
      JSON::Encoder resultJSON( jsonEncodedResults );

      jsonExec( data, length, resultJSON );

      *str = jsonEncodedResults->c_str();

      // keep a handle to the SimpleString for later deletion
      PassedStringMap::iterator i = m_passedStrings.find( *str );
      FABRIC_ASSERT( i == m_passedStrings.end() );
      m_passedStrings[ *str ] = jsonEncodedResults;
    }

    void Client::freeJsonCStr( char const *str )
    {
      PassedStringMap::iterator i = m_passedStrings.find( str );
      FABRIC_ASSERT( i != m_passedStrings.end() );
      m_passedStrings.erase( i );
      delete i->second;
    }

    void Client::close()
    {
      Util::Mutex::Lock lock( m_closeMutex );
      m_closed = true;
      m_closeCond.broadcast();
    }

    void Client::waitForClose()
    {
      Util::Mutex::Lock lock( m_closeMutex );
      if ( !m_closed )
        m_closeCond.wait( m_closeMutex );
    }

    void Client::ScheduleAsyncUserCallback(
        void *scheduleUserData,
        void (*callbackFunc)(void *),
        void *callbackFuncUserData
        )
    {
      (*callbackFunc)(callbackFuncUserData);
    }
  }
};
