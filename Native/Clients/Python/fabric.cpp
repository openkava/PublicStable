/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Base/Util/SimpleString.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Clients/Python/Client.h>
#include <Fabric/Base/Util/Log.h>
#include <Fabric/Core/Build.h>

#include <map>
#include <time.h>

//#define FABRIC_PYTHON_DEBUG

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

#ifdef FABRIC_PYTHON_DEBUG
      FABRIC_LOG( "calling createClient: %x", client.ptr());
#endif

      client->retain();
      return client.ptr();
    }
    
    extern "C" FABRIC_CLI_EXPORT void jsonExec( void *client_, char const *data, size_t length, const char **result )
    {
#ifdef FABRIC_PYTHON_DEBUG
      FABRIC_LOG( "calling jsonExec: %x", client_ );
#endif

      RC::Handle<Client> client( static_cast<Client*>( client_ ) );

#ifdef FABRIC_PYTHON_DEBUG
      FABRIC_LOG( "sending to jsonExec: %s", data );
#endif

      client->jsonExecAndAllocCStr( data, length, result );

#ifdef FABRIC_PYTHON_DEBUG
      FABRIC_LOG( "received from jsonExec: %s", *result );
#endif
    }

    extern "C" FABRIC_CLI_EXPORT void freeString( void *client_, void *string )
    {
#ifdef FABRIC_PYTHON_DEBUG
      FABRIC_LOG( "calling freeString: %x (%x)", client_, string );
#endif

      RC::Handle<Client> client( static_cast<Client*>( client_ ) );
      client->freeJsonCStr( (const char *)string );
    }

    extern "C" FABRIC_CLI_EXPORT void setJSONNotifyCallback(
        void *client_,
        void (*callback)(const char *)
        )
    {
#ifdef FABRIC_PYTHON_DEBUG
      FABRIC_LOG( "calling setJSONNotifyCallback: %x", client_ );
#endif

      RC::Handle<Client> client( static_cast<Client*>( client_ ) );
      client->setJSONNotifyCallback( callback );
    }

    extern "C" FABRIC_CLI_EXPORT void close( void *client_ )
    {
#ifdef FABRIC_PYTHON_DEBUG
      FABRIC_LOG( "calling close: %x", client_ );
#endif

      RC::Handle<Client> client( static_cast<Client*>( client_ ) );
      client->close();
    }

    extern "C" FABRIC_CLI_EXPORT void waitForClose( void *client_ )
    {
#ifdef FABRIC_PYTHON_DEBUG
      FABRIC_LOG( "calling waitForClose: %x", client_ );
#endif

      RC::Handle<Client> client( static_cast<Client*>( client_ ) );
      client->waitForClose();
      client->release();
    }
  };
};
