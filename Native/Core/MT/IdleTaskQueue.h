/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MT_IDLE_TASK_QUEUE_H
#define _FABRIC_MT_IDLE_TASK_QUEUE_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/Config.h>

#if defined(FABRIC_OS_LINUX) || defined(FABRIC_OS_MACOSX)
# define FABRIC_MT_IDLE_TASK_QUEUE_PTHREAD
#elif defined(FABRIC_OS_WINDOWS)
# define FABRIC_MT_IDLE_TASK_QUEUE_WIN32
#else
# error "Unsupported platform"
#endif


#if defined(FABRIC_MT_IDLE_TASK_QUEUE_GCD)
# include <dispatch/dispatch.h>
#elif defined(FABRIC_MT_IDLE_TASK_QUEUE_PTHREAD)
# include <pthread.h>
# include <vector>
#elif defined(FABRIC_MT_IDLE_TASK_QUEUE_WIN32)
# include <windows.h>
# include <queue>
#else
# error "Unsupported platform"
#endif

namespace Fabric
{
  namespace MT
  {
    class IdleTaskQueue : public RC::Object
    {
    public:
    
      typedef void (*Callback)( void *userdata );
    
      static RC::Handle<IdleTaskQueue> Instance();
    
      void submit( Callback callback, void *userdata );
      void wait() const;
      
    protected:
    
      IdleTaskQueue();
      ~IdleTaskQueue();
      
    private:
    
#if defined(FABRIC_MT_IDLE_TASK_QUEUE_PTHREAD)
      void *threadEntry();
      static void *ThreadEntry( void *userdata )
      {
        return static_cast<IdleTaskQueue *>( userdata )->threadEntry();
      }
#elif defined(FABRIC_MT_IDLE_TASK_QUEUE_WIN32)
      unsigned threadEntry();
      static unsigned __stdcall ThreadEntry( void *userdata )
      {
        return static_cast<IdleTaskQueue *>( userdata )->threadEntry();
      }
#endif

#if defined(FABRIC_MT_IDLE_TASK_QUEUE_GCD)
      dispatch_queue_t m_dispatchQueue;
      dispatch_group_t m_dispatchGroup;
#elif defined(FABRIC_MT_IDLE_TASK_QUEUE_PTHREAD)
      struct Task
      {
        Callback callback;
        void *userdata;
      };
      
      mutable std::vector<Task> m_tasks;
      size_t m_executingTasks;
      bool m_exiting;

      mutable pthread_mutex_t m_stateMutex;
      mutable pthread_cond_t m_stateCond;
      
      pthread_attr_t m_threadAttr;
      pthread_t m_thread;
#elif defined(FABRIC_MT_IDLE_TASK_QUEUE_WIN32)
      struct Task
      {
        Callback callback;
        void *userdata;
      };
      
      mutable std::queue<Task> m_tasks;
      bool m_exiting;

      mutable CRITICAL_SECTION m_csTasks;
      mutable CRITICAL_SECTION m_csExecute;
      HANDLE              m_hWakeup;
      HANDLE              m_hIdleThread;
#else
# error "Unsupported platform"
#endif
    };
  };
};

#endif //_FABRIC_MT_IDLE_TASK_QUEUE_H
