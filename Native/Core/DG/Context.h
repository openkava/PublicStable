/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_DG_CONTEXT_H
#define _FABRIC_DG_CONTEXT_H

#include <Fabric/Core/JSON/CommandChannel.h>
#include <Fabric/Base/RC/WeakHandleSet.h>
#include <Fabric/Core/MT/Mutex.h>
#include <Fabric/Core/Util/JSONGenerator.h>
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
      
      NamedObjectMap &getNamedObjectRegistry() const;
      
      RC::Handle<NamedObject> getNamedObject( std::string const &name ) const;
      RC::Handle<Container> getContainer( std::string const &name ) const;
      RC::Handle<Operator> getOperator( std::string const &name ) const;
      RC::Handle<Node> getNode( std::string const &name ) const;
      RC::Handle<Event> getEvent( std::string const &name ) const;
      RC::Handle<EventHandler> getEventHandler( std::string const &name ) const;
    
      virtual void jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      virtual void jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      virtual void jsonDesc( Util::JSONGenerator &resultJG ) const;
      
      void jsonRouteDG( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecDG( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonDescDG( Util::JSONGenerator &resultJG ) const;

      void registerClient( Client *client );
      virtual void jsonNotify( std::vector<std::string> const &srcs, char const *cmdData, size_t cmdLength, Util::SimpleString const *arg = 0 );
      void unregisterClient( Client *client );
      
      static std::string const &GetWrapFabricClientJSSource()
      {
        return s_wrapFabricClientJSSource;
      }
      
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
      
      mutable NamedObjectMap m_namedObjectRegistry;
      
      Clients m_clients;
      
      Util::AtomicSize m_notificationBracketCount;
      MT::Mutex m_pendingNotificationsMutex;
      Util::SimpleString *m_pendingNotificationsJSON;
      Util::JSONGenerator *m_pendingNotificationsJSONGenerator;
      Util::JSONArrayGenerator *m_pendingNotificationsJSONArrayGenerator;
      
      static std::string const s_wrapFabricClientJSSource;
    };
  };
};

#endif //_FABRIC_DG_CONTEXT_H
