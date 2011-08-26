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
    
    ThreadPool *ThreadPool::Instance()
    {
      static ThreadPool instance;
      return &instance;
    }

    ThreadPool::ThreadPool()
      : m_stateMutex( "MT::ThreadPool stateMutex" )
      , m_running( true )
    {
      m_isMainThread = true;
      
      m_stateMutex.acquire();
      size_t numCores = getNumCores();
      m_workerThreads.resize( numCores - 1 );
      for ( size_t i=0; i<m_workerThreads.size(); ++i )
        m_workerThreads[i].start( &ThreadPool::WorkerMainCallback, this );
      m_stateMutex.release();
    }
    
    void ThreadPool::terminate()
    {
      FABRIC_ASSERT( m_running );

      m_stateMutex.acquire();
      m_running = false;
      m_stateCond.broadcast();
      m_stateMutex.release();
      
      for ( size_t i=0; i<m_workerThreads.size(); ++i )
        m_workerThreads[i].waitUntilDone();
    }

    ThreadPool::~ThreadPool()
    {
      if ( m_running )
        terminate();
    }

    void ThreadPool::executeParallel( size_t count, void (*callback)( void *userdata, size_t index ), void *userdata, bool mainThreadOnly )
    {
      if ( count == 1 && (!mainThreadOnly || m_isMainThread.get()) )
        callback( userdata, 0 );
      else
      {
        Task task( count, callback, userdata );
        
        m_stateMutex.acquire();
        
        if ( mainThreadOnly )
          m_mainThreadTasks.push_back( &task );
        else m_tasks.push_back( &task );
        
        // [pzion 20101108] Must wake waiter because there is work to do
        m_stateCond.broadcast();
        
        while ( !task.completed_CRITICAL_SECTION() )
          executeOneTaskIfPossible_CRITICAL_SECTION();
        
        m_stateMutex.release();
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

        m_stateMutex.release();
        task->execute( index );
        m_stateMutex.acquire();
        
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
      m_stateMutex.acquire();
      while ( m_running )
        executeOneTaskIfPossible_CRITICAL_SECTION();
      m_stateMutex.release();
    }
    
    void ThreadPool::WorkerMainCallback( void *_this )
    {
      static_cast<ThreadPool *>(_this)->workerMain();
    }
  };
};
