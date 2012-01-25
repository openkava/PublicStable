/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Base/Util/SimpleString.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Clients/Python/Client.h>
#include <Fabric/Core/Util/Log.h>
#include <Fabric/Core/Build.h>
#include <Fabric/Core/Util/JSONGenerator.h>

#include <map>
#include <time.h>

//#define FABRIC_PYTHON_DEBUG

namespace Fabric
{
  namespace Python
  {
    // FIXME this shouldn't be global
    std::map<const char*, Util::SimpleString*> g_stringMap;

    extern "C" FABRIC_CLI_EXPORT void* createClient()
    {
      FABRIC_LOG( "%s version %s", Fabric::buildName, Fabric::buildFullVersion );
      struct tm const *lt = localtime( &Fabric::buildExpiry );
      char buf[1024];
      strftime( buf, 1024, "This build of Fabric will expire on %Y-%m-%d at %H:%M:%S", lt );
      FABRIC_LOG( "%s", buf );

      RC::Handle<Client> client = Client::Create();

#ifdef FABRIC_PYTHON_DEBUG
      FABRIC_LOG( "received createClient: %x", client.ptr());
#endif

      client->retain();
      return client.ptr();
    }
    
    extern "C" FABRIC_CLI_EXPORT void jsonExec( void *client_, char const *data, size_t length, const char **result, size_t *rlength )
    {
#ifdef FABRIC_PYTHON_DEBUG
      FABRIC_LOG( "received jsonExec: %x", client_ );
#endif

      RC::Handle<Client> client( static_cast<Client*>( client_ ) );

#ifdef FABRIC_PYTHON_DEBUG
      FABRIC_LOG( "sending to jsonExec: %s", data );
#endif

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

      // FIXME verify doesn't exist already
      g_stringMap[ jsonEncodedResults->c_str() ] = jsonEncodedResults;

#ifdef FABRIC_PYTHON_DEBUG
      FABRIC_LOG( "received from jsonExec: %s", jsonEncodedResults->c_str() );
#endif
    }

    extern "C" FABRIC_CLI_EXPORT void freeString( void *string )
    {
#ifdef FABRIC_PYTHON_DEBUG
      FABRIC_LOG( "received freeString: %x", string );
#endif

      // FIXME error handling
      Util::SimpleString *sstring = g_stringMap.find( (const char*) string )->second;
      delete sstring;
    }

    extern "C" FABRIC_CLI_EXPORT void close( void *client_ )
    {
#ifdef FABRIC_PYTHON_DEBUG
      FABRIC_LOG( "received close: %x", client_ );
#endif

      RC::Handle<Client> client( static_cast<Client*>( client_ ) );
      client->release();
    }
  };
};
