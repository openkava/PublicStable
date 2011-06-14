/*
 *
 *  Created by Peter Zion on 10-08-13.
 *  Copyright 2010 Fabric 3D Inc.. All rights reserved.
 *
 */

#ifndef _FABRIC_PLUGIN_NP_NA_CL_VIEW_PORT_H
#define _FABRIC_PLUGIN_NP_NA_CL_VIEW_PORT_H

#include "ViewPort.h"

#include <nacl/npupp.h>
#include <pgl/pgl.h>

namespace FabricPlugin
{
  class NPNaClViewPort : public ViewPort
  {
  public:
  
    static FabricNP::Object *Create();
    
    void needsRedraw();
    
  protected:
  
    NPNaClViewPort();
    ~NPNaClViewPort();
  
    virtual void setWindow( NPWindow *window );
    virtual int16_t handleEvent( void *event );
    
  private:  
  
    void createContext();
    void destroyContext();

    static void RedrawCallback( void *userdata );
    void redraw();
    
    NPExtensions* m_npExtensions;
    NPDevice *m_device;
    NPDeviceContext3D m_deviceContext3D;
    PGLContext m_pglContext;
    int m_windowWidth, m_windowHeight;
  };
};

#endif //_FABRIC_PLUGIN_NP_NA_CL_VIEW_PORT_H
