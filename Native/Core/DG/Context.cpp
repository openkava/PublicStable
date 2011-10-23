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
#include <Fabric/Core/RT/OpaqueDesc.h>
#include <Fabric/Core/RT/IntegerDesc.h>
#include <Fabric/Core/RT/StringDesc.h>
#include <Fabric/Core/RT/StructDesc.h>
#include <Fabric/Core/Plug/Manager.h>
#include <Fabric/Core/KL/Compiler.h>
#include <Fabric/Core/AST/GlobalList.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/IO/Manager.h>
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
#include <FabricThirdPartyLicenses/llvm/license.h>
#include <FabricThirdPartyLicenses/llvm/autoconf/license.h>
#include <FabricThirdPartyLicenses/llvm/lib/Support/license.h>
#include <FabricThirdPartyLicenses/llvm/projects/sample/autoconf/license.h>
#include <FabricThirdPartyLicenses/md5/license.h>
#include <FabricThirdPartyLicenses/libpng/license.h>
#include <FabricThirdPartyLicenses/liblas/license.h>

namespace Fabric
{
  namespace DG
  {
    Context::ContextMap Context::s_contextMap;
    
    RC::Handle<Context> Context::Create( RC::Handle<IO::Manager> const &ioManager, std::vector<std::string> const &pluginDirs )
    {
       return new Context( ioManager, pluginDirs );
    }
    
    RC::Handle<Context> Context::Bind( std::string const &contextID )
    {
      ContextMap::const_iterator it = s_contextMap.find( contextID );
      if ( it == s_contextMap.end() )
        throw Exception( "no context with ID " + _(contextID) );
      return it->second;
    }
    
    Context::Context( RC::Handle<IO::Manager> const &ioManager, std::vector<std::string> const &pluginDirs )
      : m_logCollector( LogCollector::Create( this ) )
      , m_rtManager( RT::Manager::Create( KL::Compiler::Create() ) )
      , m_ioManager( ioManager )
      , m_cgManager( CG::Manager::Create( m_rtManager ) )
      , m_codeManager( CodeManager::Create() )
      , m_notificationBracketCount( 0 )
      , m_pendingNotificationsMutex( "pending notifications" )
      , m_pendingNotificationsJSON( 0 )
    {
      registerCoreTypes();
      
      m_rtManager->setJSONCommandChannel( this );

      static const size_t contextIDByteCount = 96;
      uint8_t contextIDBytes[contextIDByteCount];
      Util::generateSecureRandomBytes( contextIDByteCount, contextIDBytes );
      std::string contextID = Util::encodeBase64( contextIDBytes, contextIDByteCount );
      m_contextMapIterator = s_contextMap.insert( ContextMap::value_type( contextID, this ) ).first;
    }
    
    Context::~Context()
    {
      FABRIC_ASSERT( !m_pendingNotificationsJSON );
      FABRIC_ASSERT( !m_pendingNotificationsJSONGenerator );
      FABRIC_ASSERT( !m_pendingNotificationsJSONArrayGenerator );
      
      FABRIC_ASSERT( m_clients.empty() );
      
      s_contextMap.erase( m_contextMapIterator );

      m_rtManager->setJSONCommandChannel( 0 );
    }
    
    std::string const &Context::getContextID() const
    {
      return m_contextMapIterator->first;
    }
    
    void Context::registerClient( Client *client )
    {
      FABRIC_CONFIRM( m_clients.insert( client ).second );
    }

    void Context::registerCoreTypes()
    {
      //FabricResource type: used by ResourceLoadNode
      RegisterFabricResource( m_rtManager );
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
      
      MT::Mutex::Lock pendingNotificationsMutexLock( m_pendingNotificationsMutex );
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
      if ( m_notificationBracketCount.decrementAndGetValue() == 0
        && MT::ThreadPool::Instance()->isMainThread()
        && m_pendingNotificationsJSON )
      {
        Util::SimpleString *pendingNotificationJSON;
        {
          MT::Mutex::Lock pendingNotificationsMutexLock( m_pendingNotificationsMutex );
          pendingNotificationJSON = m_pendingNotificationsJSON;
          delete m_pendingNotificationsJSONArrayGenerator;
          delete m_pendingNotificationsJSONGenerator;
          m_pendingNotificationsJSON = 0;
        }
        
        for ( Clients::const_iterator it=m_clients.begin(); it!=m_clients.end(); ++it )
          (*it)->notify( *pendingNotificationJSON );
        
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

      if ( dst.size() - dstOffset == 0 )
      {
        jsonExec( cmd, arg, resultJAG );
      }
      else
      {
        if ( dst[dstOffset] == "DG" )
          jsonRouteDG( dst, dstOffset + 1, cmd, arg, resultJAG );
        else if ( dst[dstOffset] == "RT" )
          m_rtManager->jsonRoute( dst, dstOffset + 1, cmd, arg, resultJAG );
        else if ( dst[dstOffset] == "IO" )
          m_ioManager->jsonRoute( dst, dstOffset + 1, cmd, arg, resultJAG );
        else throw Exception( "unroutable" );
      }
    }

    void Context::jsonExec(
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      throw Exception( "unknown command" );
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
  };
};
