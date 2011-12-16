/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/Object.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( Object, GC::Object )
    
    Object::Object( FABRIC_GC_OBJECT_CLASS_PARAM )
      : GC::Object( FABRIC_GC_OBJECT_CLASS_ARG )
    {
    }

    Object::~Object()
    {
    }
  }
}
