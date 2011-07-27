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
#include <Fabric/Core/RT/SizeDesc.h>
#include <Fabric/Core/RT/StringDesc.h>
#include <Fabric/Core/RT/StructDesc.h>
#include <Fabric/Core/Plug/Manager.h>
#include <Fabric/Core/KL/Compiler.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/IO/Manager.h>
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
    {
      registerCoreTypes();
      m_plugManager = Plug::Manager::Create( this, pluginDirs );
      m_rtManager->setJSONCommandChannel( this );

      static const size_t contextIDByteCount = 96;
      uint8_t contextIDBytes[contextIDByteCount];
      Util::generateSecureRandomBytes( contextIDByteCount, contextIDBytes );
      std::string contextID = Util::encodeBase64( contextIDBytes, contextIDByteCount );
      m_contextMapIterator = s_contextMap.insert( ContextMap::value_type( contextID, this ) ).first;
    }
    
    Context::~Context()
    {
      FABRIC_ASSERT( !m_pendingNotifications );
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
      RT::StructMemberInfoVector memberInfos;
      memberInfos.resize(4);
      memberInfos[0].name = "data";
      memberInfos[0].desc = m_rtManager->getDataDesc();
      memberInfos[1].name = "dataSize";
      memberInfos[1].desc = m_rtManager->getSizeDesc();
      memberInfos[2].name = "mimeType";
      memberInfos[2].desc = m_rtManager->getStringDesc();
      memberInfos[3].name = "extension";
      memberInfos[3].desc = m_rtManager->getStringDesc();
      m_rtManager->registerStruct( "FabricResource", memberInfos );
    }
    
    void Context::jsonNotify( std::vector<std::string> const &src, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      NotificationBracket notificationBracket( this );
      
      RC::Handle<JSON::Array> srcJSONArray = JSON::Array::Create();
      for ( size_t i=0; i<src.size(); ++i )
        srcJSONArray->push_back( JSON::String::Create( src[i] ) );
      
      RC::Handle<JSON::Object> notification = JSON::Object::Create();
      notification->set( "src", srcJSONArray );
      notification->set( "cmd", JSON::String::Create( cmd ) );
      if ( arg )
        notification->set( "arg", arg );
      
      MT::Mutex::Lock pendingNotificationsMutexLock( m_pendingNotificationsMutex );
      if ( !m_pendingNotifications )
        m_pendingNotifications = JSON::Array::Create();
      m_pendingNotifications->push_back( notification );
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
        && m_pendingNotifications )
      {
        std::string jsonEncodedNotifications;
        jsonEncodedNotifications = JSON::encode( m_pendingNotifications );
        m_pendingNotifications = 0;
        
        for ( Clients::const_iterator it=m_clients.begin(); it!=m_clients.end(); ++it )
          (*it)->notify( jsonEncodedNotifications );
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

    RC::ConstHandle<JSON::Value> Context::jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      time_t currentTime = time( NULL );
      if ( currentTime >= buildExpiry )
      {
        static char const *expiryMessage = "This build of Fabric has expired and will no longer work!\nContact customer support to obtain a more recent build.";
        FABRIC_LOG( expiryMessage );
        throw Exception( expiryMessage );
      }

      RC::ConstHandle<JSON::Value> result;

      if ( dst.size() - dstOffset == 0 )
      {
        result = jsonExec( cmd, arg );
      }
      else
      {
        if ( dst[dstOffset] == "DG" )
          result = jsonRouteDG( dst, dstOffset + 1, cmd, arg );
        else if ( dst[dstOffset] == "RT" )
          result = m_rtManager->jsonRoute( dst, dstOffset + 1, cmd, arg );
        else throw Exception( "unroutable" );
      }
      
      return result;
    }

    RC::ConstHandle<JSON::Value> Context::jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      throw Exception( "unknown command" );
    }
    
    static RC::Handle<JSON::Object> jsonDescLicenses_llvm_projects_sample_autoconf()
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( ThirdPartyLicenses::llvm::projects::sample::autoconf::filename, JSON::String::Create( ThirdPartyLicenses::llvm::projects::sample::autoconf::text ) );
      return result;
    }
    
    static RC::Handle<JSON::Object> jsonDescLicenses_llvm_projects_sample()
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( "autoconf", jsonDescLicenses_llvm_projects_sample_autoconf() );
      return result;
    }
    
    static RC::Handle<JSON::Object> jsonDescLicenses_llvm_projects()
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( "sample", jsonDescLicenses_llvm_projects_sample() );
      return result;
    }
    
    static RC::Handle<JSON::Object> jsonDescLicenses_llvm_lib_Support()
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( ThirdPartyLicenses::llvm::lib::Support::filename, JSON::String::Create( ThirdPartyLicenses::llvm::lib::Support::text ) );
      return result;
    }
    
    static RC::Handle<JSON::Object> jsonDescLicenses_llvm_lib()
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( "Support", jsonDescLicenses_llvm_lib_Support() );
      return result;
    }
    
    static RC::Handle<JSON::Object> jsonDescLicenses_llvm_autoconf()
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( ThirdPartyLicenses::llvm::autoconf::filename, JSON::String::Create( ThirdPartyLicenses::llvm::autoconf::text ) );
      return result;
    }
    
    static RC::Handle<JSON::Object> jsonDescLicenses_llvm()
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( ThirdPartyLicenses::llvm::filename, JSON::String::Create( ThirdPartyLicenses::llvm::text ) );
      result->set( "lib", jsonDescLicenses_llvm_lib() );
      result->set( "projects", jsonDescLicenses_llvm_projects() );
      result->set( "autoconf", jsonDescLicenses_llvm_autoconf() );
      return result;
    }
    
    static RC::Handle<JSON::Object> jsonDescLicenses_md5()
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( ThirdPartyLicenses::libpng::filename, JSON::String::Create( ThirdPartyLicenses::libpng::text ) );
      return result;
    }
    
    static RC::Handle<JSON::Object> jsonDescLicenses_libpng()
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( ThirdPartyLicenses::md5::filename, JSON::String::Create( ThirdPartyLicenses::md5::text ) );
      return result;
    }
    
    static RC::Handle<JSON::Object> jsonDescLicenses()
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( "llvm", jsonDescLicenses_llvm() );
      result->set( "md5", jsonDescLicenses_md5() );
      result->set( "libpng", jsonDescLicenses_libpng() );
      return result;
    }

    RC::Handle<JSON::Object> Context::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( "contextID", JSON::String::Create( getContextID() ) );
      result->set( "DG", jsonDescDG() );
      result->set( "RT", m_rtManager->jsonDesc() );
      result->set( "EX", Plug::Manager::Instance()->jsonDesc() );
      result->set( "licenses", jsonDescLicenses() );
      return result;
    }

    RC::ConstHandle<JSON::Value> Context::jsonRouteDG( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Value> result;

      if ( dst.size() - dstOffset == 0 )
        result = jsonExecDG( cmd, arg );
      else
      {
        std::string const &namedObjectName = dst[1];
        NamedObjectMap::const_iterator it = m_namedObjectRegistry.find( namedObjectName );
        if ( it == m_namedObjectRegistry.end() )
          throw Exception( "NamedObject "+_(namedObjectName)+" not found" );
        result = it->second->jsonRoute( dst, dstOffset + 1, cmd, arg );
      }
      
      return result;
    }

    RC::ConstHandle<JSON::Value> Context::jsonExecDG( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Value> result;
      
      if ( cmd == "createOperator" )
        Operator::jsonExecCreate( arg, this );
      else if ( cmd == "createNode" )
        Node::jsonExecCreate( arg, this );
      else if ( cmd == "createResourceLoadNode" )
        ResourceLoadNode::jsonExecCreate( arg, this );
      else if ( cmd == "createEvent" )
        Event::jsonExecCreate( arg, this );
      else if ( cmd == "createEventHandler" )
        EventHandler::jsonExecCreate( arg, this );
      else throw Exception( "unknown command" );
      
      return result;
    }

    RC::Handle<JSON::Object> Context::jsonDescDG() const
    {
      RC::Handle<JSON::Object> dgJSONDesc = JSON::Object::Create();
      for ( NamedObjectMap::const_iterator it=m_namedObjectRegistry.begin(); it!=m_namedObjectRegistry.end(); ++it )
      {
        std::string const &namedObjectName = it->first;
        RC::Handle<NamedObject> const &namedObject = it->second;
        dgJSONDesc->set( namedObjectName, namedObject->jsonDesc() );
      }
      return dgJSONDesc;
    }
  };
};
