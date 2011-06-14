/*
 *
 *  Created by Peter Zion on 10-09-27.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */
 
#ifndef _FABRIC_MT_TASK_H
#define _FABRIC_MT_TASK_H

#include <Fabric/Core/MT/Debug.h>

namespace Fabric
{
  namespace MT
  {
    class TaskBase
    {
    public:
    
      virtual void execute( void *userdata ) const = 0;
    };
    
    template< class T > class Task : public TaskBase
    {
    public:
    
      typedef void (T::*ExecuteCallback)( void *userdata );
      
      Task(  T *t, ExecuteCallback executeCallback )
        : m_t( t )
        , m_executeCallback( executeCallback )
      {
        FABRIC_MT_TRACE( "Task<>::Task( %p, %p )", t, executeCallback );
      }

      virtual void execute( void *userdata ) const
      {
        FABRIC_MT_TRACE( "Task<>::execute( %p )", userdata );
        (m_t->*m_executeCallback)( userdata );
      }
    
    private:
    
      T *m_t;
      ExecuteCallback m_executeCallback;
    };
  };
};

#endif //_FABRIC_MT_TASK_H
