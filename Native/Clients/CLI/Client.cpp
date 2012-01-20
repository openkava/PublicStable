/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Clients/CLI/Client.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/EDK/EDK.h>
#include <Fabric/Core/IO/Helpers.h>
#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Core/IO/ResourceManager.h>
#include <Fabric/Core/Plug/Manager.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/DG/Context.h>

#include <node.h>
#include <string>

namespace Fabric
{
  namespace CLI
  {
    class TestSynchronousFileResourceProvider : public IO::ResourceProvider
    {
    public:

      static RC::Handle<TestSynchronousFileResourceProvider> Create()
      {
        return new TestSynchronousFileResourceProvider();
      }

      virtual char const * getUrlScheme() const
      {
        return "testfile";
      }

      virtual void get( char const *url, bool getAsFile, void* userData )
      {
        if( strncmp( "testfile://", url, 11 ) != 0 )
            throw Exception( "Error: URL not properly formatted for local files" );//Don't put filename as it might be private

        std::string fileWithPath = ChangeSeparatorsURLToFile( std::string( url+11 ) );
        if( !FileExists( fileWithPath ) )
            throw Exception( "Error: file doesn't exist" );//Don't put filename as it might be private

        std::string extension = GetExtension( fileWithPath );
        size_t fileSize = GetFileSize( fileWithPath );

        if( getAsFile )
        {
          ResourceManager::onFileAsynchThreadCall( fileWithPath.c_str(), userData );
          ResourceManager::onProgressAsynchThreadCall( extension.c_str(), fileSize, fileSize, userData );
        }
        else
        {
          FILE *fp = fopen( fileWithPath.c_str(), "rb" );
          if ( fp == NULL )
            ResourceManager::onFailureAsynchThreadCall( "Unable to open file", userData );

          static const size_t maxReadSize = 1<<16;//64K buffers
          uint8_t *data = static_cast<uint8_t *>( malloc(maxReadSize) );
          size_t offset = 0;
          for (;;)
          {
            size_t readSize = fread( &data[0], 1, maxReadSize, fp );
            if ( ferror( fp ) )
              ResourceManager::onFailureAsynchThreadCall( "Error while reading file", userData );

            ResourceManager::onDataAsynchThreadCall( offset, readSize, data, userData );
            ResourceManager::onProgressAsynchThreadCall( "text/plain", offset+readSize, fileSize, userData );

            offset += readSize;
            if ( offset == fileSize )
              break;
          }
          free( data );
          fclose( fp );
        }
      }

    private:
      TestSynchronousFileResourceProvider(){}
    };

    void ScheduleAsynchCallback( void* scheduleUserData, void (*callbackFunc)(void *), void *callbackFuncUserData )
    {
      //In fact it's not asynch for our unit test purpose...
      (*callbackFunc)( callbackFuncUserData );
    }

    class IOManager : public IO::Manager
    {
    public:
    
      static RC::Handle<IOManager> Create()
      {
        return new IOManager;
      }

      virtual std::string queryUserFilePath(
        bool existingFile,
        std::string const &title,
        std::string const &defaultFilename,
        std::string const &extension
        ) const
      {
        //For unit test purposes only
        if ( !IO::DirExists( "TMP" ) )
	        IO::CreateDir( "TMP" );
        if(existingFile)
          return IO::JoinPath("TMP", "default.txt");
        else
          return IO::JoinPath("TMP", defaultFilename);
      }
  
    protected:
    
      IOManager() : IO::Manager( ScheduleAsynchCallback, NULL )
      {
        getResourceManager()->registerProvider( RC::Handle<IO::ResourceProvider>::StaticCast( TestSynchronousFileResourceProvider::Create() ), true );
      }
    };
    
    RC::Handle<Client> Client::Create( RC::Handle<DG::Context> const &context, ClientWrap *clientWrap )
    {
      return new Client( context, clientWrap );
    }
    
    Client::Client( RC::Handle<DG::Context> const &context, ClientWrap *clientWrap )
      : DG::Client( context )
      , m_clientWrap( clientWrap )
    {
    }
    
    void Client::invalidate()
    {
      m_clientWrap = 0;
    }
    
    Client::~Client()
    {
      invalidate();
    }
    
    void Client::notify( Util::SimpleString const &jsonEncodedNotifications ) const
    {
      if ( m_clientWrap )
        m_clientWrap->notify( jsonEncodedNotifications.data(), jsonEncodedNotifications.length() );
    }
    
    void ClientWrap::Initialize( v8::Handle<v8::Object> target )
    {
      v8::HandleScope v8HandleScope;
      
      node::HandleWrap::Initialize( target );
    
      v8::Local<v8::FunctionTemplate> v8FunctionTemplate = v8::FunctionTemplate::New( New );
      v8FunctionTemplate->InstanceTemplate()->SetInternalFieldCount( 1 );
      v8FunctionTemplate->SetClassName( v8::String::New( "Client" ) );
      
      NODE_SET_PROTOTYPE_METHOD( v8FunctionTemplate, "jsonExec", JSONExec );
      NODE_SET_PROTOTYPE_METHOD( v8FunctionTemplate, "setJSONNotifyCallback", SetJSONNotifyCallback );
      NODE_SET_PROTOTYPE_METHOD( v8FunctionTemplate, "close", Close );
      
      target->Set( v8::String::New( "Client" ), v8FunctionTemplate->GetFunction() );
    }
    
    v8::Handle<v8::Value> ClientWrap::New( v8::Arguments const &args )
    {
      // This constructor should not be exposed to public javascript.
      // Therefore we assert that we are not trying to call this as a
      // normal function.
      FABRIC_ASSERT( args.IsConstructCall() );

      v8::HandleScope v8HandleScope;
      ClientWrap *clientWrap = new ClientWrap( args.This() );
      FABRIC_ASSERT( clientWrap );
      
      return v8HandleScope.Close( args.This() );
    }
    
    ClientWrap::ClientWrap( v8::Handle<v8::Object> object )
      : node::HandleWrap( object, 0 )
      , m_mutex("Node.js ClientWrap")
    {
      std::vector<std::string> pluginPaths;
#if defined(FABRIC_OS_MACOSX)
      char const *home = getenv("HOME");
      if ( home && *home )
      {
        std::string homePath( home );
        pluginPaths.push_back( IO::JoinPath( homePath, "Library", "Fabric", "Exts" ) );
      }
      pluginPaths.push_back( "/Library/Fabric/Exts" );
#elif defined(FABRIC_OS_LINUX)
      char const *home = getenv("HOME");
      if ( home && *home )
      {
        std::string homePath( home );
        pluginPaths.push_back( IO::JoinPath( homePath, ".fabric", "Exts" ) );
      }
      pluginPaths.push_back( "/usr/lib/fabric/Exts" );
#elif defined(FABRIC_OS_WINDOWS)
      char const *appData = getenv("APPDATA");
      if ( appData && *appData )
      {
        std::string appDataDir(appData);
        pluginPaths.push_back( IO::JoinPath( appDataDir, "Fabric" , "Exts" ) );
      }
#endif

      CG::CompileOptions compileOptions;
      compileOptions.setGuarded( false );

      RC::Handle<IO::Manager> ioManager = IOManager::Create();
      RC::Handle<DG::Context> dgContext = DG::Context::Create( ioManager, pluginPaths, compileOptions, true );
#if defined(FABRIC_MODULE_OPENCL)
      OCL::registerTypes( dgContext->getRTManager() );
#endif

      Plug::Manager::Instance()->loadBuiltInPlugins( pluginPaths, dgContext->getCGManager(), DG::Context::GetCallbackStruct() );
      
      m_client = Client::Create( dgContext, this );

      m_mainThreadTLS = true;
      uv_async_init( uv_default_loop(), &m_uvAsync, &ClientWrap::AsyncCallback );
      SetHandle( (uv_handle_t *)&m_uvAsync );
      
      //// uv_timer_init adds a loop reference. (That is, it calls uv_ref.) This
      //// is not the behavior we want in Node. Timers should not increase the
      //// ref count of the loop except when active.
      //uv_unref( uv_default_loop() );
    }

    ClientWrap::~ClientWrap()
    {
      //if (!active_)
      //  uv_ref( uv_default_loop() );

      m_notifyCallback.Dispose();
    }
    
    void ClientWrap::AsyncCallback( uv_async_t *async, int status )
    {
      ClientWrap *clientWrap = static_cast<ClientWrap *>( async->data );
      Util::Mutex::Lock lock( clientWrap->m_mutex );
      for ( std::vector<std::string>::const_iterator it=clientWrap->m_bufferedNotifications.begin(); it!=clientWrap->m_bufferedNotifications.end(); ++it )
        clientWrap->notify( it->data(), it->length() );
      clientWrap->m_bufferedNotifications.clear();
    }
    
#define UNWRAP \
  FABRIC_ASSERT( !args.Holder().IsEmpty() ); \
  FABRIC_ASSERT( args.Holder()->InternalFieldCount() > 0 ); \
  ClientWrap* wrap = static_cast<ClientWrap *>( args.Holder()->GetPointerFromInternalField(0) ); \
  if ( !wrap ) \
    return v8::ThrowException( v8::String::New( "client has already been closed" ) );

    v8::Handle<v8::Value> ClientWrap::JSONExec( v8::Arguments const &args )
    {
      UNWRAP
      
      if ( args.Length() != 1 || !args[0]->IsString() )
        return v8::ThrowException( v8::String::New( "jsonExec: takes one string parameter (jsonEncodedCommands)" ) );
      
      v8::HandleScope v8HandleScope;
      v8::Handle<v8::String> v8JSONEncodedCommands = v8::Handle<v8::String>::Cast( args[0] );
      v8::String::Utf8Value v8JSONEncodedCommandsUtf8Value( v8JSONEncodedCommands );
      Util::SimpleString jsonEncodedResults;
      {
        v8::Handle<v8::Object> v8This = args.This();
        Util::JSONGenerator resultJSON( &jsonEncodedResults );
        wrap->m_client->jsonExec(
          const_cast<char const *>(*v8JSONEncodedCommandsUtf8Value),
          size_t(v8JSONEncodedCommandsUtf8Value.length()),
          resultJSON
          );
      }
      v8::Handle<v8::String> v8JSONEncodedResults = v8::String::New( jsonEncodedResults.data(), jsonEncodedResults.length() );
      return v8HandleScope.Close( v8JSONEncodedResults );
    }
      
    v8::Handle<v8::Value> ClientWrap::SetJSONNotifyCallback( v8::Arguments const &args )
    {
      UNWRAP
      
      if ( args.Length() != 1 || !args[0]->IsFunction() )
        return v8::ThrowException( v8::String::New( "setJSONNotifyCallback: takes 1 function parameter (notificationCallback)" ) );
      v8::Handle<v8::Function> v8Callback = v8::Handle<v8::Function>::Cast( args[0] );

      v8::HandleScope v8HandleScope;
      wrap->m_notifyCallback = v8::Persistent<v8::Function>::New( v8Callback );
      wrap->m_client->notifyInitialState();
      return v8HandleScope.Close( v8::Handle<v8::Value>() );
    }
    
    v8::Handle<v8::Value> ClientWrap::Close( v8::Arguments const &args )
    {
      UNWRAP
      node::HandleWrap::Close( args );
      if ( wrap->m_client )
      {
        wrap->m_client->invalidate();
        wrap->m_client = 0;
      }
      return v8::Handle<v8::Value>();
    }
    
    void ClientWrap::notify( char const *data, size_t length )
    {
      Util::Mutex::Lock lock(m_mutex);
      if ( m_mainThreadTLS )
      {
        v8::HandleScope v8HandleScope;
        v8::Handle<v8::Value> jsonEncodedNotificationsV8String = v8::String::New( data, length );
        m_notifyCallback->Call( m_notifyCallback, 1, &jsonEncodedNotificationsV8String );
      }
      else
      {
        m_bufferedNotifications.push_back( std::string( data, length ) );
        uv_async_send( &m_uvAsync );
      }
    }
  };
};
