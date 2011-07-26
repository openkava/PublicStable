/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_DG_CONTEXT_H
#define _FABRIC_DG_CONTEXT_H

#include <Fabric/Core/JSON/CommandChannel.h>
#include <Fabric/Base/RC/WeakHandleSet.h>
#include <Fabric/Core/MT/Mutex.h>
#include <Fabric/Core/Util/UnorderedMap.h>
#include <Fabric/Base/Util/AtomicSize.h>

#include <vector>

namespace Fabric
{
  namespace JSON
  {
    class Value;
    class Object;
    class Array;
  };
  
  namespace MT
  {
    class LogCollector;
    class IdleTaskQueue;
  };
  
  namespace RT
  {
    class Manager;
  };
  
  namespace CG
  {
    class Manager;
  };
  
  namespace IO
  {
    class Manager;
  };
  
  namespace Plug
  {
    class Manager;
  };
  
  namespace DG
  {
    class NamedObject;
    class Container;
    class Node;
    class Event;
    class EventHandler;
    class Operator;
    class BindingList;
    class CodeManager;
    class Client;
    
    class Context : public JSON::CommandChannel
    {
      typedef std::set<Client *> Clients;
      typedef Util::UnorderedMap< std::string, Context * > ContextMap;
      
      void openNotificationBracket();
      void closeNotificationBracket();
      void registerCoreTypes();
    
    public:
    
      class NotificationBracket
      {
      public:
      
        NotificationBracket( RC::Handle<Context> const &context )
          : m_context( context )
        {
          m_context->openNotificationBracket();
        }
        
        ~NotificationBracket()
        {
          m_context->closeNotificationBracket();
        }
        
      private:
      
        RC::Handle<Context> m_context;
      };
    
      typedef Util::UnorderedMap< std::string, RC::Handle<NamedObject> > NamedObjectMap;
    
      static RC::Handle<Context> Create( RC::Handle<IO::Manager> const &ioManager, std::vector<std::string> const &pluginDirs );
      static RC::Handle<Context> Bind( std::string const &contextID );
      
      std::string const &getContextID() const;

      RC::Handle<MT::LogCollector> getLogCollector() const;
      RC::Handle<RT::Manager> getRTManager() const;
      RC::Handle<CG::Manager> getCGManager() const;
      RC::Handle<IO::Manager> getIOManager() const;
      RC::Handle<CodeManager> getCodeManager() const;
      RC::Handle<Plug::Manager> getPlugManager() const;
      
      NamedObjectMap &getNamedObjectRegistry() const;
      
      RC::Handle<NamedObject> getNamedObject( std::string const &name ) const;
      RC::Handle<Container> getContainer( std::string const &name ) const;
      RC::Handle<Operator> getOperator( std::string const &name ) const;
      RC::Handle<Node> getNode( std::string const &name ) const;
      RC::Handle<Event> getEvent( std::string const &name ) const;
      RC::Handle<EventHandler> getEventHandler( std::string const &name ) const;
    
      virtual RC::ConstHandle<JSON::Value> jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      virtual RC::ConstHandle<JSON::Value> jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      virtual RC::Handle<JSON::Object> jsonDesc() const;
      
      RC::ConstHandle<JSON::Value> jsonRouteDG( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      RC::ConstHandle<JSON::Value> jsonExecDG( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      RC::Handle<JSON::Object> jsonDescDG() const;

      void registerClient( Client *client );
      void jsonNotify( std::vector<std::string> const &src, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      void unregisterClient( Client *client );
      
    protected:
    
      Context( RC::Handle<IO::Manager> const &ioManager, std::vector<std::string> const &pluginDirs );
      ~Context();
      
    private:
    
      static ContextMap s_contextMap;
      ContextMap::iterator m_contextMapIterator;
    
      RC::Handle<MT::LogCollector> m_logCollector;
      RC::Handle<RT::Manager> m_rtManager;
      RC::Handle<IO::Manager> m_ioManager;
      RC::Handle<CG::Manager> m_cgManager;
      RC::Handle<CodeManager> m_codeManager;
      RC::Handle<Plug::Manager> m_plugManager;
      
      mutable NamedObjectMap m_namedObjectRegistry;
      
      Clients m_clients;
      
      Util::AtomicSize m_notificationBracketCount;
      MT::Mutex m_pendingNotificationsMutex;
      RC::Handle<JSON::Array> m_pendingNotifications;
    };
  };
};

#endif //_FABRIC_DG_CONTEXT_H
