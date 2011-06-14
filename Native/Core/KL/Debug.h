/*
 *
 *  Created by Peter Zion on 10-11-22.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_KL_DEBUG_H
#define _FABRIC_KL_DEBUG_H
 
#include <Fabric/Core/Util/Debug.h>

#if defined(FABRIC_BUILD_DEBUG)
namespace Fabric
{
  namespace KL
  {
    extern bool gDebugEnabled;
  };
};
#endif

#define FABRIC_KL_TRACE _FABRIC_TRACE( KL )
#define FABRIC_KL_TRACE_NOTE _FABRIC_TRACE_NOTE
#define FABRIC_KL_TRACE_DUMP_BUFFER( size, data ) _FABRIC_TRACE_DUMP_BUFFER( size, data )

#endif //_FABRIC_KL_DEBUG_H
