/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/DG/CodeManager.h>
#include <Fabric/Core/DG/Client.h>
#include <Fabric/Core/DG/NamedObject.h>
#include <Fabric/Core/DG/Node.h>
#include <Fabric/Core/DG/ResourceLoadNode.h>
#include <Fabric/Core/DG/Event.h>
#include <Fabric/Core/DG/EventHandler.h>
#include <Fabric/Core/DG/Operator.h>
#include <Fabric/Core/DG/LogCollector.h>
#include <Fabric/Core/CG/CompileOptions.h>
#include <Fabric/Core/RT/StringDesc.h>
#include <Fabric/Core/Plug/Manager.h>
#include <Fabric/Core/KL/Compiler.h>
#include <Fabric/Core/AST/GlobalList.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/RT/StringDesc.h>
#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Core/IO/FileHandleManager.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Base/JSON/Boolean.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Base/JSON/Encode.h>
#include <Fabric/Base/JSON/Decode.h>
#include <Fabric/Core/Util/Random.h>
#include <Fabric/Core/Util/Base64.h>
#include <Fabric/Core/Util/Timer.h>
#include <Fabric/Core/Build.h>
#include <Fabric/EDK/Common.h>
#include <FabricThirdPartyLicenses/llvm/license.h>
#include <FabricThirdPartyLicenses/llvm/autoconf/license.h>
#include <FabricThirdPartyLicenses/llvm/lib/Support/license.h>
#include <FabricThirdPartyLicenses/llvm/projects/sample/autoconf/license.h>
#include <FabricThirdPartyLicenses/md5/license.h>
#include <FabricThirdPartyLicenses/libpng/license.h>
#include <FabricThirdPartyLicenses/liblas/license.h>
#include <FabricThirdPartyLicenses/teem/license.h>

namespace Fabric
{
  namespace DG
  {
    Util::Mutex Context::s_contextMapMutex("Context::s_contextMapMutex");
    Context::ContextMap Context::s_contextMap;
    Context *s_activeContext = NULL;
    
    Context::ActiveContextBracket::ActiveContextBracket( Context* currContext )
      : m_prevContext( s_activeContext )
      , m_currContext( currContext )
    {
      s_activeContext = m_currContext;
    }

    Context::ActiveContextBracket::~ActiveContextBracket()
    {
      FABRIC_ASSERT( s_activeContext == m_currContext );
      s_activeContext = m_prevContext;
    }

    RC::Handle<Context> Context::Create(
      RC::Handle<IO::Manager> const &ioManager,
      std::vector<std::string> const &pluginDirs,
      CG::CompileOptions const &compileOptions,
      bool optimizeSynchronously
      )
    {
       return new Context( ioManager, pluginDirs, compileOptions, optimizeSynchronously );
    }
    
    RC::Handle<Context> Context::Bind( std::string const &contextID )
    {
      Util::Mutex::Lock contextMapLock( s_contextMapMutex );
      ContextMap::const_iterator it = s_contextMap.find( contextID );
      if ( it == s_contextMap.end() )
        throw Exception( "no context with ID " + _(contextID) );
      return it->second;
    }
    
    Context::Context(
      RC::Handle<IO::Manager> const &ioManager,
      std::vector<std::string> const &pluginDirs,
      CG::CompileOptions const &compileOptions,
      bool optimizeSynchronously
      )
      : m_mutex( "Context::Context" )
      , m_logCollector( LogCollector::Create( this ) )
      , m_rtManager( RT::Manager::Create( KL::Compiler::Create() ) )
      , m_ioManager( ioManager )
      , m_cgManager( CG::Manager::Create( m_rtManager ) )
      , m_compileOptions( compileOptions )
      , m_codeManager( CodeManager::Create( &m_compileOptions, optimizeSynchronously ) )
      , m_clientsMutex( "Context::m_clients" )
      , m_notificationBracketCount( 0 )
      , m_pendingNotificationsMutex( "pending notifications" )
      , m_pendingNotificationsJSON( 0 )
      , m_gcContainer( this )
      , m_mrInterface( &m_gcContainer, m_rtManager )
      , m_klcInterface( &m_gcContainer, m_cgManager, m_compileOptions )
    {
      registerCoreTypes();
      
      m_rtManager->setJSONCommandChannel( this );

      static const size_t contextIDByteCount = 96;
      uint8_t contextIDBytes[contextIDByteCount];
      Util::generateSecureRandomBytes( contextIDByteCount, contextIDBytes );
      m_contextID = Util::encodeBase64( contextIDBytes, contextIDByteCount );
      Util::Mutex::Lock contextMapLock( s_contextMapMutex );
      s_contextMap.insert( ContextMap::value_type( m_contextID, this ) ).first;
    }
    
    Context::~Context()
    {
      FABRIC_ASSERT( !m_pendingNotificationsJSON );
      
      FABRIC_ASSERT( m_clients.empty() );
      FABRIC_ASSERT( s_activeContext != this );//Should always use ActiveContextBracket

      {
        Util::Mutex::Lock contextMapLock( s_contextMapMutex );
        
        s_contextMap.erase( s_contextMap.find( m_contextID ) );
      }
      
      m_rtManager->setJSONCommandChannel( 0 );
    }
    
    std::string const &Context::getContextID() const
    {
      return m_contextID;
    }
    
    void Context::registerClient( Client *client )
    {
      Util::Mutex::Lock clientLock( m_clientsMutex );
      FABRIC_CONFIRM( m_clients.insert( client ).second );
    }

    void Context::registerCoreTypes()
    {
      RegisterFabricResourceType( m_rtManager );
    }
    
    void Context::jsonNotify(
      std::vector<std::string> const &srcs,
      char const *cmdData,
      size_t cmdLength,
      Util::SimpleString const *argJSON
      )
    {
      NotificationBracket notificationBracket( this );
      
      Util::SimpleString json;
      {
        Util::JSONGenerator jg( &json );
        Util::JSONObjectGenerator jog = jg.makeObject();
        
        {
          Util::JSONGenerator srcsJG = jog.makeMember( "src", 3 );
          Util::JSONArrayGenerator srcsJAG = srcsJG.makeArray();
          for ( std::vector<std::string>::const_iterator it=srcs.begin(); it!=srcs.end(); ++it )
          {
            Util::JSONGenerator srcJG = srcsJAG.makeElement();
            srcJG.makeString( *it );
          }
        }
        
        {
          Util::JSONGenerator cmdJG = jog.makeMember( "cmd", 3 );
          cmdJG.makeString( cmdData, cmdLength );
        }
        
        if ( argJSON )
        {
          Util::JSONGenerator argJG = jog.makeMember( "arg", 3 );
          argJG.appendJSON( *argJSON );
        }
      }
      
      Util::Mutex::Lock pendingNotificationsMutexLock( m_pendingNotificationsMutex );
      if ( !m_pendingNotificationsJSON )
      {
        m_pendingNotificationsJSON = new Util::SimpleString;
        m_pendingNotificationsJSONGenerator = new Util::JSONGenerator( m_pendingNotificationsJSON );
        m_pendingNotificationsJSONArrayGenerator = m_pendingNotificationsJSONGenerator->newArray();
      }
      m_pendingNotificationsJSONArrayGenerator->makeElement().appendJSON( json );
    }
    
    void Context::unregisterClient( Client *client )
    {
      Util::Mutex::Lock clientLock( m_clientsMutex );
      Clients::iterator it = m_clients.find( client );
      FABRIC_ASSERT( it != m_clients.end() );
      m_clients.erase( it );
    }

    void Context::openNotificationBracket()
    {
      m_notificationBracketCount.increment();
    }
    
    void Context::closeNotificationBracket()
    {
      ActiveContextBracket activeContextBracket( this );

      if ( m_notificationBracketCount.decrementAndGetValue() == 0
        && m_pendingNotificationsJSON )
      {
        Util::SimpleString *pendingNotificationJSON;
        {
          Util::Mutex::Lock pendingNotificationsMutexLock( m_pendingNotificationsMutex );
          pendingNotificationJSON = m_pendingNotificationsJSON;
          delete m_pendingNotificationsJSONArrayGenerator;
          delete m_pendingNotificationsJSONGenerator;
          m_pendingNotificationsJSON = 0;
        }

        {
          Util::Mutex::Lock clientLock( m_clientsMutex );
          // [pzion 20111130] This is a bit insane: in a strange case I haven't
          // tracked down yet, notifying a client can cause it to become
          // unregistered.  We work around this problem by saving the next iterator.
          Clients::const_iterator it = m_clients.begin();
          while( it != m_clients.end() )
          {
            Client *client = *it++;
            client->notify( *pendingNotificationJSON );
          }
        }

        delete pendingNotificationJSON;
      }
    }
    
    RC::Handle<MT::LogCollector> Context::getLogCollector() const
    {
      return m_logCollector;
    }
    
    RC::Handle<RT::Manager> Context::getRTManager() const
    {
      return m_rtManager;
    }
    
    RC::Handle<CG::Manager> Context::getCGManager() const
    {
      return m_cgManager;
    }
    
    RC::Handle<IO::Manager> Context::getIOManager() const
    {
      return m_ioManager;
    }
    
    RC::Handle<CodeManager> Context::getCodeManager() const
    {
      return m_codeManager;
    }
    
    Context::NamedObjectMap &Context::getNamedObjectRegistry() const
    {
      return m_namedObjectRegistry;
    }
              
    RC::Handle<NamedObject> Context::getNamedObject( std::string const &name ) const
    {
      NamedObjectMap::const_iterator it = m_namedObjectRegistry.find( name );
      if ( it == m_namedObjectRegistry.end() )
        throw Exception( "no NamedObject named " + _(name) );
      return it->second;
    }
    
    RC::Handle<Container> Context::getContainer( std::string const &name ) const
    {
      RC::Handle<Container> container;
      try
      {
        RC::Handle<NamedObject> namedObject = getNamedObject( name );
        if ( !namedObject->isContainer() )
          throw Exception("");
        container = RC::Handle<Container>::StaticCast( namedObject );
      }
      catch ( Exception e )
      {
        throw Exception( "no Container named " + _(name) );
      }
      return container;
    }
    
    RC::Handle<Operator> Context::getOperator( std::string const &name ) const
    {
      RC::Handle<Operator> operator_;
      try
      {
        RC::Handle<NamedObject> namedObject = getNamedObject( name );
        if ( !namedObject->isOperator() )
          throw Exception("");
        operator_ = RC::Handle<Operator>::StaticCast( namedObject );
      }
      catch ( Exception e )
      {
        throw Exception( "no Operator named " + _(name) );
      }
      return operator_;
    }
    
    RC::Handle<Node> Context::getNode( std::string const &name ) const
    {
      RC::Handle<Node> node;
      try
      {
        RC::Handle<Container> container = getContainer( name );
        if ( !container->isNode() )
          throw Exception("");
        node = RC::Handle<Node>::StaticCast( container );
      }
      catch ( Exception e )
      {
        throw Exception( "no Node named " + _(name) );
      }
      return node;
    }
    
    RC::Handle<Event> Context::getEvent( std::string const &name ) const
    {
      RC::Handle<Event> event;
      try
      {
        RC::Handle<Container> container = getContainer( name );
        if ( !container->isEvent() )
          throw Exception("");
        event = RC::Handle<Event>::StaticCast( container );
      }
      catch ( Exception e )
      {
        throw Exception( "no Event named " + _(name) );
      }
      return event;
    }
    
    RC::Handle<EventHandler> Context::getEventHandler( std::string const &name ) const
    {
      RC::Handle<EventHandler> eventHandler;
      try
      {
        RC::Handle<Container> container = getContainer( name );
        if ( !container->isEventHandler() )
          throw Exception("");
        eventHandler = RC::Handle<EventHandler>::StaticCast( container );
      }
      catch ( Exception e )
      {
        throw Exception( "no EventHandler named " + _(name) );
      }
      return eventHandler;
    }
    
    static bool IsExpired()
    {
      static bool haveIsExpired = false;
      static bool isExpired;
      if ( !haveIsExpired )
      {
        time_t currentTime = time( NULL );
        isExpired = currentTime >= buildExpiry;
        haveIsExpired = true;
      }
      return isExpired;
    }
    
    void Context::jsonRoute(
      std::vector<std::string> const &dst,
      size_t dstOffset,
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( IsExpired() )
      {
        static char const *expiryMessage = "This build of Fabric has expired and will no longer work!\nContact customer support to obtain a more recent build.";
        FABRIC_LOG( expiryMessage );
        throw Exception( expiryMessage );
      }
      
      Util::Mutex::Lock mutexLock( m_mutex );
      ActiveContextBracket activeContextBracket( this );

      if ( dst.size() - dstOffset == 0 )
      {
        jsonExec( cmd, arg, resultJAG );
      }
      else
      {
        std::string const &first = dst[dstOffset];
        if ( first == "DG" )
          jsonRouteDG( dst, dstOffset + 1, cmd, arg, resultJAG );
        else if ( first == "RT" )
          m_rtManager->jsonRoute( dst, dstOffset + 1, cmd, arg, resultJAG );
        else if ( first == "IO" )
          m_ioManager->jsonRoute( dst, dstOffset + 1, cmd, arg, resultJAG );
        else if ( first == "MR" )
        {
          MT::TLSLogCollectorAutoSet logCollector( m_logCollector );
          m_mrInterface.jsonRoute( dst, dstOffset + 1, cmd, arg, resultJAG );
        }
        else if ( first == "KLC" )
        {
          MT::TLSLogCollectorAutoSet logCollector( m_logCollector );
          m_klcInterface.jsonRoute( dst, dstOffset + 1, cmd, arg, resultJAG );
        }
        else throw Exception( "unroutable" );
      }
    }

    void Context::jsonExec(
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( cmd == "getMemoryUsage" )
        jsonExecGetMemoryUsage( resultJAG );
      else throw Exception( "unknown command" );
    }
    
    static void jsonDescLicenses_llvm_projects_sample_autoconf( Util::JSONGenerator &resultJG )
    {
      Util::JSONObjectGenerator resultJOG = resultJG.makeObject();
      Util::JSONGenerator memberJG = resultJOG.makeMember( ThirdPartyLicenses::llvm::projects::sample::autoconf::filename );
      memberJG.makeString( ThirdPartyLicenses::llvm::projects::sample::autoconf::text );
    }
    
    static void jsonDescLicenses_llvm_projects_sample( Util::JSONGenerator &resultJG )
    {
      Util::JSONObjectGenerator resultJOG = resultJG.makeObject();
      Util::JSONGenerator memberJG = resultJOG.makeMember( "autoconf", 8 );
      jsonDescLicenses_llvm_projects_sample_autoconf( memberJG );
    }
    
    static void jsonDescLicenses_llvm_projects( Util::JSONGenerator &resultJG )
    {
      Util::JSONObjectGenerator resultJOG = resultJG.makeObject();
      Util::JSONGenerator memberJG = resultJOG.makeMember( "sample", 6 );
      jsonDescLicenses_llvm_projects_sample( memberJG );
    }
    
    static void jsonDescLicenses_llvm_lib_Support( Util::JSONGenerator &resultJG )
    {
      Util::JSONObjectGenerator resultJOG = resultJG.makeObject();
      Util::JSONGenerator memberJG = resultJOG.makeMember( ThirdPartyLicenses::llvm::lib::Support::filename );
      memberJG.makeString( ThirdPartyLicenses::llvm::lib::Support::text );
    }
    
    static void jsonDescLicenses_llvm_lib( Util::JSONGenerator &resultJG )
    {
      Util::JSONObjectGenerator resultJOG = resultJG.makeObject();
      Util::JSONGenerator memberJG = resultJOG.makeMember( "Support", 7 );
      jsonDescLicenses_llvm_lib_Support( memberJG );
    }
    
    static void jsonDescLicenses_llvm_autoconf( Util::JSONGenerator &resultJG )
    {
      Util::JSONObjectGenerator resultJOG = resultJG.makeObject();
      Util::JSONGenerator memberJG = resultJOG.makeMember( ThirdPartyLicenses::llvm::autoconf::filename );
      memberJG.makeString( ThirdPartyLicenses::llvm::autoconf::text );
    }
    
    static void jsonDescLicenses_llvm( Util::JSONGenerator &resultJG )
    {
      Util::JSONObjectGenerator resultJGObject = resultJG.makeObject();
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( ThirdPartyLicenses::llvm::filename );
        memberJG.makeString( ThirdPartyLicenses::llvm::text );
      }
      {
        Util::JSONGenerator libJG = resultJGObject.makeMember( "lib", 3 );
        jsonDescLicenses_llvm_lib( libJG );
      }
      {
        Util::JSONGenerator projectsJG = resultJGObject.makeMember( "projects", 8 );
        jsonDescLicenses_llvm_projects( projectsJG );
      }
      {
        Util::JSONGenerator autoconfJG = resultJGObject.makeMember( "autoconf", 8 );
        jsonDescLicenses_llvm_autoconf( autoconfJG );
      }
    }
    
    static void jsonDescLicenses_libpng( Util::JSONGenerator &resultJG )
    {
      Util::JSONObjectGenerator resultJOG = resultJG.makeObject();
      Util::JSONGenerator memberJG = resultJOG.makeMember( ThirdPartyLicenses::libpng::filename );
      memberJG.makeString( ThirdPartyLicenses::libpng::text );
    }
    
    static void jsonDescLicenses_md5( Util::JSONGenerator &resultJG )
    {
      Util::JSONObjectGenerator resultJOG = resultJG.makeObject();
      Util::JSONGenerator memberJG = resultJOG.makeMember( ThirdPartyLicenses::md5::filename );
      memberJG.makeString( ThirdPartyLicenses::md5::text );
    }

    static void jsonDescLicenses_liblas( Util::JSONGenerator &resultJG )
    {
      Util::JSONObjectGenerator resultJOG = resultJG.makeObject();
      Util::JSONGenerator memberJG = resultJOG.makeMember( ThirdPartyLicenses::liblas::filename );
      memberJG.makeString( ThirdPartyLicenses::liblas::text );
    }

    static void jsonDescLicenses_teem( Util::JSONGenerator &resultJG )
    {
      Util::JSONObjectGenerator resultJOG = resultJG.makeObject();
      Util::JSONGenerator memberJG = resultJOG.makeMember( ThirdPartyLicenses::teem::filename );
      memberJG.makeString( ThirdPartyLicenses::teem::text );
    }

    static void jsonDescLicenses( Util::JSONGenerator &resultJG )
    {
      Util::JSONObjectGenerator resultJGObject = resultJG.makeObject();
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "llvm", 4 );
        jsonDescLicenses_llvm( memberJG );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "md5", 3 );
        jsonDescLicenses_md5( memberJG );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "libpng", 6 );
        jsonDescLicenses_libpng( memberJG );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "liblas", 6 );
        jsonDescLicenses_liblas( memberJG );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "teem", 4 );
        jsonDescLicenses_liblas( memberJG );
      }
    }

    static void jsonDescBuild( Util::JSONGenerator &resultJG )
    {
      Util::JSONObjectGenerator resultJGObject = resultJG.makeObject();
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "isExpired", 9 );
        memberJG.makeBoolean( IsExpired() );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "name", 4 );
        memberJG.makeString( buildName );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "pureVersion", 11 );
        memberJG.makeString( buildPureVersion );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "fullVersion", 11 );
        memberJG.makeString( buildFullVersion );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "desc", 4 );
        memberJG.makeString( buildDesc );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "copyright", 9 );
        memberJG.makeString( buildCopyright );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "url", 3 );
        memberJG.makeString( buildURL );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "os", 2 );
        memberJG.makeString( buildOS );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "arch", 4 );
        memberJG.makeString( buildArch );
      }
    }

    void Context::jsonDesc( Util::JSONGenerator &resultJG ) const
    {
      Util::JSONObjectGenerator resultJGObject = resultJG.makeObject();
      jsonDesc( resultJGObject );
    }
    
    void Context::jsonDesc( Util::JSONObjectGenerator &resultJGObject ) const
    {
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "contextID", 9 );
        memberJG.makeString( getContextID() );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "DG", 2 );
        jsonDescDG( memberJG );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "RT", 2 );
        m_rtManager->jsonDesc( memberJG );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "EX", 2 );
        Plug::Manager::Instance()->jsonDesc( memberJG );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "licenses", 8 );
        jsonDescLicenses( memberJG );
      }
      {
        Util::JSONGenerator memberJG = resultJGObject.makeMember( "build", 5 );
        jsonDescBuild( memberJG );
      }
    }
    
    void Context::jsonExecGetMemoryUsage( Util::JSONArrayGenerator &resultJAG ) const
    {
      Util::JSONGenerator jg = resultJAG.makeElement();
      Util::JSONObjectGenerator jog = jg.makeObject();
      Util::JSONGenerator dgJG = jog.makeMember( "DG" );
      jsonDGGetMemoryUsage( dgJG );
    }
    
    void Context::jsonDGGetMemoryUsage( Util::JSONGenerator &jg ) const
    {
      Util::JSONObjectGenerator jog = jg.makeObject();
      for ( NamedObjectMap::const_iterator it = m_namedObjectRegistry.begin(); it != m_namedObjectRegistry.end(); ++it )
      {
        Util::JSONGenerator namedObjectJG = jog.makeMember( it->first );
        it->second->jsonGetMemoryUsage( namedObjectJG );
      }
    }

    void Context::jsonRouteDG(
      std::vector<std::string> const &dst,
      size_t dstOffset,
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( dst.size() - dstOffset == 0 )
        jsonExecDG( cmd, arg, resultJAG );
      else
      {
        std::string const &namedObjectName = dst[1];
        NamedObjectMap::const_iterator it = m_namedObjectRegistry.find( namedObjectName );
        if ( it == m_namedObjectRegistry.end() )
          throw Exception( "NamedObject "+_(namedObjectName)+" not found" );
        it->second->jsonRoute( dst, dstOffset + 1, cmd, arg, resultJAG );
      }
    }

    void Context::jsonExecDG(
      std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( cmd == "createOperator" )
        Operator::jsonExecCreate( arg, this, resultJAG );
      else if ( cmd == "createNode" )
        Node::jsonExecCreate( arg, this, resultJAG );
      else if ( cmd == "createResourceLoadNode" )
        ResourceLoadNode::jsonExecCreate( arg, this, resultJAG );
      else if ( cmd == "createEvent" )
        Event::jsonExecCreate( arg, this, resultJAG );
      else if ( cmd == "createEventHandler" )
        EventHandler::jsonExecCreate( arg, this, resultJAG );
      else throw Exception( "unknown command" );
    }

    void Context::jsonDescDG( Util::JSONGenerator &resultJG ) const
    {
      Util::JSONObjectGenerator resultJGObject = resultJG.makeObject();
      for ( NamedObjectMap::const_iterator it=m_namedObjectRegistry.begin(); it!=m_namedObjectRegistry.end(); ++it )
      {
        std::string const &namedObjectName = it->first;
        RC::Handle<NamedObject> const &namedObject = it->second;
        {
          Util::JSONGenerator memberJG = resultJGObject.makeMember( namedObjectName );
          namedObject->jsonDesc( memberJG );
        }
      }
    }
    
    std::string const Context::s_wrapFabricClientJSSource(
#include <Fabric/Core/DG/FABRIC.Wrappers.js.inc>
);

    static void throwException( size_t length, char const *data )
    {
      throw Exception( length, data );
    }

    Context* GetActiveContext()
    {
      FABRIC_ASSERT( s_activeContext != NULL );
      if( s_activeContext == NULL )
        throw Exception( "Unexpected error: no active Fabric context" );

      return s_activeContext;
    }

    void FileHandleCreateFromPath( void *stringData, char const *filePathCString, bool folder, bool readOnly )
    {
      std::string handle = GetActiveContext()->getIOManager()->getFileHandleManager()->createHandle( filePathCString, folder, readOnly );
      RC::ConstHandle<RT::StringDesc> stringDesc = GetActiveContext()->getRTManager()->getStringDesc();
      stringDesc->setValue( handle.data(), handle.length(), stringData );
    }

    void FileGetPath( void const *stringData, void *pathStringData )
    {
      RC::ConstHandle<RT::StringDesc> stringDesc = GetActiveContext()->getRTManager()->getStringDesc();
      std::string handle( stringDesc->getValueData( stringData ), stringDesc->getValueLength( stringData ) );
      std::string path = GetActiveContext()->getIOManager()->getFileHandleManager()->getPath( handle );
      stringDesc->setValue( path.data(), path.length(), pathStringData );
    }

    bool FileHandleIsValid( void const *stringData )
    {
      RC::ConstHandle<RT::StringDesc> stringDesc = GetActiveContext()->getRTManager()->getStringDesc();
      std::string handle( stringDesc->getValueData( stringData ), stringDesc->getValueLength( stringData ) );
      return GetActiveContext()->getIOManager()->getFileHandleManager()->isValid( handle );
    }

    bool FileHandleIsReadOnly( void const *stringData )
    {
      RC::ConstHandle<RT::StringDesc> stringDesc = GetActiveContext()->getRTManager()->getStringDesc();
      std::string handle( stringDesc->getValueData( stringData ), stringDesc->getValueLength( stringData ) );
      return GetActiveContext()->getIOManager()->getFileHandleManager()->isReadOnly( handle );
    }

    bool FileHandleIsFolder( void const *stringData )
    {
      RC::ConstHandle<RT::StringDesc> stringDesc = GetActiveContext()->getRTManager()->getStringDesc();
      std::string handle( stringDesc->getValueData( stringData ), stringDesc->getValueLength( stringData ) );
      return GetActiveContext()->getIOManager()->getFileHandleManager()->isFolder( handle );
    }

    bool FileHandleTargetExists( void const *stringData )
    {
      RC::ConstHandle<RT::StringDesc> stringDesc = GetActiveContext()->getRTManager()->getStringDesc();
      std::string handle( stringDesc->getValueData( stringData ), stringDesc->getValueLength( stringData ) );
      return GetActiveContext()->getIOManager()->getFileHandleManager()->targetExists( handle );
    }

    void FileHandleEnsureTargetExists( void const *stringData )
    {
      RC::ConstHandle<RT::StringDesc> stringDesc = GetActiveContext()->getRTManager()->getStringDesc();
      std::string handle( stringDesc->getValueData( stringData ), stringDesc->getValueLength( stringData ) );
      GetActiveContext()->getIOManager()->getFileHandleManager()->ensureTargetExists( handle );
    }

    EDK::Callbacks Context::GetCallbackStruct()
    {
      Fabric::EDK::Callbacks callbacks;
      callbacks.m_malloc = malloc;
      callbacks.m_realloc = realloc;
      callbacks.m_free = free;
      callbacks.m_throwException = throwException;
      callbacks.m_fileHandleCreateFromPath = FileHandleCreateFromPath;
      callbacks.m_fileGetPath = FileGetPath;
      callbacks.m_fileHandleIsValid = FileHandleIsValid;
      callbacks.m_fileHandleIsReadOnly = FileHandleIsReadOnly;
      callbacks.m_fileHandleIsFolder = FileHandleIsFolder;
      callbacks.m_fileHandleTargetExists = FileHandleTargetExists;
      callbacks.m_fileHandleEnsureTargetExists = FileHandleEnsureTargetExists;

      return callbacks;
    }
  };
};
