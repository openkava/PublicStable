/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Base/Util/SimpleString.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Clients/Python/Client.h>
#include <Fabric/Core/Util/Log.h>
#include <Fabric/Core/Build.h>
#include <Fabric/Core/Util/JSONGenerator.h>

#include <time.h>

namespace Fabric
{
  namespace Python
  {
    extern "C" FABRIC_CLI_EXPORT void* createClient()
    {
      FABRIC_LOG( "%s version %s", Fabric::buildName, Fabric::buildFullVersion );
      struct tm const *lt = localtime( &Fabric::buildExpiry );
      char buf[1024];
      strftime( buf, 1024, "This build of Fabric will expire on %Y-%m-%d at %H:%M:%S", lt );
      FABRIC_LOG( "%s", buf );

      RC::Handle<Client> client = Client::Create();
      FABRIC_LOG( "received createClient: %x", client.ptr());

      client->retain();
      return client.ptr();
    }
    
    extern "C" FABRIC_CLI_EXPORT void jsonExec( void *client_, char const *data, size_t length, const char **result, size_t *rlength )
    {
      FABRIC_LOG( "received jsonExec: %x", client_ );

      RC::Handle<Client> client( static_cast<Client*>( client_ ) );
      FABRIC_LOG("GOT: %s", data);

      // FIXME track these pointers
      Util::SimpleString *jsonEncodedResults = new Util::SimpleString();
      Util::JSONGenerator resultJSON( jsonEncodedResults );
      
      try
      {
        client->jsonExec( data, length, resultJSON );
      }
      catch ( Exception e )
      {
        FABRIC_LOG( "exception: " + e.getDesc() );
      }

      *rlength = jsonEncodedResults->length();
      *result = jsonEncodedResults->c_str();

      FABRIC_LOG( "result ptr = %x", *result );

      FABRIC_LOG( "OUTPUT: %s", jsonEncodedResults->c_str() );
    }

    extern "C" FABRIC_CLI_EXPORT void freeString( void *string )
    {
      FABRIC_LOG( "received freeString: %x", string );

      Util::SimpleString *sstring = (Util::SimpleString*)string;
      //delete sstring;
    }

    extern "C" FABRIC_CLI_EXPORT void close( void *client_ )
    {
      FABRIC_LOG( "received close: %x", client_ );

      RC::Handle<Client> client( static_cast<Client*>( client_ ) );
      client->release();
    }
  };
};
