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
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Core/Util/TLS.h>

#include <vector>

namespace Fabric
{
  namespace MT
  {
    class LogCollector;

    class ThreadPool
    {
      class Task 
      {
      public:
      
        Task(
          RC::Handle<LogCollector> const &logCollector,
          size_t count,
          void (*callback)( void *, size_t ),
          void *userdata
          )
          : m_logCollector( logCollector )
          , m_count( count )
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

        RC::Handle<LogCollector> getLogCollector() const
        {
          return m_logCollector;
        }
        
      private:
        
        RC::Handle<LogCollector> m_logCollector;
        size_t m_count;
        void (*m_callback)( void *, size_t );
        void *m_userdata;

        size_t m_nextIndex;
        size_t m_completedCount;
      };
      
    public:

      static ThreadPool *Instance();

      ThreadPool();
      ~ThreadPool();
      
      void executeParallel(
        RC::Handle<LogCollector> const &logCollector,
        size_t count,
        void (*callback)( void *userdata, size_t index ),
        void *userdata,
        bool mainThreadOnly
        );
      
      void terminate();
      
      bool isMainThread() const
      {
        return m_isMainThread.get();
      }
        
    protected:
    
      void executeOneTaskIfPossible_CRITICAL_SECTION();

      void workerMain();    
        
    private:
    
      static void WorkerMainCallback( void *_this );

      Mutex m_stateMutex;
      Cond m_stateCond;
      std::vector<Thread> m_workerThreads;
      std::vector<Task *> m_tasks;
      Util::TLSVar<bool> m_isMainThread;
      std::vector<Task *> m_mainThreadTasks;
      bool m_running;
    };
    
    inline void executeParallel( RC::Handle<LogCollector> const &logCollector, size_t count, void (*callback)( void *userdata, size_t index ), void *userdata, bool mainThreadOnly )
    {
      ThreadPool::Instance()->executeParallel( logCollector, count, callback, userdata, mainThreadOnly );
    }
  };
};

#endif //_FABRIC_MT_IMPL_H
