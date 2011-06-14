/*
 *
 *  Created by Peter Zion on 10-10-12.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_MT_DEBUG_H
#define _FABRIC_MT_DEBUG_H
 
#include <Fabric/Core/Util/Debug.h>
#include <Fabric/Core/Util/Assert.h>

#if defined(FABRIC_BUILD_DEBUG)
namespace Fabric
{
  namespace MT
  {
    extern bool gDebugEnabled;
  };
};
#endif

#define FABRIC_MT_TRACE _FABRIC_TRACE( MT )
#define FABRIC_MT_TRACE_NOTE _FABRIC_TRACE_NOTE
#define FABRIC_MT_TRACE_DUMP_BUFFER( size, data ) _FABRIC_TRACE_DUMP_BUFFER( size, data )

#endif //_FABRIC_MT_DEBUG_H
