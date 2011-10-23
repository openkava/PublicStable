/*
 *
 *  Created by Peter Zion on 10-09-16.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_DG_EVENT_HANDLER_H
#define _FABRIC_DG_EVENT_HANDLER_H

#include <Fabric/Core/DG/Container.h>
#include <Fabric/Core/DG/SelectedNode.h>
#include <list>
#include <set>

namespace Fabric
{
  namespace DG
  {
    class Context;
    class Node;
    class Scope;
    class Binding;
    class BindingList;
    class Event;
    
    class EventHandler : public Container
    {
      friend class Node;
      friend class Event;
      
      typedef Util::UnorderedMap< std::string, RC::Handle<Node> > ExternalScopes;
      
    public:
    
      typedef Util::UnorderedMap< std::string, RC::Handle<Node> > Bindings;
      typedef std::vector< RC::Handle<EventHandler> > ChildEventHandlers;
      typedef std::multiset<EventHandler *> ParentEventHandlers;

      static RC::Handle<EventHandler> Create( std::string const &name, RC::Handle<Context> const &context );
      
      virtual bool isEventHandler() const { return true; }

      void appendChildEventHandler( RC::Handle<EventHandler> const &event );
      ChildEventHandlers const &getChildEventHandlers() const;
      void removeChildEventHandler( RC::Handle<EventHandler> const &event );
      
      ParentEventHandlers const &getParentEventHandlers() const;
      
      Bindings const &getScopes() const;
      void setScope( std::string const &name, RC::Handle<Node> const &node );
      void removeScope( std::string const &name );
      std::string const &getScopeName() const;
      void setScopeName( std::string const &bindingName );
      
      RC::Handle<BindingList> getPreDescendBindingList( ) const;
      RC::Handle<BindingList> getPostDescendBindingList( ) const;
      void setPreDescendBindingList( RC::Handle<BindingList> opList );
      void setPostDescendBindingList( RC::Handle<BindingList> opList );
      
      void setSelector( std::string const &targetScopeName, RC::Handle<Binding> const &selector );
      
      virtual void jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      virtual void jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      static void jsonExecCreate( RC::ConstHandle<JSON::Value> const &arg, RC::Handle<Context> const &context, Util::JSONArrayGenerator &resultJAG );
      void jsonExecAppendChildEventHandler( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecRemoveChildEventHandler( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecAddScope( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecRemoveScope( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecSetSelector( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecSetBindingName( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonDesc( Util::JSONGenerator &resultJG ) const;
      virtual void jsonDesc( Util::JSONObjectGenerator &resultJOG ) const;
      virtual void jsonDescType( Util::JSONGenerator &resultJG ) const;
      void jsonDescChildEventHandlers( Util::JSONGenerator &resultJG ) const;
      void jsonDescScopes( Util::JSONGenerator &resultJG ) const;
      void jsonDescBindingName( Util::JSONGenerator &resultJG ) const;
      
    protected:
    
      EventHandler( std::string const &name, RC::Handle<Context> const &context );
      ~EventHandler();
      
      virtual void setOutOfDate();
      
      void addEvent( Event *event );
      void removeEvent( Event *event );
      void addParent( EventHandler *eventHandler );
      void removeParent( EventHandler *eventHandler );
      
      void fire( Scope const *parentScope, RC::ConstHandle<RT::Desc> const &selectorType, SelectedNodeList *selectedNodes );
      void collectErrorsForScope( Scope const *parentScope );
    
      virtual void propagateMarkForRecompileImpl( unsigned generation );    
      virtual void propagateMarkForRefreshImpl( unsigned generation );    
      virtual void collectErrors();
      virtual void invalidateRunState();
      virtual void refreshRunState();
      virtual void collectTasksImpl( unsigned generation, MT::TaskGroupStream &taskGroupStream ) const;
      virtual bool canExecute() const;
      
      void ensureRunState() const;
      
    private:
    
      Context *m_context;
      
      std::set<Event *> m_events;

      ParentEventHandlers m_parentEventHandlers;
      ChildEventHandlers m_childEventHandlers;

      ExternalScopes m_bindings;
      std::string m_bindingName;
      
      RC::Handle<BindingList> m_preDescendBindings;
      RC::Handle<BindingList> m_postDescendBindings;

      std::string m_selectNodeBindingName;
      RC::Handle<Binding> m_selectBinding;
      
      struct RunState
      {
        bool canExecute;
      };
      
      mutable RunState *m_runState;
    };
  };
};

#endif //_FABRIC_DG_EVENT_HANDLER_H
