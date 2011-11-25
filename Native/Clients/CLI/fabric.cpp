/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Clients/CLI/ClientFactory.h>
#include <Fabric/Core/Util/Log.h>
#include <Fabric/Core/Build.h>

#include <time.h>

namespace Fabric
{
  namespace CLI
  {
    extern "C" FABRIC_CLI_EXPORT void init( v8::Handle<v8::Object> target )
    {
      FABRIC_LOG( "%s version %s", Fabric::buildName, Fabric::buildFullVersion );
      struct tm const *lt = localtime( &Fabric::buildExpiry );
      char buf[1024];
      strftime( buf, 1024, "This build of Fabric will expire on %Y-%m-%d at %H:%M:%S", lt );
      FABRIC_LOG( "%s", buf );

      v8::HandleScope handleScope;
      target->Set( v8::String::New("createClient"), CreateClientV8Function() );
    }
  };
};
