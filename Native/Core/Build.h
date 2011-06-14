/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_BUILD_H
#define _FABRIC_BUILD_H

#include <time.h>

#define FABRIC_VERSION_STRING_BUILDER(x,y,z,t) x "." y "." z "." t
#define FABRIC_VERSION_STRING \
  FABRIC_VERSION_STRING_BUILDER( FABRIC_VERSION_MAJ, FABRIC_VERSION_MIN, FABRIC_VERSION_REV, FABRIC_VERSION_BUILD )

namespace Fabric
{
  extern char const *buildVersion;
  extern char const *buildDesc;
  extern char const *buildHash;
  extern time_t const buildExpiry; 
};

#endif //_FABRIC_BUILD_H
