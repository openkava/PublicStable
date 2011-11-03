/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MT_IDLE_TASK_QUEUE_H
#define _FABRIC_MT_IDLE_TASK_QUEUE_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/Config.h>
#include <Fabric/Core/Util/Mutex.h>
#include <Fabric/Core/Util/Cond.h>
#include <Fabric/Core/Util/Thread.h>

namespace Fabric
{
  namespace MT
  {
    class IdleTaskQueue : public RC::Object
    {
    public:
    
      typedef void (*Callback)( void *userdata );
    
      static RC::Handle<IdleTaskQueue> const &Instance();
    
      void submit( Callback callback, void *userdata );
      void wait() const;
      
      void terminate();
      
    protected:
    
      IdleTaskQueue();
      ~IdleTaskQueue();
      
    private:
    
      void threadEntry();
      static void ThreadEntry( void *userdata )
      {
        static_cast<IdleTaskQueue *>( userdata )->threadEntry();
      }

      struct Task
      {
        Callback callback;
        void *userdata;
      };
      
      mutable std::vector<Task> m_tasks;
      size_t m_executingTasks;
      bool m_exiting;

      mutable Util::Mutex m_stateMutex;
      mutable Util::Cond m_stateCond;
      
      Util::Thread m_thread;

      bool m_terminated;
    };
  };
};

#endif //_FABRIC_MT_IDLE_TASK_QUEUE_H
