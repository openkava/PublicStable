/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_EXTERNALS_H
#define _FABRIC_KL_EXTERNALS_H

#include <string>

namespace Fabric
{
  namespace KL
  {
    void *LookupExternalSymbol( std::string const &functionName );
  };
};

#endif //_FABRIC_KL_EXTERNALS_H
