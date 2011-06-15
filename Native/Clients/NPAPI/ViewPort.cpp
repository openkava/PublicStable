/*
 *
 *  Created by Peter Zion on 10-08-13.
 *  Copyright 2010 Fabric 3D Inc.. All rights reserved.
 *
 */

#include "ViewPort.h"
#include "Interface.h"
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/RT/IntegerDesc.h>
#include <Fabric/Clients/NPAPI/Context.h>
#include <Fabric/Core/DG/Node.h>
#include <Fabric/Core/DG/Event.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Base/JSON/Scalar.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>

#if defined(FABRIC_OS_WINDOWS)
# include <intrin.h>
#endif

namespace Fabric
{
  namespace NPAPI
  {
    ViewPort::ViewPort( RC::ConstHandle<Interface> const &interface, uint32_t timerInterval )
      : m_npp( interface->getNPP() )
      , m_interface( interface.ptr() )
      , m_logCollector( interface->getContext()->getLogCollector() )
      , m_redrawFinishedCallback( 0 )
      , m_redrawFinishedCallbackPendingInvokeCount( 0 )
      , m_fpsCount( 0 )
      , m_fps( 0.0 )
    {
      if ( timerInterval )
      {
        m_timer = NPN_ScheduleTimer( m_npp, timerInterval, true, &ViewPort::TimerFiredCallback );
        s_timers[m_npp].insert( Util::UnorderedMap< uint32_t, ViewPort *>::value_type( m_timer, this ) );
      }
      else m_timer = 0;

      RC::Handle<DG::Context> context = interface->getContext();
      RC::Handle<RT::Manager> rtManager = context->getRTManager();
      m_integerDesc = rtManager->getIntegerDesc();
      
      m_windowNode = DG::Node::Create( "FABRIC.window", context );
      m_windowNode->addMember( "width", m_integerDesc, m_integerDesc->getDefaultData() );
      m_windowNode->addMember( "height", m_integerDesc, m_integerDesc->getDefaultData() );
       
      m_redrawEvent = DG::Event::Create( "FABRIC.window.redraw", context );

#if defined(FABRIC_OS_WINDOWS)
      LARGE_INTEGER      fpsTimerFreq;
      ::QueryPerformanceFrequency( &fpsTimerFreq );
      m_fpsTimerFreq = double( fpsTimerFreq.QuadPart );
      ::QueryPerformanceCounter( &m_fpsStart );
#else
      gettimeofday( &m_fpsStart, NULL );
#endif

      m_interface->getContext()->registerViewPort( "viewPort", this );
    }
    
    ViewPort::~ViewPort()
    {
      m_interface->getContext()->unregisterViewPort( "viewPort", this );

      if ( m_redrawFinishedCallback )
        NPN_ReleaseObject( m_redrawFinishedCallback );
    }
    
    void ViewPort::issuePendingRedrawFinishedCallbacks()
    {
      while ( m_redrawFinishedCallbackPendingInvokeCount > 0 )
      {
        std::vector<std::string> src;
        src.push_back("VP");
        src.push_back("viewPort");
        
        m_interface->getContext()->jsonNotify( src, "redrawFinished", 0 );
        
#if defined(FABRIC_OS_WINDOWS)
        ::_InterlockedDecrement( &m_redrawFinishedCallbackPendingInvokeCount );
#else
        __sync_sub_and_fetch( &m_redrawFinishedCallbackPendingInvokeCount, 1 );
#endif
      }
    }
    
    void ViewPort::timerFired()
    {
    }
    
    void ViewPort::TimerFiredCallback( NPP npp, uint32_t timer )
    {
      Util::UnorderedMap< uint32_t, ViewPort *> &timers = s_timers[npp];
      Util::UnorderedMap< uint32_t, ViewPort *>::iterator it = timers.find( timer );
      FABRIC_ASSERT( it != timers.end() );
      FABRIC_ASSERT( it->second->m_npp == npp );
      it->second->timerFired();
    }

    RC::Handle<MT::LogCollector> ViewPort::getLogCollector() const
    {
      return m_logCollector;
    }
      
    void ViewPort::redrawFinished()
    {
      ++m_fpsCount;
       
      double secDiff;

#if defined(FABRIC_OS_WINDOWS)
      LARGE_INTEGER    fpsNow;
      ::QueryPerformanceCounter( &fpsNow );

      secDiff = double(fpsNow.QuadPart - m_fpsStart.QuadPart) / m_fpsTimerFreq;
#else
      struct timeval fpsNow;
      gettimeofday( &fpsNow, NULL );
      secDiff = double((int)fpsNow.tv_sec - (int)m_fpsStart.tv_sec) + (double((int)fpsNow.tv_usec - (int)m_fpsStart.tv_usec) / 1.0e6);
#endif

      if ( secDiff >= 1.0 )
      {
        m_fps = float( m_fpsCount / secDiff );
        char buffer[64];
        sprintf( buffer, "%0.2f fps", m_fps );
        static const bool logFPS = false;
        if ( logFPS )
        {
          if ( m_logCollector )
          {
            m_logCollector->add( buffer );
            m_logCollector->flush();
          }
        }
        else FABRIC_DEBUG_LOG( "%s", buffer );

        memcpy( &m_fpsStart, &fpsNow, sizeof(m_fpsStart) );
        m_fpsCount = 0;
      }
      
      m_logCollector->flush();

#if defined(FABRIC_OS_WINDOWS)
      ::_InterlockedIncrement( &m_redrawFinishedCallbackPendingInvokeCount );
#else
      __sync_fetch_and_add( &m_redrawFinishedCallbackPendingInvokeCount, 1 );
#endif

      NPN_PluginThreadAsyncCall( m_npp, &ViewPort::IssuePendingRedrawFinishedCallbacks, this );
    }
    
    void ViewPort::setRedrawFinishedCallback( NPObject *npObject )
    {
      if ( m_redrawFinishedCallback != npObject )
      {
        if ( m_redrawFinishedCallback )
          NPN_ReleaseObject(m_redrawFinishedCallback);
        m_redrawFinishedCallback = npObject;
        if ( m_redrawFinishedCallback )
          NPN_RetainObject(m_redrawFinishedCallback);
      }
    }
    
    void ViewPort::didResize( size_t width, size_t height )
    {
      m_windowNode->setData( "width", 0, &width );
      m_windowNode->setData( "height", 0, &height );    
    }
    
    NPError ViewPort::nppGetValue( NPPVariable variable, void *value )
    {
      return NPERR_INVALID_PARAM;
    }
 
    NPError ViewPort::nppSetWindow( NPWindow *npWindow )
    {
      return NPERR_NO_ERROR;
    }
    
    int16_t ViewPort::nppHandleEvent( void *event )
    {
      return false;
    }

    NPError ViewPort::nppDestroy( NPSavedData** save )
    {
      if ( m_timer )
      {
        Util::UnorderedMap< uint32_t, ViewPort *> &timers = s_timers[m_npp];
        Util::UnorderedMap< uint32_t, ViewPort *>::iterator it = timers.find( m_timer );
        FABRIC_ASSERT( it != timers.end() );
        timers.erase( it );
        NPN_UnscheduleTimer( m_npp, m_timer );
      }
      
      return NPERR_NO_ERROR;
    }

    ViewPort::Timers ViewPort::s_timers;

    RC::Handle<DG::Node> ViewPort::getWindowNode() const
    {
      return m_windowNode;
    }
    
    RC::Handle<DG::Event> ViewPort::getRedrawEvent() const
    {
      return m_redrawEvent;
    }

    RC::ConstHandle<Interface> ViewPort::getInterface() const
    {
      return m_interface;
    }

    RC::Handle<JSON::Object> ViewPort::jsonDesc() const
    {
      size_t width, height;
      getWindowSize( width, height );
      
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( "fps", JSON::Scalar::Create( m_fps ) );
      result->set( "width", JSON::Integer::Create( width ) );
      result->set( "height", JSON::Integer::Create( height ) );
      result->set( "windowNode", JSON::String::Create( m_windowNode->getName() ) );
      result->set( "redrawEvent", JSON::String::Create( m_redrawEvent->getName() ) );
      return result;
    }

    RC::ConstHandle<JSON::Value> ViewPort::jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Value> result;
      if ( cmd == "needsRedraw" )
        needsRedraw();
      else throw Exception( "unrecognized command" );
      return result;
    }
  };
};
