/*
 *
 *  Created by Peter Zion on 10-08-13.
 *  Copyright 2010 Fabric 3D Inc.. All rights reserved.
 *
 */

#include "NPNaClViewPort.h"
#include "Interface.h"
#include <FabricUtil/Exception.h>
#include <FabricGL/OpenGL.h>

using namespace FabricSceneGraph;
using FabricUtil::Exception;

namespace FabricPlugin
{
  FabricNP::Object *NPNaClViewPort::Create()
  {
    FABRIC_PLUGIN_TRACE("NPNaClViewPort::Create");
    return Spec()->create( new NPNaClViewPort );
  }
  
  NPNaClViewPort::NPNaClViewPort()
    : m_npExtensions( 0 )
    , m_pglContext( 0 )
  {
    FABRIC_PLUGIN_TRACE("NPNaClViewPort::NPNaClViewPort()");
  }
  
  NPNaClViewPort::~NPNaClViewPort()
  {
    FABRIC_PLUGIN_TRACE("NPNaClViewPort::~NPNaClViewPort()");
    if ( m_pglContext )
      destroyContext();
  }
    
  void NPNaClViewPort::needsRedraw()
  {
    FABRIC_PLUGIN_TRACE("NPNaClViewPort::needsRedraw()");
    NPN_PluginThreadAsyncCall( FabricNP::npp, &NPNaClViewPort::RedrawCallback, this );
  }
  
  void NPNaClViewPort::RedrawCallback( void *userdata )
  {
    FABRIC_PLUGIN_TRACE( "NPNaClViewPort::RedrawCallback()" );
    NPNaClViewPort *npNaClViewPort = static_cast<NPNaClViewPort *>( userdata );
    npNaClViewPort->redraw();
  }    

  void NPNaClViewPort::setWindow( NPWindow *window )
  {
    FABRIC_PLUGIN_TRACE("NPNaClViewPort::setWindow()");

    m_windowWidth = window->width;
    m_windowHeight = window->height;
    
    if ( !m_pglContext )
      createContext();

    needsRedraw();
  }
    
  void NPNaClViewPort::createContext()
  {
    FABRIC_PLUGIN_TRACE( "NPNaClViewPort::createContext()" );
    NPP npp = FabricNP::npp;
    
    if ( !m_npExtensions )
    {
      NPN_GetValue( npp, NPNVPepperExtensions, reinterpret_cast<void*>(&m_npExtensions) );
      FABRIC_ASSERT( m_npExtensions );
    }
    
    m_device = m_npExtensions->acquireDevice( npp, NPPepper3DDevice );
    FABRIC_ASSERT( m_device );
    
    NPDeviceContext3DConfig config;
    config.commandBufferSize = 1024 * 1024;
    m_device->initializeContext( npp, &config, &m_deviceContext3D );

    m_pglContext = pglCreateContext( npp, m_device, &m_deviceContext3D );
  }

  void NPNaClViewPort::redraw()
  {
    FABRIC_PLUGIN_TRACE( "NPNaClViewPort::redraw()" );
    if ( m_pglContext )
    {
      if ( !pglMakeCurrent( m_pglContext ) && pglGetError() == PGL_CONTEXT_LOST )
      {
        FABRIC_PLUGIN_TRACE_NOTE( "Lost context, recreating" );
        destroyContext();
        createContext();
        FABRIC_ASSERT( pglMakeCurrent( m_pglContext ) );
      }

      if ( m_camera )
        m_camera->render( m_windowWidth, m_windowHeight );
      else FABRIC_PLUGIN_TRACE_NOTE( "No camera; skipping" );
      
      pglSwapBuffers();
      pglMakeCurrent( NULL );
    }
    else FABRIC_PLUGIN_TRACE_NOTE( "No context; skipping" );
  }

  void NPNaClViewPort::destroyContext()
  {
    FABRIC_PLUGIN_TRACE( "NPNaClViewPort::destroyContext()" );

    pglDestroyContext( m_pglContext );
    m_device->destroyContext( FabricNP::npp, &m_deviceContext3D );

    m_pglContext = 0;
  }

  int16_t NPNaClViewPort::handleEvent( void *event )
  {
    //FABRIC_PLUGIN_TRACE( "NPNaClViewPort::handleEvent()" );
  }
};
