/*
 *
 *  Created by Peter Zion on 10-11-11.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_MT_MUTEX_H
#define _FABRIC_MT_MUTEX_H

#include <Fabric/Core/MT/Debug.h>

#if defined(FABRIC_OS_MACOSX) || defined(FABRIC_OS_LINUX)
# include <pthread.h>
#elif defined(FABRIC_OS_WINDOWS) 
#include <windows.h>
#else
# error "missing FABRIC_OS_... definition"
#endif

namespace Fabric
{
  namespace MT
  {
    class Mutex
    {
    public:
    
      Mutex( char const *name )
        : m_name( name )
      {
        FABRIC_MT_TRACE( "Mutex[%s]::Mutex()", m_name );
#if defined(FABRIC_OS_WINDOWS) 
        ::InitializeCriticalSection( &m_cs );
#else
        pthread_mutexattr_t   attr;
        pthread_mutexattr_init( &attr );
        pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
                
        int result = pthread_mutex_init( &m_mutex, &attr );
        
        pthread_mutexattr_destroy( &attr );
        
        FABRIC_ASSERT( result == 0 );
#endif
      }
      
      ~Mutex()
      {
        FABRIC_MT_TRACE( "Mutex[%s]::~Mutex()", m_name );
#if defined(FABRIC_OS_WINDOWS) 
        ::DeleteCriticalSection( &m_cs );
#else
        int result = pthread_mutex_destroy( &m_mutex );
        FABRIC_ASSERT( result == 0 );
#endif
      }
      
      class Lock
      {
      public:
      
        Lock( Mutex &mutex )
          : m_mutex( mutex )
        {
          FABRIC_MT_TRACE( "Mutex[%s]::Mutex::Lock()", m_mutex.m_name );
          m_mutex.acquire();
        }
        
        ~Lock()
        {
          FABRIC_MT_TRACE( "Mutex[%s]::Mutex::~Lock()", m_mutex.m_name );
          m_mutex.release();
        }
        
      private:
      
        Mutex &m_mutex;
      };
      
    protected:
    
      void acquire()
      {
        FABRIC_MT_TRACE( "Mutex[%s]::acquire()", m_name );
#if defined(FABRIC_OS_WINDOWS) 
        ::EnterCriticalSection( &m_cs );
#else
        int result = pthread_mutex_lock( &m_mutex );
        FABRIC_ASSERT( result == 0 );
#endif
      }
      
      void release()
      {
        FABRIC_MT_TRACE( "Mutex[%s]::release()", m_name );
#if defined(FABRIC_OS_WINDOWS) 
        ::LeaveCriticalSection( &m_cs );
#else
        int result = pthread_mutex_unlock( &m_mutex );
        FABRIC_ASSERT( result == 0 );
#endif
      }

    private:

      char const *m_name;
      char buf[64];
#if defined(FABRIC_OS_WINDOWS) 
      CRITICAL_SECTION m_cs;
#else
      pthread_mutex_t m_mutex;
#endif
    };
  };
};

#endif //_FABRIC_MT_MUTEX_H
