/*
 *
 *  Created by Peter Zion on 10-10-12.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_OGL_DEBUG_H
#define _FABRIC_OGL_DEBUG_H

#include <Fabric/Core/Util/Debug.h>

#if defined(FABRIC_BUILD_DEBUG)
namespace Fabric
{
  namespace OGL
  {
    extern bool gDebugEnabled;
  };
};
#endif

#define FABRIC_OGL_TRACE _FABRIC_TRACE( OGL )
#define FABRIC_OGL_TRACE_NOTE _FABRIC_TRACE_NOTE
#define FABRIC_OGL_TRACE_DUMP_BUFFER( size, data ) _FABRIC_TRACE_DUMP_BUFFER( size, data )

#endif //_FABRIC_OGL_DEBUG_H
