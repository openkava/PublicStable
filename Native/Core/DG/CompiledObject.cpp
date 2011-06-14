/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/DG/CompiledObject.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Core/Util/Timer.h>

#include <map>

namespace Fabric
{
  namespace DG
  {
    CompiledObject::CompiledObject( RC::Handle<Context> const &context )
      : m_markForRecompileGeneration( s_markForRecompileGlobalGeneration )
      , m_markForRefreshGeneration( s_markForRefreshGlobalGeneration )
      , m_collectTasksGeneration( s_collectTasksGlobalGeneration )
      , m_canExecute( true )
    {
    }
    
    CompiledObject::Errors const &CompiledObject::getErrors() const
    {
      return m_errors;
    }
    
    CompiledObject::Errors &CompiledObject::getErrors()
    {
      return m_errors;
    }
    
    bool CompiledObject::canExecute() const
    {
      return m_canExecute;
    }
    
    void CompiledObject::markForRecompile()
    {
      markForRecompile( s_markForRecompileGlobalGeneration + 1 );
    }
    
    void CompiledObject::markForRecompile( unsigned generation )
    {
      if ( m_markForRecompileGeneration != generation )
      {
        s_compiledObjectsMarkedForRecompile.insert( this );
        m_markForRecompileGeneration = generation;
        propagateMarkForRecompileImpl( generation );
      }
    }
    
    void CompiledObject::Compile( std::set< RC::Handle<CompiledObject> > const &taskCollectors )
    {
      std::map< RC::Handle<CompiledObject>, bool > hadErrorsMap;
      
      for ( std::set< RC::Handle<CompiledObject> >::const_iterator it=taskCollectors.begin(); it!=taskCollectors.end(); ++it )
      {
        RC::Handle<CompiledObject> const &compiledObject = *it;
        hadErrorsMap[compiledObject] = !compiledObject->m_errors.empty();
        compiledObject->m_errors.clear();
      }

      for ( std::set< RC::Handle<CompiledObject> >::const_iterator it=taskCollectors.begin(); it!=taskCollectors.end(); ++it )
      {
        RC::Handle<CompiledObject> const &compiledObject = *it;
        compiledObject->collectErrors();
      }

      for ( std::set< RC::Handle<CompiledObject> >::const_iterator it=taskCollectors.begin(); it!=taskCollectors.end(); ++it )
      {
        RC::Handle<CompiledObject> const &compiledObject = *it;
        if ( hadErrorsMap[compiledObject] || !compiledObject->m_errors.empty() )
          compiledObject->jsonNotifyErrorDelta();
      }

      for ( std::set< RC::Handle<CompiledObject> >::const_iterator it=taskCollectors.begin(); it!=taskCollectors.end(); ++it )
      {
        RC::Handle<CompiledObject> const &taskCollector = *it;
        taskCollector->invalidateRunState();
      }
    }
    
    void CompiledObject::markForRefresh()
    {
      markForRefresh( s_markForRefreshGlobalGeneration + 1 );
    }
    
    void CompiledObject::markForRefresh( unsigned generation )
    {
      if ( m_markForRefreshGeneration != generation )
      {
        s_compiledObjectsMarkedForRefresh.insert( this );
        m_markForRefreshGeneration = generation;
        propagateMarkForRefreshImpl( generation );
      }
    }
    
    void CompiledObject::Refresh( std::set< RC::Handle<CompiledObject> > const &taskCollectors )
    {
      for ( std::set< RC::Handle<CompiledObject> >::const_iterator it=taskCollectors.begin(); it!=taskCollectors.end(); ++it )
      {
        RC::Handle<CompiledObject> const &taskCollector = *it;
        taskCollector->refreshRunState();
      }
    }
    
    void CompiledObject::collectTasks( MT::TaskGroupStream &taskGroupStream ) const
    {
      collectTasks( ++s_collectTasksGlobalGeneration, taskGroupStream );
    }
      
    void CompiledObject::collectTasks( unsigned generation, MT::TaskGroupStream &taskGroupStream ) const
    {
      if ( m_collectTasksGeneration != generation )
      {
        m_collectTasksGeneration = generation;
        collectTasksImpl( generation, taskGroupStream );
      }
    }
    
    std::set< RC::Handle<CompiledObject> > CompiledObject::s_compiledObjectsMarkedForRecompile;
    unsigned CompiledObject::s_markForRecompileGlobalGeneration = 0;
    
    std::set< RC::Handle<CompiledObject> > CompiledObject::s_compiledObjectsMarkedForRefresh;
    unsigned CompiledObject::s_markForRefreshGlobalGeneration = 0;
    
    unsigned CompiledObject::s_collectTasksGlobalGeneration = 0;
      
    RC::ConstHandle<JSON::Value> CompiledObject::jsonDescErrors() const
    {
      Errors const &errors = getErrors();
      
      RC::Handle<JSON::Array> result = JSON::Array::Create();
      for ( size_t i=0; i<errors.size(); ++i )
        result->push_back( JSON::String::Create( errors[i] ) );
      return result;
    }
    
    void CompiledObject::jsonNotifyErrorDelta() const
    {
    }
    
    void CompiledObject::PrepareForExecution()
    {
      if ( !s_compiledObjectsMarkedForRecompile.empty() )
      {
        Compile( s_compiledObjectsMarkedForRecompile );
        s_compiledObjectsMarkedForRecompile.clear();
      }
      ++s_markForRecompileGlobalGeneration;
      
      if ( !s_compiledObjectsMarkedForRefresh.empty() )
      {
        Refresh( s_compiledObjectsMarkedForRefresh );
        s_compiledObjectsMarkedForRefresh.clear();
      }
      ++s_markForRefreshGlobalGeneration;
    }
  };
};
