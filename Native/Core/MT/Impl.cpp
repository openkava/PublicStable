/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "Impl.h"
#include "Util.h"

#if defined(FABRIC_POSIX)
# include <stdlib.h>
# include <sys/types.h>
# include <sys/sysctl.h>
#elif defined(FABRIC_WIN32)
# include <windows.h>
# include <process.h>
#endif
#if defined(FABRIC_OS_LINUX)
# include <sys/prctl.h>
#endif

namespace Fabric
{
  namespace MT
  {
    void setThreadName( const char *name )
    {
#if defined(FABRIC_OS_LINUX)
      char    pthread_name[ 16 ];
      ::memset( pthread_name, 0, sizeof( pthread_name ) );
      ::strncpy( pthread_name, name, sizeof( pthread_name ) - 1 );
      ::prctl( PR_SET_NAME, (long)(intptr_t)pthread_name );
#elif defined(FABRIC_OS_WINDOWS)
      const DWORD MS_VC_EXCEPTION=0x406D1388;

      #pragma pack(push,8)
      typedef struct tagTHREADNAME_INFO
      {
         DWORD dwType; // Must be 0x1000.
         LPCSTR szName; // Pointer to name (in user addr space).
         DWORD dwThreadID; // Thread ID (-1=caller thread).
         DWORD dwFlags; // Reserved for future use, must be zero.
      } THREADNAME_INFO;
      #pragma pack(pop)

      THREADNAME_INFO info;
      info.dwType = 0x1000;
      info.szName = "Fabric Worker Thread";
      info.dwThreadID = -1;
      info.dwFlags = 0;

      __try
      {
        RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
      }
      __except(EXCEPTION_EXECUTE_HANDLER)
      {
      }
#endif
    }
    
    ThreadPool ThreadPool::m_singleton;

#if defined( FABRIC_POSIX )
    ThreadPool::ThreadPool()
      : m_stateMutex( "MT::ThreadPool stateMutex" )
      , m_exiting( false )
    {
      m_isMainThread = true;
      
      m_stateMutex.lock();
      size_t numCores = getNumCores();
      m_workerThreads.resize( numCores - 1 );
      for ( size_t i=0; i<m_workerThreads.size(); ++i )
        m_workerThreads[i].start( &ThreadPool::WorkerMainCallback, this );
      m_stateMutex.unlock();
    }
    
    ThreadPool::~ThreadPool()
    {
      m_stateMutex.lock();
      m_exiting = true;
      m_stateCond.broadcast();
      m_stateMutex.unlock();
      
      for ( size_t i=0; i<m_workerThreads.size(); ++i )
        m_workerThreads[i].waitUntilDone();
    }

    void ThreadPool::executeParallel( size_t count, void (*callback)( void *userdata, size_t index ), void *userdata, bool mainThreadOnly )
    {
      if ( count == 1 && (!mainThreadOnly || m_isMainThread.get()) )
        callback( userdata, 0 );
      else
      {
        Task task( count, callback, userdata );
        
        m_stateMutex.lock();
        
        if ( mainThreadOnly )
          m_mainThreadTasks.push_back( &task );
        else m_tasks.push_back( &task );
        
        // [pzion 20101108] Must wake waiter because there is work to do
        m_stateCond.broadcast();
        
        while ( !task.completed_CRITICAL_SECTION() )
          executeOneTaskIfPossible_CRITICAL_SECTION();
        
        m_stateMutex.unlock();
      }
    }

    void ThreadPool::executeOneTaskIfPossible_CRITICAL_SECTION()
    {
      if ( m_tasks.empty() && (!m_isMainThread.get() || m_mainThreadTasks.empty()) )
        m_stateCond.wait( m_stateMutex );
      else
      {
        std::vector<Task *> *taskQueue;
        if ( m_isMainThread.get() && !m_mainThreadTasks.empty() )
          taskQueue = &m_mainThreadTasks;
        else taskQueue = &m_tasks;
        
        Task *task = taskQueue->back();
        
        size_t index;
        bool keep;
        task->preExecute_CRITICAL_SECTION( index, keep );
        if ( !keep )
          taskQueue->pop_back();

        m_stateMutex.unlock();
        task->execute( index );
        m_stateMutex.lock();
        
        task->postExecute_CRITICAL_SECTION();
        if ( task->completed_CRITICAL_SECTION() )
        {
          // [pzion 20101108] Must wake waiter because they might be
          // waiting on the task completion
          m_stateCond.broadcast();
        }
      }
    }

    void ThreadPool::workerMain()
    {
      m_stateMutex.lock();
      while ( !m_exiting )
        executeOneTaskIfPossible_CRITICAL_SECTION();
      m_stateMutex.unlock();
    }
    
    void ThreadPool::WorkerMainCallback( void *_this )
    {
      static_cast<ThreadPool *>(_this)->workerMain();
    }
#elif defined( FABRIC_WIN32 )
    ThreadPool::ThreadPool()
      : m_exiting( false )
      , m_running( false )
    {
      FABRIC_MT_TRACE( "ThreadPool::ThreadPool()" );
      
      m_isMainThread = true;
      
      ::InitializeCriticalSection( &m_cs );

      // Create a manually reset event. This is important, because
      // signalling an auto-reset event only wakes up a single waiter.
      m_hWakeup = ::CreateEvent( NULL, TRUE, FALSE, NULL );

      m_workerThreads.resize( getNumCores() - 1 );
      m_workerExit.resize( getNumCores() - 1 );
      for( size_t i = 0; i < m_workerThreads.size(); i++ )
      {
        m_workerExit[ i ] = ::CreateEvent( NULL, FALSE, FALSE, NULL );

        ThreadData    *tdata = new ThreadData;
        tdata->m_this = this;
        tdata->m_exitEvent = m_workerExit[ i ];
        m_workerThreads[ i ] = (HANDLE)_beginthreadex( NULL, 0, WorkerMainCallback, tdata, 0, NULL );

        if( m_workerThreads[ i ] == NULL )
          throw Exception( "CreateThread: unknown failure" );

      }

      m_running = true;
    }
    
    ThreadPool::~ThreadPool()
    {
      FABRIC_MT_TRACE( "ThreadPool::~ThreadPool()" );
      
      if( m_running )
        terminate();

      FABRIC_ASSERT_TEXT( !m_running, "ThreadPool still running. Who forgot to turn the lights off?" );
    }

    void ThreadPool::terminate()
    {
      FABRIC_MT_TRACE( "ThreadPool::terminate" );

      if( !m_running )
        return;

      m_running = false;

      if( !m_workerThreads.empty() )
      {
        ::EnterCriticalSection( &m_cs );
        m_exiting = true;
        ::SetEvent( m_hWakeup );
        ::LeaveCriticalSection( &m_cs );
        
        if( ::WaitForMultipleObjects( 
          DWORD( m_workerThreads.size() ), &m_workerExit[ 0 ], TRUE, INFINITE ) == WAIT_FAILED )
          throw Exception( "Thread wait failed." );
      }
      
      ::EnterCriticalSection( &m_cs );
      for( size_t i = 0; i < m_workerExit.size(); i++ )
        ::CloseHandle( m_workerExit[ i ] );
      ::CloseHandle( m_hWakeup );
      m_hWakeup = NULL;
      ::LeaveCriticalSection( &m_cs );

      ::DeleteCriticalSection( &m_cs );
      ::ZeroMemory( &m_cs, sizeof( m_cs ) );
    }

    void ThreadPool::executeParallel( size_t count, void (*callback)( void *userdata, size_t index ), void *userdata, bool mainThreadOnly )
    {
      FABRIC_MT_TRACE( "ThreadPool::executeParallel( %u, %p, %p )", (unsigned)count, callback, userdata );

      if( !m_running )
        throw Exception( "Thread pool no longer running." );
      
      if ( count == 1 && (!mainThreadOnly || m_isMainThread.get()) )
        callback( userdata, 0 );
      else if ( count > 1 )
      {
        Task task( count, callback, userdata );
        
        ::EnterCriticalSection( &m_cs );
          
        if ( mainThreadOnly )
          m_mainThreadTasks.push_back( &task );
        else m_tasks.push_back( &task );
        
        if( !::SetEvent( m_hWakeup ) )
          throw Exception( "SetEvent(): unknown failure" );
        
        while ( !task.completed_CRITICAL_SECTION() )
          executeOneTaskIfPossible_CRITICAL_SECTION();
        
        ::ResetEvent( m_hWakeup );

        ::LeaveCriticalSection( &m_cs );
      }
    }

    void ThreadPool::executeOneTaskIfPossible_CRITICAL_SECTION()
    {
      FABRIC_MT_TRACE( "ThreadPool::executeOneTaskIfPossible_CRITICAL_SECTION()" );
      
      if ( m_tasks.empty() && (!m_isMainThread.get() || m_mainThreadTasks.empty()) )
      {
        ::LeaveCriticalSection( &m_cs );

        FABRIC_MT_TRACE_NOTE( "waiting for task.." );
        if( ::WaitForSingleObject( m_hWakeup, INFINITE ) == WAIT_FAILED )
          throw Exception( "WaitForSingleObject(): unknown failure" );

        ::EnterCriticalSection( &m_cs );
      }
      else
      {
        std::vector<Task *> *taskQueue;
        if ( m_isMainThread.get() && !m_mainThreadTasks.empty() )
          taskQueue = &m_mainThreadTasks;
        else taskQueue = &m_tasks;
        
        Task *task = taskQueue->back();
        
        FABRIC_MT_TRACE_NOTE( "task = %p", task );
        size_t index;
        bool keep;
        FABRIC_MT_TRACE_NOTE( "before preExecute" );
        task->preExecute_CRITICAL_SECTION( index, keep );
        FABRIC_MT_TRACE_NOTE( "after preEecute" );
        if ( !keep )
          taskQueue->pop_back();

        ::LeaveCriticalSection( &m_cs );
        
        task->execute( index );
        
        ::EnterCriticalSection( &m_cs );
        
        task->postExecute_CRITICAL_SECTION();
        if ( task->completed_CRITICAL_SECTION() )
        {
          // Wake up the other waiting threads
          if( !::SetEvent( m_hWakeup ) )
            throw Exception( "SetEvent(): unknown failure" );
        }
      }
    }

    void ThreadPool::workerMain()
    {
      FABRIC_MT_TRACE( "ThreadPool::workerMain()" );
      
      ::EnterCriticalSection( &m_cs );
        
      while ( !m_exiting )
        executeOneTaskIfPossible_CRITICAL_SECTION();
        
      ::LeaveCriticalSection( &m_cs );
    }
    
    unsigned int ThreadPool::WorkerMainCallback( void *threadDataPtr )
    {
      FABRIC_MT_TRACE( "ThreadPool::WorkerMainCallback( %p )", threadDataPtr );
      ThreadData    *threadData = (ThreadData *)threadDataPtr;

      setThreadName( "Fabric Worker Thread" );
      threadData->m_this->workerMain();
      ::SetEvent( threadData->m_exitEvent );
      delete threadData;
      return 0;
    }
#endif
  };
};
