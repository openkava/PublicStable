/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MT/IdleTaskQueue.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Core/Util/Log.h>


namespace Fabric
{
  namespace MT
  {
    RC::Handle<IdleTaskQueue> const &IdleTaskQueue::Instance()
    {
      static RC::Handle<IdleTaskQueue> idleTaskQueue;
      if ( !idleTaskQueue )
        idleTaskQueue = new IdleTaskQueue;
      return idleTaskQueue;
    }

    IdleTaskQueue::IdleTaskQueue()
      : m_stateMutex( "IdleTaskQueue::m_stateMutex" )
      , m_terminated( false )
    {
      m_executingTasks = 0;
      m_exiting = false;
      
      m_thread.start( &IdleTaskQueue::ThreadEntry, this );
    }
    
    IdleTaskQueue::~IdleTaskQueue()
    {
      terminate();
    }
    
    void IdleTaskQueue::terminate()
    {
      if ( m_terminated )
        return;
      m_terminated = true;
      
      wait();

      m_stateMutex.acquire();
      m_exiting = true;
      m_stateCond.broadcast();
      m_stateMutex.release();

      m_thread.waitUntilDone();
    }
    
    void IdleTaskQueue::submit( Callback callback, void *userdata )
    {
      Mutex::Lock stateMutexLock( m_stateMutex );
      size_t oldSize = m_tasks.size();
      m_tasks.resize( oldSize + 1 );
      Task &task = m_tasks[oldSize];
      task.callback = callback;
      task.userdata = userdata;
      m_stateCond.broadcast();
    }
    
    void IdleTaskQueue::wait() const
    {
      Mutex::Lock stateMutexLock( m_stateMutex );
      m_tasks.clear();
      while ( m_executingTasks > 0 )
        m_stateCond.wait( m_stateMutex );
    }

    void IdleTaskQueue::threadEntry()
    {
      m_stateMutex.acquire();
      while ( !m_exiting )
      {
        if ( m_tasks.empty() )
          m_stateCond.wait( m_stateMutex );
        else
        {
          Task task = m_tasks.back();
          m_tasks.pop_back();
          ++m_executingTasks;
          m_stateMutex.release();
          sleep(1);
          try
          {
            task.callback( task.userdata );
          }
          catch ( Exception e )
          {
            FABRIC_LOG( "Exception caught on IdleTaskQueue: %s", (const char*)e.getDesc() );
          }
          catch ( ... )
          {
            FABRIC_LOG( "Exception caught on IdleTaskQueue: unknown exception" );
          }
          m_stateMutex.acquire();
          --m_executingTasks;
          m_stateCond.broadcast();
        }
      }
      m_stateMutex.release();
    }
  };
};
