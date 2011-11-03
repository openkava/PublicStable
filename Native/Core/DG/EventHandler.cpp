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

      Util::SimpleString json;
      {
        Util::JSONGenerator jg( &json );
        eventHandler->jsonDesc( jg );
      }
      eventHandler->jsonNotifyDelta( json );
      
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
      FABRIC_CONFIRM( m_events.insert( event ).second );
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
      
      Util::SimpleString json;
      {
        Util::JSONGenerator jg( &json );
        jsonDescChildEventHandlers( jg );
      }
      jsonNotifyMemberDelta( "childEventHandlers", 18, json );
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
          m_childEventHandlers[i]->removeParent( this );
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
      
      Util::SimpleString json;
      {
        Util::JSONGenerator jg( &json );
        jsonDescChildEventHandlers( jg );
      }
      jsonNotifyMemberDelta( "childEventHandlers", 18, json );
    }

    EventHandler::ParentEventHandlers const &EventHandler::getParentEventHandlers() const
    {
      return m_parentEventHandlers;
    }

    void EventHandler::setScope( std::string const &name, RC::Handle<Node> const &node )
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
        
        Util::SimpleString json;
        {
          Util::JSONGenerator jg( &json );
          jsonDescScopes( jg );
        }
        jsonNotifyMemberDelta( "scopes", 6, json );
      }
    }

    void EventHandler::removeScope( std::string const &name )
    {
      if ( name.length() == 0 )
        throw Exception( "name must be non-empty" );
      
      ExternalScopes::iterator it = m_bindings.find(name);
      if ( it == m_bindings.end() )
        throw Exception( _(name) + ": no such scope" );
      it->second->removeEventHandler( this );
      m_bindings.erase( it );
        
      markForRecompile();
      
      Util::SimpleString json;
      {
        Util::JSONGenerator jg( &json );
        jsonDescScopes( jg );
      }
      jsonNotifyMemberDelta( "scopes", 6, json );
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
        
        Util::SimpleString json;
        {
          Util::JSONGenerator jg( &json );
          jsonDescBindingName( jg );
        }
        jsonNotifyMemberDelta( "bindingName", 11, json );
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
        std::vector<std::string> errors;
        RC::Handle<MT::ParallelCall> parallelCall = binding->bind( errors, selfScope, 0 );
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
        std::vector<std::string> errors;
        RC::Handle<MT::ParallelCall> parallelCall = binding->bind( errors, selfScope, 0 );
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
        std::vector<std::string> errors;
        RC::Handle<MT::ParallelCall> parallelCall = m_selectBinding->bind( errors, bindingsScope, 0, 2, prefixes );
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
        std::string const errorPrefix = "pre-descend operator " + _(i) + ": ";
        std::vector<std::string> errors;
        binding->bind( errors, selfScope, 0 );
        for ( size_t i=0; i<errors.size(); ++i )
          getErrors().push_back( errorPrefix + errors[i] );
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
        std::string const errorPrefix = "post-descend operator " + _(i) + ": ";
        std::vector<std::string> errors;
        binding->bind( errors, selfScope, 0 );
        for ( size_t i=0; i<errors.size(); ++i )
          getErrors().push_back( errorPrefix + errors[i] );
      }
        
      if ( m_selectBinding )
      {
        void *prefixes[2] = { 0, 0 };
        std::string const errorPrefix = "selector: ";
        std::vector<std::string> errors;
        m_selectBinding->bind( errors, bindingsScope, 0, 2, prefixes );
        for ( size_t i=0; i<errors.size(); ++i )
          getErrors().push_back( errorPrefix + errors[i] );
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
      
    void EventHandler::jsonDescChildEventHandlers( Util::JSONGenerator &resultJG ) const
    {
      Util::JSONArrayGenerator childEventHandlersJAG = resultJG.makeArray();
      for ( ChildEventHandlers::const_iterator it=m_childEventHandlers.begin(); it!=m_childEventHandlers.end(); ++it )
      {
        RC::Handle<EventHandler> const &childEventHandler = *it;
        Util::JSONGenerator childEventHandlerJG = childEventHandlersJAG.makeElement();
        childEventHandlerJG.makeString( childEventHandler->getName() );
      }
    }
      
    void EventHandler::jsonDesc( Util::JSONGenerator &resultJG ) const
    {
      Container::jsonDesc( resultJG );
    }

    void EventHandler::jsonDesc( Util::JSONObjectGenerator &resultJOG ) const
    {
      Container::jsonDesc( resultJOG );
      
      {
        Util::JSONGenerator memberJG = resultJOG.makeMember( "childEventHandlers", 18 );
        jsonDescChildEventHandlers( memberJG );
      }
      
      {
        Util::JSONGenerator memberJG = resultJOG.makeMember( "scopes", 6 );
        jsonDescScopes( memberJG );
      }
      
      {
        Util::JSONGenerator memberJG = resultJOG.makeMember( "scopeName", 9 );
        memberJG.makeString( m_bindingName );
      }
      
      {
        Util::JSONGenerator memberJG = resultJOG.makeMember( "preDescendBindings", 18 );
        m_preDescendBindings->jsonDesc( memberJG );
      }
      
      {
        Util::JSONGenerator memberJG = resultJOG.makeMember( "postDescendBindings", 19 );
        m_preDescendBindings->jsonDesc( memberJG );
      }
      
      Util::JSONGenerator selectorGenerator = resultJOG.makeMember( "selector", 8 );
      if ( !m_selectBinding && m_selectNodeBindingName.empty() )
        selectorGenerator.makeNull();
      else
      {
        Util::JSONObjectGenerator selectorGeneratorObject = selectorGenerator.makeObject();
        
        {
          Util::JSONGenerator bindingJG = selectorGeneratorObject.makeMember( "binding", 7 );
          if ( m_selectBinding )
            m_selectBinding->jsonDesc( bindingJG );
          else bindingJG.makeNull();
        }
        
        {
          Util::JSONGenerator targetScopeNameJG = selectorGeneratorObject.makeMember( "targetScopeName", 15 );
          if ( !m_selectNodeBindingName.empty() )
            targetScopeNameJG.makeString( m_selectNodeBindingName );
          else targetScopeNameJG.makeNull();
        }
      }
    }
      
    void EventHandler::jsonDescType( Util::JSONGenerator &resultJG ) const
    {
      resultJG.makeString( "EventHandler", 12 );
    }
      
    void EventHandler::jsonDescScopes( Util::JSONGenerator &resultJG ) const
    {
      Util::JSONObjectGenerator resultJSONObject = resultJG.makeObject();
      for ( ExternalScopes::const_iterator it=m_bindings.begin(); it!=m_bindings.end(); ++it )
      {
        std::string const &name = it->first;
        RC::Handle<Node> const &node = it->second;
        Util::JSONGenerator memberJG = resultJSONObject.makeMember( name );
        memberJG.makeString( node->getName() );
      }
    }
      
    void EventHandler::jsonDescBindingName( Util::JSONGenerator &resultJG ) const
    {
      resultJG.makeString( m_bindingName );
    }

    void EventHandler::jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      if ( dst.size() - dstOffset == 1 && dst[dstOffset] == "preDescendBindings" )
        m_preDescendBindings->jsonRoute( dst, dstOffset+1, cmd, arg, resultJAG );
      else if ( dst.size() - dstOffset == 1 && dst[dstOffset] == "postDescendBindings" )
        m_postDescendBindings->jsonRoute( dst, dstOffset+1, cmd, arg, resultJAG );
      else NamedObject::jsonRoute( dst, dstOffset, cmd, arg, resultJAG );
    }
      
    void EventHandler::jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      if ( cmd == "appendChildEventHandler" )
        jsonExecAppendChildEventHandler( arg, resultJAG );
      else if ( cmd == "removeChildEventHandler" )
        jsonExecRemoveChildEventHandler( arg, resultJAG );
      else if ( cmd == "setScope" )
        jsonExecAddScope( arg, resultJAG );
      else if ( cmd == "removeScope" )
        jsonExecRemoveScope( arg, resultJAG );
      else if ( cmd == "setSelector" )
        jsonExecSetSelector( arg, resultJAG );
      else if ( cmd == "setScopeName" )
        jsonExecSetBindingName( arg, resultJAG );
      else Container::jsonExec( cmd, arg, resultJAG );
    }

    void EventHandler::jsonExecCreate( RC::ConstHandle<JSON::Value> const &arg, RC::Handle<Context> const &context, Util::JSONArrayGenerator &resultJAG )
    {
      Create( arg->toString()->value(), context );
    }
    
    void EventHandler::jsonExecAppendChildEventHandler( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      appendChildEventHandler( m_context->getEventHandler( arg->toString()->value() ) );
    }
    
    void EventHandler::jsonExecRemoveChildEventHandler( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      removeChildEventHandler( m_context->getEventHandler( arg->toString()->value() ) );
    }
    
    void EventHandler::jsonExecAddScope( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
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
      
      setScope( name, node );
    }
    
    void EventHandler::jsonExecRemoveScope( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      std::string scopeName = arg->toString()->value();
      
      removeScope( scopeName );
    }
    
    void EventHandler::jsonExecSetSelector( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
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
    
    void EventHandler::jsonExecSetBindingName( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      setScopeName( arg->toString()->value() );
    }
  };
};
