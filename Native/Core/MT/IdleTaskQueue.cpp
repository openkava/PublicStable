/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MT/IdleTaskQueue.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Core/Util/Log.h>

#if defined(FABRIC_MT_IDLE_TASK_QUEUE_WIN32)
# include <process.h>
#endif


namespace Fabric
{
  
  
  namespace MT
  {
    IdleTaskQueue *IdleTaskQueue::Instance()
    {
      static IdleTaskQueue *idleTaskQueue = 0;
      if ( !idleTaskQueue )
        idleTaskQueue = new IdleTaskQueue;
      return idleTaskQueue;
    }
    
    IdleTaskQueue::IdleTaskQueue()
    {
#if defined(FABRIC_MT_IDLE_TASK_QUEUE_GCD)
      m_dispatchQueue = dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_LOW, NULL );
      m_dispatchGroup = dispatch_group_create();
#elif defined(FABRIC_MT_IDLE_TASK_QUEUE_PTHREAD)
      m_executingTasks = 0;
      m_exiting = false;
      
      pthread_mutex_init( &m_stateMutex, 0 );
      pthread_cond_init( &m_stateCond, 0 );

      pthread_attr_init( &m_threadAttr );
      pthread_attr_setstacksize( &m_threadAttr, 64*1024*1024 );
      
      pthread_create( &m_thread, &m_threadAttr, &IdleTaskQueue::ThreadEntry, this );
#elif defined(FABRIC_MT_IDLE_TASK_QUEUE_WIN32)
      ::InitializeCriticalSection( &m_csTasks );
      ::InitializeCriticalSection( &m_csExecute );
      m_hWakeup = ::CreateEvent( NULL, FALSE, FALSE, NULL );

      m_exiting = false;

      HANDLE hThread;
      hThread = (HANDLE)_beginthreadex( 0, 64 * 1024 * 1024, ThreadEntry, this, 0, NULL );
      if( hThread == NULL )
        throw Exception( "_beginthreadex: Unknown failure." );

      // Set low priority on the thread
      ::SetThreadPriority( hThread, THREAD_PRIORITY_IDLE );
      ::DuplicateHandle( 
        GetCurrentProcess(), hThread, 
        GetCurrentProcess(), &m_hIdleThread,
        0, FALSE, DUPLICATE_SAME_ACCESS );
#else
# error "Unsupported platform"
#endif
    }
    
    IdleTaskQueue::~IdleTaskQueue()
    {
      wait();

#if defined(FABRIC_MT_IDLE_TASK_QUEUE_GCD)
      dispatch_release( m_dispatchGroup );
#elif defined(FABRIC_MT_IDLE_TASK_QUEUE_PTHREAD)
      pthread_mutex_lock( &m_stateMutex );
      m_exiting = true;
      pthread_cond_broadcast( &m_stateCond );
      pthread_mutex_unlock( &m_stateMutex );

      void *result;
      pthread_join( m_thread, &result );
      
      pthread_attr_destroy( &m_threadAttr );
#elif defined(FABRIC_MT_IDLE_TASK_QUEUE_WIN32)
      m_exiting = true;
      ::SetEvent( m_hWakeup );
      ::WaitForSingleObject( m_hIdleThread, INFINITE );

      ::DeleteCriticalSection( &m_csTasks );
      ::DeleteCriticalSection( &m_csExecute );
#else
# error "Unsupported platform"
#endif
    }
    
    void IdleTaskQueue::submit( Callback callback, void *userdata )
    {
#if defined(FABRIC_MT_IDLE_TASK_QUEUE_GCD)
      dispatch_group_async_f( m_dispatchGroup, m_dispatchQueue, userdata, callback );
#elif defined(FABRIC_MT_IDLE_TASK_QUEUE_PTHREAD)
      pthread_mutex_lock( &m_stateMutex );
      size_t oldSize = m_tasks.size();
      m_tasks.resize( oldSize + 1 );
      Task &task = m_tasks[oldSize];
      task.callback = callback;
      task.userdata = userdata;
      pthread_cond_broadcast( &m_stateCond );
      pthread_mutex_unlock( &m_stateMutex );
#elif defined(FABRIC_MT_IDLE_TASK_QUEUE_WIN32)
      ::EnterCriticalSection( &m_csTasks );
      Task    task;
      task.callback = callback;
      task.userdata = userdata;
      m_tasks.push( task );
      ::LeaveCriticalSection( &m_csTasks );
      ::SetEvent( m_hWakeup );
#else
# error "Unsupported platform"
#endif
    }
    
    void IdleTaskQueue::wait() const
    {
#if defined(FABRIC_MT_IDLE_TASK_QUEUE_GCD)
      dispatch_group_wait( m_dispatchGroup, DISPATCH_TIME_FOREVER );
#elif defined(FABRIC_MT_IDLE_TASK_QUEUE_PTHREAD)
      pthread_mutex_lock( &m_stateMutex );
      m_tasks.clear();
      while ( m_executingTasks > 0 )
        pthread_cond_wait( &m_stateCond, &m_stateMutex );
      pthread_mutex_unlock( &m_stateMutex );
#elif defined(FABRIC_MT_IDLE_TASK_QUEUE_WIN32)
      ::EnterCriticalSection( &m_csTasks );
      while( !m_tasks.empty() )
        m_tasks.pop();
      ::LeaveCriticalSection( &m_csTasks );
      ::EnterCriticalSection( &m_csExecute );
      ::LeaveCriticalSection( &m_csExecute );
#else
# error "Unsupported platform"
#endif
    }

#if defined(FABRIC_MT_IDLE_TASK_QUEUE_PTHREAD)
    void *IdleTaskQueue::threadEntry()
    {
      pthread_mutex_lock( &m_stateMutex );
      while ( !m_exiting )
      {
        if ( m_tasks.empty() )
          pthread_cond_wait( &m_stateCond, &m_stateMutex );
        else
        {
          Task task = m_tasks.back();
          m_tasks.pop_back();
          ++m_executingTasks;
          pthread_mutex_unlock( &m_stateMutex );
          sleep(1);
          try
          {
            task.callback( task.userdata );
          }
          catch ( Exception e )
          {
            FABRIC_LOG( "Exception caught on IdleTaskQueue: %s", e.getDesc().c_str() );
          }
          catch ( ... )
          {
            FABRIC_LOG( "Exception caught on IdleTaskQueue: unknown exception" );
          }
          pthread_mutex_lock( &m_stateMutex );
          --m_executingTasks;
          pthread_cond_broadcast( &m_stateCond );
        }
      }
      pthread_mutex_unlock( &m_stateMutex );
    }
#elif defined(FABRIC_MT_IDLE_TASK_QUEUE_WIN32)
    unsigned IdleTaskQueue::threadEntry()
    {
      while( !m_exiting )
      {
        ::EnterCriticalSection( &m_csTasks );
        if( m_tasks.empty() )
        {
          ::LeaveCriticalSection( &m_csTasks );
          ::WaitForSingleObject( m_hWakeup, INFINITE );
        }
        else
        {
          ::EnterCriticalSection( &m_csExecute );
          Task task = m_tasks.front();
          m_tasks.pop();
          ::LeaveCriticalSection( &m_csTasks );
          try
          {
            task.callback( task.userdata );
          }
          catch ( Exception e )
          {
            FABRIC_LOG( "Exception caught on IdleTaskQueue: %s", e.getDesc().c_str() );
          }
          catch ( ... )
          {
            FABRIC_LOG( "Exception caught on IdleTaskQueue: unknown exception" );
          }
          ::LeaveCriticalSection( &m_csExecute );
        }
      }
      return 0;
    }
#endif
  };
};
