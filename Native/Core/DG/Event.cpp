/*
 *
 *  Created by Peter Zion on 10-09-16.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */

#include "Event.h"
#include "EventHandler.h"
#include "Node.h"
#include "Operator.h"
#include "Scope.h"
#include "SelectedNode.h"
#include "Debug.h"
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/DG/ExecutionEngine.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  

  namespace DG
  {
    RC::Handle<Event> Event::Create( std::string const &name, RC::Handle<Context> const &context )
    {
      RC::Handle<Event> event = new Event( name, context );
      
      Util::SimpleString json;
      {
        Util::JSONGenerator jg( &json );
        event->jsonDesc( jg );
      }
      event->jsonNotifyDelta( json );

      return event;
    }
    
    Event::Event( std::string const &name, RC::Handle<Context> const &context )
      : Container( name, context )
      , m_context( context.ptr() )
      , m_runState( 0 )
    {
    }
    
    Event::~Event()
    {
      for ( size_t i=0; i<m_eventHandlers.size(); ++i )
      {
        RC::Handle<EventHandler> const &eventHandler = m_eventHandlers[i];
        eventHandler->removeEvent( this );
      }

      delete m_runState;
    }
      
    void Event::setOutOfDate()
    {
    }
      
    void Event::appendEventHandler( RC::Handle<EventHandler> const &eventHandler )
    {
      if ( !eventHandler )
        throw Exception( "event handler is null" );
      
      m_eventHandlers.push_back( eventHandler );
      eventHandler->addEvent( this );
      
      markForRecompile();
      
      Util::SimpleString json;
      {
        Util::JSONGenerator jg( &json );
        jsonDescEventHandlers( jg );
      }
      jsonNotifyMemberDelta( "eventHandlers", 13, json );
    }
    
    Event::EventHandlers const &Event::getEventHandlers() const
    {
      return m_eventHandlers;
    }
      
    void Event::fire() const
    {
      PrepareForExecution();
      fire( 0, 0, 0 );
    }
    
    void Event::select( RC::ConstHandle<RT::Desc> selectorType, SelectedNodeList &selectedNodes ) const
    {
      PrepareForExecution();
      fire( 0, selectorType, &selectedNodes );
    }
    
    void Event::fire( Scope const *parentScope, RC::ConstHandle<RT::Desc> const &selectorType, SelectedNodeList *selectedNodes ) const
    {
      ensureRunState();
      if ( !m_runState->canExecute )
        throw Exception( "cannot execute because of errors" );
 
      ExecutionEngine::ContextSetter contextSetter( m_context );
      m_runState->taskGroupStream.execute( m_context->getLogCollector(), 0 );
      for ( EventHandlers::const_iterator it=m_eventHandlers.begin(); it!=m_eventHandlers.end(); ++it )
      {
        RC::Handle<EventHandler> const &eventHandler = *it;
        eventHandler->fire( parentScope, selectorType, selectedNodes );
      }
      if ( m_context->getLogCollector() )
        m_context->getLogCollector()->flush();
      
      jsonNotify( "didFire", 7 );
    }
      
    void Event::collectTasksImpl( unsigned generation, MT::TaskGroupStream &taskGroupStream ) const
    {
      for ( EventHandlers::const_iterator it=m_eventHandlers.begin(); it!=m_eventHandlers.end(); ++it )
      {
        RC::Handle<EventHandler> const &eventHandler = *it;
        eventHandler->collectTasks( generation, taskGroupStream );
      }
    }
    
    void Event::propagateMarkForRecompileImpl( unsigned generation )
    {
      for ( EventHandlers::const_iterator it=m_eventHandlers.begin(); it!=m_eventHandlers.end(); ++it )
      {
        RC::Handle<EventHandler> const &eventHandler = *it;
        eventHandler->markForRecompile( generation );
      }
    }
    
    void Event::propagateMarkForRefreshImpl( unsigned generation )
    {
    }
    
    void Event::collectErrors()
    {
      collectErrors( 0 );
    }
    
    void Event::collectErrors( Scope *scope )
    {
      for ( EventHandlers::const_iterator it=m_eventHandlers.begin(); it!=m_eventHandlers.end(); ++it )
      {
        RC::Handle<EventHandler> const &eventHandler = *it;
        eventHandler->collectErrorsForScope( scope );
      }
    }
    
    void Event::invalidateRunState()
    {
      delete m_runState;
      m_runState = 0;
    }
    
    void Event::refreshRunState()
    {
      FABRIC_ASSERT( false && "Event runstates should never be refreshed" );
    }
    
    void Event::ensureRunState() const
    {
      if ( !m_runState )
      {
        m_runState = new RunState;
        
        m_runState->canExecute = getErrors().empty();
        if ( m_runState->canExecute )
        {
          for ( EventHandlers::const_iterator it=m_eventHandlers.begin(); it!=m_eventHandlers.end(); ++it )
          {
            RC::Handle<EventHandler> const &eventHandler = *it;
            if ( !eventHandler->canExecute() )
            {
              m_runState->canExecute = false;
              break;
            }
          }
        }

        if ( m_runState->canExecute )
          collectTasks( m_runState->taskGroupStream );
      }
    }

    bool Event::canExecute() const
    {
      ensureRunState();
      return m_runState->canExecute;
    }
      
    void Event::jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      if ( cmd == "appendEventHandler" )
        jsonExecAppendEventHandler( arg, resultJAG );
      else if ( cmd == "fire" )
        jsonExecFire( resultJAG );
      else if ( cmd == "select" )
        jsonExecSelect( arg, resultJAG );
      else Container::jsonExec( cmd, arg, resultJAG );
    }

    void Event::jsonExecCreate( RC::ConstHandle<JSON::Value> const &arg, RC::Handle<Context> const &context, Util::JSONArrayGenerator &resultJAG )
    {
      Create( arg->toString()->value(), context );
    }
    
    void Event::jsonExecAppendEventHandler( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      appendEventHandler( m_context->getEventHandler( arg->toString()->value() ) );
    }
    
    void Event::jsonExecFire( Util::JSONArrayGenerator &resultJAG )
    {
      fire();
    }
    
    void Event::jsonExecSelect( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      RC::ConstHandle<RT::Desc> desc = m_context->getRTManager()->getDesc( arg->toString()->value() );
      
      SelectedNodeList selectedNodeList;
      select( desc, selectedNodeList );
      
      Util::JSONGenerator resultJG = resultJAG.makeElement();
      Util::JSONArrayGenerator elementsJAG = resultJG.makeArray();
      for ( SelectedNodeList::const_iterator it=selectedNodeList.begin(); it!=selectedNodeList.end(); ++it )
      {
        SelectedNode const &selectedNode = *it;
        Util::JSONGenerator elementJG = elementsJAG.makeElement();
        Util::JSONObjectGenerator elementJOG = elementJG.makeObject();
        {
          Util::JSONGenerator nodeJG = elementJOG.makeMember( "node", 4 );
          nodeJG.makeString( selectedNode.node->getName() );
        }
        {
          Util::JSONGenerator dataJG = elementJOG.makeMember( "data", 4 );
          desc->generateJSON( &selectedNode.data[0], dataJG );
        }
      }
    }
      
    void Event::jsonDesc( Util::JSONGenerator &resultJG ) const
    {
      Container::jsonDesc( resultJG );
    }
    
    void Event::jsonDesc( Util::JSONObjectGenerator &resultJOG ) const
    {
      Container::jsonDesc( resultJOG );
      Util::JSONGenerator eventHandlersJG = resultJOG.makeMember( "eventHandlers", 13 );
      jsonDescEventHandlers( eventHandlersJG );
    }
      
    void Event::jsonDescType( Util::JSONGenerator &resultJG ) const
    {
      resultJG.makeString( "Event", 5 );
    }
      
    void Event::jsonDescEventHandlers( Util::JSONGenerator &resultJG ) const
    {
      Util::JSONArrayGenerator eventHandlersJAG = resultJG.makeArray();
      for ( EventHandlers::const_iterator it=m_eventHandlers.begin(); it!=m_eventHandlers.end(); ++it )
      {
        RC::Handle<EventHandler> const &eventHandler = *it;
        Util::JSONGenerator eventHandlerJG = eventHandlersJAG.makeElement();
        eventHandlerJG.makeString( eventHandler->getName() );
      }
    }
  };
};
