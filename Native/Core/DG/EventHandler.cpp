/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "EventHandler.h"
#include "Event.h"
#include "Node.h"
#include "Binding.h"
#include "BindingList.h"
#include "Scope.h"
#include "Operator.h"
#include "SelectedNode.h"
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Base/JSON/Null.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  

  namespace DG
  {
    RC::Handle<EventHandler> EventHandler::Create( std::string const &name, RC::Handle<Context> const &context )
    {
      RC::Handle<EventHandler> eventHandler = new EventHandler( name, context );
      eventHandler->notifyDelta( eventHandler->jsonDesc() );
      return eventHandler;
    }
    
    EventHandler::EventHandler( std::string const &name, RC::Handle<Context> const &context )
      : Container( name, context )
      , m_context( context.ptr() )
      , m_runState( 0 )
    {
      m_preDescendBindings = BindingList::Create( context );
      m_preDescendBindings->addOwner( this, "preDescendBindings" );
      
      m_postDescendBindings = BindingList::Create( context );
      m_postDescendBindings->addOwner( this, "postDescendBindings" );
    }
    
    EventHandler::~EventHandler()
    {
      delete m_runState;
      
      FABRIC_ASSERT( m_events.empty() );
      
      while ( !m_bindings.empty() )
      {
        Util::UnorderedMap< std::string, RC::Handle<Node> >::iterator it = m_bindings.begin();
        RC::Handle<Node> node = it->second;
        FABRIC_ASSERT( node );
        m_bindings.erase( it );
        
        Node::EventHandlers::iterator jt = node->m_eventHandlers.find( this );
        FABRIC_ASSERT( jt != node->m_eventHandlers.end() );
        node->m_eventHandlers.erase( jt );
      }
      
      FABRIC_ASSERT( m_parentEventHandlers.empty() );
      
      for ( size_t i=0; i<m_childEventHandlers.size(); ++i )
      {
        RC::Handle<EventHandler> const &childEventHandler = m_childEventHandlers[i];
        childEventHandler->removeParent( this );
      }
      
      m_postDescendBindings->removeOwner( this );
      m_preDescendBindings->removeOwner( this );
    }
      
    void EventHandler::addEvent( Event *event )
    {
      bool insertResult = m_events.insert( event ).second;
      FABRIC_ASSERT( insertResult );
    }
    
    void EventHandler::removeEvent( Event *event )
    {
      std::set< Event * >::iterator it = m_events.find( event );
      FABRIC_ASSERT( it != m_events.end() );
      m_events.erase( it );
    }
      
    void EventHandler::addParent( EventHandler *eventHandler )
    {
      m_parentEventHandlers.insert( eventHandler );
    }
    
    void EventHandler::removeParent( EventHandler *eventHandler )
    {
      ParentEventHandlers::iterator it = m_parentEventHandlers.find( eventHandler );
      FABRIC_ASSERT( it != m_parentEventHandlers.end() );
      m_parentEventHandlers.erase( it );
    }
      
    void EventHandler::appendChildEventHandler( RC::Handle<EventHandler> const &eventHandler )
    {
      if ( !eventHandler )
        throw Exception( "event handler is null" );
      m_childEventHandlers.push_back( eventHandler );
      eventHandler->addParent( this );
          
      markForRecompile();
      
      notifyDelta( "childEventHandlers", jsonDescChildEventHandlers() );
    }
    
    EventHandler::ChildEventHandlers const &EventHandler::getChildEventHandlers() const
    {
      return m_childEventHandlers;
    }
      
    void EventHandler::removeChildEventHandler( RC::Handle<EventHandler> const &eventHandler )
    {
      size_t numRemoved = 0;
      for ( size_t i=0; i<m_childEventHandlers.size(); )
      {
        if ( m_childEventHandlers[i] == eventHandler )
        {
          for ( size_t j=i+1; j<m_childEventHandlers.size(); ++j )
            m_childEventHandlers[j-1] = m_childEventHandlers[j];
          m_childEventHandlers.resize( m_childEventHandlers.size() - 1 );
          ++numRemoved;
        }
        else ++i;
      }
      if ( numRemoved == 0 )
        throw Exception( "child event handler not found" );
          
      markForRecompile();
      
      notifyDelta( "childEventHandlers", jsonDescChildEventHandlers() );
    }

    EventHandler::ParentEventHandlers const &EventHandler::getParentEventHandlers() const
    {
      return m_parentEventHandlers;
    }

    void EventHandler::addScope( std::string const &name, RC::Handle<Node> const &node )
    {
      if ( name.length() == 0 )
        throw Exception( "name must be non-empty" );
      
      RC::Handle<Node> &bindingSlot = m_bindings[name];
      if ( bindingSlot != node )
      {
        if ( bindingSlot )
          bindingSlot->removeEventHandler( this );
        bindingSlot = node;
        if ( bindingSlot )
          bindingSlot->addEventHandler( this );
          
        markForRecompile();
        
        notifyDelta( "scopes", jsonDescScopes() );
      }
    }

    EventHandler::Bindings const &EventHandler::getScopes() const
    {
      return m_bindings;
    }
      
    std::string const &EventHandler::getScopeName() const
    {
      return m_bindingName;
    }
    
    void EventHandler::setScopeName( std::string const &bindingName )
    {
      if ( bindingName != m_bindingName )
      {
        m_bindingName = bindingName;
        markForRecompile();
        notifyDelta( "bindingName", JSON::String::Create( m_bindingName ) );
      }
    }
    
    RC::Handle<BindingList> EventHandler::getPreDescendBindingList( ) const
    {
      return m_preDescendBindings;
    }
    
    RC::Handle<BindingList> EventHandler::getPostDescendBindingList( ) const
    {
      return m_postDescendBindings;
    }
    
    void EventHandler::setPreDescendBindingList( RC::Handle<BindingList> opList )
    {
      if( opList.isNull() )
        throw Exception( "no operator list given" );
      
      m_preDescendBindings->removeOwner( this );
      opList->addOwner( this, "preDescendBindings" );
      m_preDescendBindings = opList;
    }
    
    void EventHandler::setPostDescendBindingList( RC::Handle<BindingList> opList )
    {
      if( opList.isNull() )
        throw Exception( "no operator list given" );
      
      m_postDescendBindings->removeOwner( this );
      opList->addOwner( this, "postDescendBindings" );
      m_postDescendBindings = opList;
    }
        
    void EventHandler::setSelector( std::string const &nodeBindingName, RC::Handle<Binding> const &binding )
    {
      if ( nodeBindingName.empty() )
        throw Exception( "nodeBindingName: must be non-empty" );
      m_selectNodeBindingName = nodeBindingName;
      m_selectBinding = binding;
      markForRecompile();
    }
    
    void EventHandler::fire( Scope const *parentScope, RC::ConstHandle<RT::Desc> const &selectorType, SelectedNodeList *selectedNodes )
    {
      BindingsScope bindingsScope( m_bindings, parentScope );
      SelfScope selfScope( this, &bindingsScope );
      NamedScope bindingNameScope( m_bindingName, this, &bindingsScope );
      Scope *childScope = m_bindingName.length()>0? (Scope *)&bindingNameScope: (Scope *)&bindingsScope;
      
      size_t numPreDescendBindings = m_preDescendBindings->size();
      for ( size_t i=0; i<numPreDescendBindings; ++i )
      {
        RC::Handle<Binding> binding = m_preDescendBindings->get(i);
        Util::AutoPtr<MT::ParallelCall> parallelCall = binding->bind( selfScope, 0 );
        parallelCall->executeSerial();
      }
      
      for ( size_t i=0; i<m_childEventHandlers.size(); ++i )
      {
        RC::Handle<EventHandler> const &childEventHandler = m_childEventHandlers[i];
        childEventHandler->fire( childScope, selectorType, selectedNodes );
      }
    
      size_t numPostDescendBindings = m_postDescendBindings->size();
      for ( size_t i=0; i<numPostDescendBindings; ++i )
      {
        RC::Handle<Binding> binding = m_postDescendBindings->get(i);
        Util::AutoPtr<MT::ParallelCall> parallelCall = binding->bind( selfScope, 0 );
        parallelCall->executeSerial();
      }
      
      if ( m_selectBinding && selectorType && selectedNodes )
      {
        Bindings::const_iterator it = m_bindings.find( m_selectNodeBindingName );
        FABRIC_ASSERT( it != m_bindings.end() );
        RC::Handle<Node> const &node = it->second;
        bool shouldSelect = false;
        SelectedNode selectedNode( node, selectorType );
        void *prefixes[2] = { &shouldSelect, &selectedNode.data[0] };
        Util::AutoPtr<MT::ParallelCall> parallelCall = m_selectBinding->bind( bindingsScope, 0, 2, prefixes );
        parallelCall->executeSerial();
        if ( shouldSelect )
          selectedNodes->push_back( selectedNode );
      }
    }
      
    void EventHandler::collectErrorsForScope( Scope const *parentScope )
    {
      BindingsScope bindingsScope( m_bindings, parentScope );
      SelfScope selfScope( this, &bindingsScope );
      NamedScope bindingNameScope( m_bindingName, this, &bindingsScope );
      Scope *childScope = m_bindingName.length()>0? (Scope *)&bindingNameScope: (Scope *)&bindingsScope;

      size_t numPreDescendBindings = m_preDescendBindings->size();
      for ( size_t i=0; i<numPreDescendBindings; ++i )
      {
        RC::Handle<Binding> binding = m_preDescendBindings->get(i);
        try
        {
          binding->bind( selfScope, 0 );
        }
        catch ( Exception e )
        {
          getErrors().push_back( "pre-descend operator " + _(i) + ": " + e );
        }
      }
      
      for ( size_t i=0; i<m_childEventHandlers.size(); ++i )
      {
        RC::Handle<EventHandler> const &childEventHandler = m_childEventHandlers[i];
        childEventHandler->collectErrorsForScope( childScope );
      }
    
      size_t numPostDescendBindings = m_postDescendBindings->size();
      for ( size_t i=0; i<numPostDescendBindings; ++i )
      {
        RC::Handle<Binding> binding = m_postDescendBindings->get(i);
        try
        {
          binding->bind( selfScope, 0 );
        }
        catch ( Exception e )
        {
          getErrors().push_back( "post-descend operator " + _(i) + ": " + e );
        }
      }
        
      if ( m_selectBinding )
      {
        void *prefixes[2] = { 0, 0 };
        try
        {
          m_selectBinding->bind( bindingsScope, 0, 2, prefixes );
        }
        catch ( Exception e )
        {
          getErrors().push_back( "selector: " + e );
        }
      }
    }
    
    void EventHandler::collectTasksImpl( unsigned generation, MT::TaskGroupStream &taskGroupStream ) const
    {
      for ( Util::UnorderedMap< std::string, RC::Handle<Node> >::const_iterator it=m_bindings.begin(); it!=m_bindings.end(); ++it )
      {
        RC::Handle<Node> const &boundNode = it->second;
        boundNode->collectTasks( generation, taskGroupStream );
      }
      
      for ( size_t i=0; i<m_childEventHandlers.size(); ++i )
      {
        RC::Handle<EventHandler> const &childEventHandler = m_childEventHandlers[i];
        childEventHandler->collectTasks( generation, taskGroupStream );
      }
    }
    
    void EventHandler::propagateMarkForRecompileImpl( unsigned generation )
    {
      for ( size_t i=0; i<m_childEventHandlers.size(); ++i )
      {
        RC::Handle<EventHandler> const &childEventHandler = m_childEventHandlers[i];
        childEventHandler->markForRecompile( generation );
      }
      
      for ( ParentEventHandlers::const_iterator it=m_parentEventHandlers.begin(); it!=m_parentEventHandlers.end(); ++it )
      {
        EventHandler *parentEventHandler = *it;
        parentEventHandler->markForRecompile( generation );
      }
      
      for ( std::set< Event * >::const_iterator it=m_events.begin(); it!=m_events.end(); ++it )
      {
        Event *event = *it;
        event->markForRecompile( generation );
      }
    }
     
    void EventHandler::propagateMarkForRefreshImpl( unsigned generation )
    {
    }

    void EventHandler::collectErrors()
    {
      if ( m_selectBinding )
      {
        Bindings::const_iterator it = m_bindings.find( m_selectNodeBindingName );
        if ( it == m_bindings.end() )
          getErrors().push_back( "selector node '" + m_selectNodeBindingName + "' is not bound" );
      }
    }
    
    void EventHandler::invalidateRunState()
    {
      delete m_runState;
      m_runState = 0;
    }
    
    void EventHandler::refreshRunState()
    {
      FABRIC_ASSERT( false && "EventHandler runstates should never need refreshing" );
    }
    
    void EventHandler::ensureRunState() const
    {
      if ( !m_runState )
      {
        m_runState = new RunState;
        m_runState->canExecute = getErrors().empty();
        
        if ( m_runState->canExecute )
        {
          for ( Util::UnorderedMap< std::string, RC::Handle<Node> >::const_iterator it=m_bindings.begin(); it!=m_bindings.end(); ++it )
          {
            RC::Handle<Node> const &boundNode = it->second;
            if ( !boundNode->canExecute() )
            {
              m_runState->canExecute = false;
              break;
            }
          }
        }
          
        if ( m_runState->canExecute )
        {
          for ( size_t i=0; i<m_childEventHandlers.size(); ++i )
          {
            RC::Handle<EventHandler> const &childEventHandler = m_childEventHandlers[i];
            if ( !childEventHandler->canExecute() )
            {
              m_runState->canExecute = false;
              break;
            }
          }
        }
      }
    }
    
    bool EventHandler::canExecute() const
    {
      ensureRunState();
      return m_runState->canExecute;
    }

    void EventHandler::setOutOfDate()
    {
    }
      
    RC::ConstHandle<JSON::Value> EventHandler::jsonDescChildEventHandlers() const
    {
      RC::Handle<JSON::Array> childEventHandlersJSONArray = JSON::Array::Create();
      for ( ChildEventHandlers::const_iterator it=m_childEventHandlers.begin(); it!=m_childEventHandlers.end(); ++it )
      {
        RC::Handle<EventHandler> const &childEventHandler = *it;
        childEventHandlersJSONArray->push_back( JSON::String::Create( childEventHandler->getName() ) );
      }
      return childEventHandlersJSONArray;
    }

    RC::Handle<JSON::Object> EventHandler::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = Container::jsonDesc();
      
      result->set( "childEventHandlers", jsonDescChildEventHandlers() );
      result->set( "scopes", jsonDescScopes() );
      result->set( "scopeName", JSON::String::Create( m_bindingName ) );
      
      RC::Handle<JSON::Object> scopesJSONObject = JSON::Object::Create();
      for ( ExternalScopes::const_iterator it=m_bindings.begin(); it!=m_bindings.end(); ++it )
      {
        std::string const &name = it->first;
        RC::Handle<Node> const &node = it->second;
        scopesJSONObject->set( name, JSON::String::Create( node->getName() ) );
      }
      result->set( "scopes", scopesJSONObject );
      
      RC::ConstHandle<JSON::Value> selectorJSONValue;
      if ( !m_selectBinding && m_selectNodeBindingName.empty() )
        selectorJSONValue = JSON::Null::Create();
      else
      {
        RC::ConstHandle<JSON::Value> selectorBindingJSONValue;
        if ( m_selectBinding )
          selectorBindingJSONValue = m_selectBinding->jsonDesc();
        else selectorBindingJSONValue = JSON::Null::Create();
        
        RC::ConstHandle<JSON::Value> selectorTargetScopeJSONValue;
        if ( !m_selectNodeBindingName.empty() )
          selectorTargetScopeJSONValue = JSON::String::Create( m_selectNodeBindingName );
        else selectorTargetScopeJSONValue = JSON::Null::Create();
        
        RC::Handle<JSON::Object> selectorJSONObject = JSON::Object::Create();
        selectorJSONObject->set( "binding", selectorBindingJSONValue );
        selectorJSONObject->set( "targetScopeName", selectorTargetScopeJSONValue );
        selectorJSONValue = selectorJSONObject;
      }
      result->set( "selector", selectorJSONValue );

      return result;
    }
      
    RC::ConstHandle<JSON::Value> EventHandler::jsonDescType() const
    {
      static RC::ConstHandle<JSON::Value> result = JSON::String::Create( "EventHandler" );
      return result;
    }
      
    RC::Handle<JSON::Object> EventHandler::jsonDescScopes() const
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      for ( ExternalScopes::const_iterator it=m_bindings.begin(); it!=m_bindings.end(); ++it )
        result->set( it->first, JSON::String::Create( it->second->getName() ) );
      return result;
    }

    RC::ConstHandle<JSON::Value> EventHandler::jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Value> result;

      if ( dst.size() - dstOffset == 1 && dst[dstOffset] == "preDescendBindings" )
        result = m_preDescendBindings->jsonRoute( dst, dstOffset+1, cmd, arg );
      else if ( dst.size() - dstOffset == 1 && dst[dstOffset] == "postDescendBindings" )
        result = m_postDescendBindings->jsonRoute( dst, dstOffset+1, cmd, arg );
      else result = NamedObject::jsonRoute( dst, dstOffset, cmd, arg );
      
      return result;
    }
      
    RC::ConstHandle<JSON::Value> EventHandler::jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Value> result;

      if ( cmd == "appendChildEventHandler" )
        jsonExecAppendChildEventHandler( arg );
      else if ( cmd == "removeChildEventHandler" )
        jsonExecRemoveChildEventHandler( arg );
      else if ( cmd == "addScope" )
        jsonExecAddScope( arg );
      else if ( cmd == "setSelector" )
        jsonExecSetSelector( arg );
      else if ( cmd == "setBindingName" )
        jsonExecSetBindingName( arg );
      else result = Container::jsonExec( cmd, arg );
      
      return result;
    }

    void EventHandler::jsonExecCreate( RC::ConstHandle<JSON::Value> const &arg, RC::Handle<Context> const &context )
    {
      Create( arg->toString()->value(), context );
    }
    
    void EventHandler::jsonExecAppendChildEventHandler( RC::ConstHandle<JSON::Value> const &arg )
    {
      appendChildEventHandler( m_context->getEventHandler( arg->toString()->value() ) );
    }
    
    void EventHandler::jsonExecRemoveChildEventHandler( RC::ConstHandle<JSON::Value> const &arg )
    {
      removeChildEventHandler( m_context->getEventHandler( arg->toString()->value() ) );
    }
    
    void EventHandler::jsonExecAddScope( RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      
      std::string name;
      try
      {
        name = argJSONObject->get( "name" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "'name': " + e;
      }
      
      RC::Handle<Node> node;
      try
      {
        node = m_context->getNode( argJSONObject->get( "node" )->toString()->value() );
      }
      catch ( Exception e )
      {
        throw "'node': " + e;
      }
      
      addScope( name, node );
    }
    
    void EventHandler::jsonExecSetSelector( RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      
      std::string targetName;
      try
      {
        targetName = argJSONObject->get( "targetName" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "'targetName': " + e;
      }
      
      RC::Handle<Operator> operator_;
      try
      {
        operator_ = m_context->getOperator( argJSONObject->get( "operator" )->toString()->value() );
      }
      catch ( Exception e )
      {
        throw "'operator': " + e;
      }
      
      std::vector<std::string> parameterLayout;
      try
      {
        RC::ConstHandle<JSON::Array> parameterLayoutJSONArray = argJSONObject->get("parameterLayout")->toArray();
        size_t parameterLayoutJSONArraySize = parameterLayoutJSONArray->size();
        for ( size_t i=0; i<parameterLayoutJSONArraySize; ++i )
        {
          try
          {
            parameterLayout.push_back( parameterLayoutJSONArray->get(i)->toString()->value() );
          }
          catch ( Exception e )
          {
            throw "index " + _(i) + ": " + e;
          }
        }
      }
      catch ( Exception e )
      {
        throw "'parameterLayout': " + e;
      }
      
      RC::Handle<Binding> binding = Binding::Create( m_context );
      binding->setOperator( operator_ );
      binding->setPrototype( parameterLayout );
      
      setSelector( targetName, binding );
    }
    
    void EventHandler::jsonExecSetBindingName( RC::ConstHandle<JSON::Value> const &arg )
    {
      setScopeName( arg->toString()->value() );
    }
  };
};
