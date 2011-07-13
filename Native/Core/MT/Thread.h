/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MT_THREAD_H
#define _FABRIC_MT_THREAD_H

#include <Fabric/Base/Exception.h>

#if defined(FABRIC_POSIX)
# include <pthread.h>
#elif defined(FABRIC_WIN32) 
# include <windows.h>
#else
# error "missing FABRIC_PLATFORM_... definition"
#endif

namespace Fabric
{
  namespace MT
  {
    class Thread
    {
    public:
    
      Thread()
      {
#if defined(FABRIC_POSIX)
        m_posixThread = 0;
#elif defined(FABRIC_WIN32)
        m_windowsThread = NULL;
#else
# error "missing FABRIC_PLATFORM_... definition"
#endif
      }
      
      void start( void (*callback)( void * ), void *userdata )
      {
        m_startInfo.callback = callback;
        m_startInfo.userdata = userdata;
        
#if defined(FABRIC_POSIX)
        FABRIC_ASSERT( !m_posixThread );
        m_posixThread = new pthread_t;
        if ( pthread_create( m_posixThread, 0, &Thread::Main, &m_startInfo ) != 0 )
        {
          delete m_posixThread;
          m_posixThread = 0;
          throw Exception( "pthread_create(): unknown failure" );
        }
#elif defined(FABRIC_WIN32)
        FABRIC_ASSERT( m_windowsThread == NULL );
        m_windowsThread = (HANDLE)_beginthreadex( NULL, 0, &Thread::Main, &m_startInfo, 0, NULL );
        if ( m_windowsThread == NULL )
          throw Exception( "_beginthreadex(): unknown failure" );
#else
# error "missing FABRIC_PLATFORM_... definition"
#endif
      }
      
      void waitUntilDone()
      {
#if defined(FABRIC_POSIX)
        FABRIC_ASSERT( m_posixThread );
        if ( pthread_join( *m_posixThread, 0 ) != 0 )
          throw Exception( "pthread_join(): unknown failure" );
        delete m_posixThread;
        m_posixThread = 0;
#elif defined(FABRIC_WIN32)
        FABRIC_ASSERT( m_windowsThread != NULL );
        if( ::WaitForSingleObject( m_windowsThread, INFINITE ) == WAIT_FAILED )
          throw Exception( "WaitForSingleObject(): unknown failure" );
        ::CloseHandle( m_windowsThread );
        m_windowsThread = NULL;
#else
# error "missing FABRIC_PLATFORM_... definition"
#endif
      }
      
      ~Thread()
      {
#if defined(FABRIC_POSIX)
        FABRIC_ASSERT( !m_posixThread );
#elif defined(FABRIC_WIN32)
        FABRIC_ASSERT( m_windowsThread == NULL );
#else
# error "missing FABRIC_PLATFORM_... definition"
#endif
      }

    private:
    
      struct StartInfo
      {
        void (*callback)( void * );
        void *userdata;
      };

#if defined(FABRIC_POSIX)
      static void *Main( void *userdata )
      {
        StartInfo const *startInfo = static_cast<StartInfo const *>( userdata );
        startInfo->callback( startInfo->userdata );
        pthread_exit(0);
        return 0;
      }
#elif defined(FABRIC_WIN32)
      static unsigned __stdcall Main( void *userdata )
      {
        StartInfo const *startInfo = static_cast<StartInfo const *>( userdata );
        startInfo->callback( startInfo->userdata );
        return 0;
      }
#else
# error "missing FABRIC_PLATFORM_... definition"
#endif
      
#if defined(FABRIC_POSIX)
      pthread_t *m_posixThread;
#elif defined(FABRIC_WIN32)
      HANDLE m_windowsThread;
#else
# error "missing FABRIC_PLATFORM_... definition"
#endif
      StartInfo m_startInfo;
    };
  };
};

#endif //_FABRIC_MT_THREAD_H
