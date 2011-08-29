/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_NPAPI_WINDOWED_INVALIDATING_CA_VIEW_PORT_H
#define _FABRIC_NPAPI_WINDOWED_INVALIDATING_CA_VIEW_PORT_H

#include <Fabric/Clients/NPAPI/Darwin/WindowedCAViewPort.h>

#include <npapi/npapi.h>

namespace Fabric
{
  namespace NPAPI
  {
    class WindowedInvalidatingCAViewPort : public WindowedCAViewPort
    {
    public:
    
      static RC::Handle<ViewPort> Create( RC::ConstHandle<Interface> const &interface );

      virtual void redrawFinished();
      
    protected:
    
      WindowedInvalidatingCAViewPort( RC::ConstHandle<Interface> const &interface );
      
      void sendInvalidateMessage();

    private:

      NPP m_npp;
    };
  };
};

#endif //_FABRIC_NPAPI_WINDOWED_INVALIDATING_CA_VIEW_PORT_H
