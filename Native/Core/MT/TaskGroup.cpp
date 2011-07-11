/*
 *
 *  Created by Peter Zion on 10-09-27.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */

#include "TaskGroup.h"
#include "Task.h"
#include "Debug.h"
#include "Impl.h"

namespace Fabric
{
  namespace MT
  {
    TaskGroup::TaskGroup()
    {
      FABRIC_MT_TRACE( "TaskGroup::TaskGroup()" );
    }

    TaskGroup::TaskGroup( TaskGroup const &that )
      : m_tasks( that.m_tasks )
    {
      FABRIC_MT_TRACE( "TaskGroup::TaskGroup()" );
    }
    
    TaskGroup::~TaskGroup()
    {
      FABRIC_MT_TRACE( "TaskGroup::~TaskGroup()" );
    }
    
    TaskGroup &TaskGroup::operator =( TaskGroup const &that )
    {
      FABRIC_MT_TRACE( "TaskGroup::operator =()" );
      m_tasks = that.m_tasks;
      return *this;
    }
      
    void TaskGroup::clear()
    {
      FABRIC_MT_TRACE( "TaskGroup::clear()" );
      m_tasks.resize(0);
    }
        
    void TaskGroup::add( TaskBase const *task )
    {
      FABRIC_MT_TRACE( "TaskGroup::add()" );
      m_tasks.push_back( task );
    }
    
    void TaskGroup::executeTask( void *userdata, unsigned index ) const
    {
      m_tasks[index]->execute( userdata );
    }
    
    void TaskGroup::ExecuteTask( void *_context, size_t index )
    {
      Context *context = static_cast<Context *>( _context );
      context->taskGroup->executeTask( context->userdata, index );
    }

    void TaskGroup::execute( void *userdata ) const
    {
      FABRIC_MT_TRACE( "TaskGroup::execute( %p )", userdata );
      if ( !m_tasks.empty() )
      {
        Context context;
        context.taskGroup = this;
        context.userdata = userdata;
        executeParallel( m_tasks.size(), &TaskGroup::ExecuteTask, &context, false );
      }
    }

    std::string TaskGroup::desc() const
    {
      FABRIC_MT_TRACE( "TaskGroup::desc()" );
      char buf[4096];
      snprintf( buf, 4096, "%u tasks", (unsigned)m_tasks.size() );
      return buf;
    }
  };
};
