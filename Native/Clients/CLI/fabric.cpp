/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Clients/CLI/ClientFactory.h>

namespace Fabric
{
  namespace CLI
  {
    extern "C" __attribute__ ((visibility("default"))) void init( v8::Handle<v8::Object> target )
    {
      v8::HandleScope handleScope;
      target->Set( v8::String::New("createClient"), CreateClientV8Function() );
    }
  };
};
