/*
 *
 *  Created by Peter Zion on 10-10-12.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_OCL_DEBUG_H
#define _FABRIC_OCL_DEBUG_H

#include <Fabric/Core/Util/Debug.h>

#if defined(FABRIC_BUILD_DEBUG)
namespace Fabric
{
  namespace OCL
  {
    extern bool gDebugEnabled;
  };
};
#endif

#define FABRIC_OCL_TRACE _FABRIC_TRACE( OCL )
#define FABRIC_OCL_TRACE_NOTE _FABRIC_TRACE_NOTE
#define FABRIC_OCL_TRACE_DUMP_BUFFER( size, data ) _FABRIC_TRACE_DUMP_BUFFER( size, data )

#endif //_FABRIC_OCL_DEBUG_H
