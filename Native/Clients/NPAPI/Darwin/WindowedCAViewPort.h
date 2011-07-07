/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_NPAPI_WINDOWED_CA_VIEW_PORT_H
#define _FABRIC_NPAPI_WINDOWED_CA_VIEW_PORT_H

#include <Fabric/Clients/NPAPI/ViewPort.h>

#include <npapi/npapi.h>
#include <vector>

#ifdef __OBJC__
@class CALayer;
@class NPCAOpenGLLayer;
#else
class CALayer;
class NPCAOpenGLLayer;
#endif

namespace Fabric
{
  namespace NPAPI
  {
    class WindowedCAViewPort : public ViewPort
    {
    public:
    
      static RC::Handle<ViewPort> Create( RC::ConstHandle<Interface> const &interface );

      virtual void getWindowSize( size_t &width, size_t &height ) const;
      void setWindowSize( size_t width, size_t height );
    
      virtual NPError nppGetValue( NPPVariable variable, void *value );
      virtual int16_t nppHandleEvent( void *event );

      virtual void needsRedraw();
      virtual void redrawFinished();

      virtual void pushOGLContext();
      virtual void popOGLContext();

    protected:
    
      WindowedCAViewPort( RC::ConstHandle<Interface> const &interface );
      ~WindowedCAViewPort();

      //virtual void timerFired();

    private:

      NPP m_npp;
      NPCAOpenGLLayer *m_npCAOpenGLLayer;
      size_t m_width, m_height;
      std::vector<CGLContextObj> m_cglContextStack;
    };
  };
};

#endif //_FABRIC_NPAPI_WINDOWED_CA_VIEW_PORT_H
