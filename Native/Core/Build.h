/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_BUILD_H
#define _FABRIC_BUILD_H

#include <time.h>

namespace Fabric
{
  extern char const *buildName;
  extern char const *buildVersion;
  extern char const *buildNameAndVersion;
  extern char const *buildDesc;
  extern time_t const buildExpiry; 
  extern size_t const buildCacheGeneration;
  extern char const *buildOS;
  extern char const *buildArch;
};

#endif //_FABRIC_BUILD_H
