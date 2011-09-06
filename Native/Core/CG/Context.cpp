/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/CG/Context.h>

namespace Fabric
{
  namespace CG
  {
    RC::Handle<Context> Context::Create()
    {
      return new Context;
    }
    
    Context::Context()
    {
    }
  };
};
