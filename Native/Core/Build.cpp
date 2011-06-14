/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/Build.h>

#define FABRIC_COPYRIGHT_YEARS "2010-2011"

namespace Fabric
{
  char const *buildVersion = FABRIC_VERSION_STRING;
  char const *buildDesc = 
    "Fabric version " FABRIC_VERSION_STRING "\n"
    "Built on " __DATE__ " at " __TIME__ "\n"
    "Copyright " FABRIC_COPYRIGHT_YEARS " Fabric Technologies Inc. All rights reserved.\n";
  char const *buildHash = FABRIC_VERSION_STRING " - " __DATE__ " - " __TIME__;
  time_t const buildExpiry = 1310318166;
};
