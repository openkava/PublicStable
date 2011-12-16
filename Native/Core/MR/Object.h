/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_OBJECT_H
#define _FABRIC_MR_OBJECT_H

#include <Fabric/Core/GC/Object.h>

namespace Fabric
{
  namespace MR
  {
    class Object : public GC::Object
    {
      FABRIC_GC_OBJECT_CLASS_DECL();
      
    public:

      Object( FABRIC_GC_OBJECT_CLASS_PARAM );
      ~Object();
    };
  }
}

#endif //_FABRIC_CG_OBJECT_H
