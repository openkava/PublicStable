/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MT_IMPL_H
#define _FABRIC_MT_IMPL_H

#include <Fabric/Core/MT/Debug.h>
#include <Fabric/Core/MT/Mutex.h>
#include <Fabric/Core/MT/Cond.h>
#include <Fabric/Core/MT/Thread.h>
#include <Fabric/Core/MT/Cond.h>
#include <Fabric/Core/Util/TLS.h>

#include <vector>

namespace Fabric
{
  namespace MT
  {
    class ThreadPool
    {
      class Task 
      {
      public:
      
        Task( size_t count, void (*callback)( void *, size_t ), void *userdata )
          : m_count( count )
          , m_callback( callback )
          , m_userdata( userdata )
          , m_nextIndex( 0 )
          , m_completedCount( 0 )
        {
        }
        
        ~Task()
        {
          FABRIC_ASSERT( m_nextIndex == m_count );
          FABRIC_ASSERT( m_completedCount == m_count );
        }
        
        void preExecute_CRITICAL_SECTION( size_t &index, bool &keep )
        {
          FABRIC_ASSERT( m_nextIndex < m_count );
          index = m_nextIndex++;
          keep = m_nextIndex < m_count;
        }
        
        void execute( size_t index )
        {
          m_callback( m_userdata, index );
        }
        
        void postExecute_CRITICAL_SECTION()
        {
          FABRIC_ASSERT( m_completedCount < m_count );
          ++m_completedCount;
        }
        
        bool completed_CRITICAL_SECTION() const
        {
          return m_completedCount == m_count;
        }
        
      private:
        
        size_t m_count;
        void (*m_callback)( void *, size_t );
        void *m_userdata;

        size_t m_nextIndex;
        size_t m_completedCount;
      };
      
    public:

      static ThreadPool *Instance()
      {
        return &m_singleton;
      }

      static void Term( )
      {
#if defined(FABRIC_WIN32)
        // On Windows, threads can't be destroyed when the parent
        // thread is being destroyed. This causes issues with Chrome
        // in single-process mode and all the other browsers. 
        // We have to terminate the threadpool manually from the
        // NPAPI destroy callback.
        Instance()->terminate();
#endif
      }
      
      ThreadPool();
      ~ThreadPool();
      
      void executeParallel( size_t count, void (*callback)( void *userdata, size_t index ), void *userdata, bool mainThreadOnly );
        
    protected:
    
      void executeOneTaskIfPossible_CRITICAL_SECTION();

      void workerMain();    
        
    private:
      static ThreadPool m_singleton;

#if defined( FABRIC_POSIX )
      static void WorkerMainCallback( void *_this );

      Mutex m_stateMutex;
      Cond m_stateCond;
      std::vector<Thread> m_workerThreads;
#elif defined( FABRIC_WIN32)
      static unsigned __stdcall WorkerMainCallback( void *_this );

      void terminate();

      CRITICAL_SECTION    m_cs;
      HANDLE              m_hWakeup;
      std::vector<HANDLE> m_workerThreads;
      std::vector<HANDLE> m_workerExit;
      bool                m_running;

      struct ThreadData
      {
          ThreadPool  *m_this;
          HANDLE      m_exitEvent;
      };
#endif
      std::vector<Task *> m_tasks;
      Util::TLSVar<bool> m_isMainThread;
      std::vector<Task *> m_mainThreadTasks;
      bool m_exiting;
    };
    
    inline void executeParallel( size_t count, void (*callback)( void *userdata, size_t index ), void *userdata, bool mainThreadOnly )
    {
      ThreadPool::Instance()->executeParallel( count, callback, userdata, mainThreadOnly );
    }
  };
};

#endif //_FABRIC_MT_IMPL_H
