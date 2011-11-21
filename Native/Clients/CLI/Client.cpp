/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Clients/CLI/Client.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Core/IO/Helpers.h>
#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Core/IO/Stream.h>
#include <Fabric/Core/Plug/Manager.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/DG/Context.h>

#include <node.h>
#include <string>

namespace Fabric
{
  namespace CLI
  {
    class IOStream : public IO::Stream
    {
    public:

      static RC::Handle<IOStream> Create(
        std::string const &url,
        DataCallback dataCallback,
        EndCallback endCallback,
        FailureCallback failureCallback,
        RC::Handle<RC::Object> const &target,
        void *userData
        )
      {
        return new IOStream( url, dataCallback, endCallback, failureCallback, target, userData );
      }
      
    protected:

      IOStream(
        std::string const &url,
        DataCallback dataCallback,
        EndCallback endCallback,
        FailureCallback failureCallback,
        RC::Handle<RC::Object> const &target,
        void *userData
        )
        : IO::Stream( dataCallback, endCallback, failureCallback, target, userData )
        , m_url( url )
      {
        size_t colonIndex = m_url.find( ':' );
        if ( colonIndex == m_url.length() )
          onFailure( m_url, "malformed URL" );
        else
        {
          std::string method = m_url.substr( 0, colonIndex );
          if ( method != "file" )
            onFailure( m_url, "unsupported method " + _(method) );
          else
          {
            std::string filename = m_url.substr( colonIndex+1, m_url.length() - colonIndex - 1 );
            if ( filename.length() == 0 )
              onFailure( m_url, "empty filename" );
            else
            {
              FILE *fp = fopen( filename.c_str(), "rb" );
              if ( fp == NULL )
                onFailure( m_url, "unable to open file" );

              fseek(fp, 0, SEEK_END);
              size_t totalSize = ftell(fp);
              fseek(fp, 0, SEEK_SET); 
      
              static const size_t maxReadSize = 1<<16;//64K buffers
              uint8_t *data = static_cast<uint8_t *>( malloc(maxReadSize) );
              size_t offset = 0;
              for (;;)
              {
                size_t readSize = fread( &data[0], 1, maxReadSize, fp );
                if ( ferror( fp ) )
                  onFailure( m_url, "error while reading file" );

                onData( m_url, "text/plain", totalSize, offset, readSize, data );

                if ( readSize < maxReadSize )
                  break;
                offset += readSize;
              }
              free( data );
              fclose( fp );

              onEnd( m_url, "text/plain" );
            }
          }
        }
      }
      
    private:

      std::string m_url;
    };

    class IOManager : public IO::Manager
    {
    public:
    
      static RC::Handle<IOManager> Create()
      {
        return new IOManager;
      }
    
      virtual RC::Handle<IO::Stream> createStream(
        std::string const &url,
        IO::Stream::DataCallback dataCallback,
        IO::Stream::EndCallback endCallback,
        IO::Stream::FailureCallback failureCallback,
        RC::Handle<RC::Object> const &target,
        void *userData
        ) const
      {
        return IOStream::Create( url, dataCallback, endCallback, failureCallback, target, userData );
      }

      virtual std::string queryUserFilePath(
        bool existingFile,
        std::string const &title,
        std::string const &defaultFilename,
        std::string const &extension
        ) const
      {
        if ( !IO::DirExists( "TMP" ) )
	        IO::CreateDir( "TMP" );
        if(existingFile)
          return IO::JoinPath("TMP", "default.txt");
        else
          return IO::JoinPath("TMP", defaultFilename);
      }
  
    protected:
    
      IOManager()
      {
      }
    };
    
    RC::Handle<Client> Client::Create( RC::Handle<DG::Context> const &context, v8::Handle<v8::Object> clientWrap )
    {
      return new Client( context, clientWrap );
    }
    
    Client::Client( RC::Handle<DG::Context> const &context, v8::Handle<v8::Object> clientWrap )
      : DG::Client( context )
    {
      m_clientWrap = v8::Persistent<v8::Object>::New( clientWrap );
    }
    
    Client::~Client()
    {
      m_clientWrap.Dispose();
    }
    
    void Client::notify( Util::SimpleString const &jsonEncodedNotifications ) const
    {
      v8::HandleScope v8HandleScope;
      v8::Handle<v8::Value> v8CallbackValue = m_clientWrap->Get( v8::String::New("notifyCallback") );
      if ( v8CallbackValue->IsFunction() )
      {
        v8::Handle<v8::Function> v8CallbackFunction = v8::Handle<v8::Function>::Cast( v8CallbackValue );
        v8::Handle<v8::Value> jsonEncodedNotificationsV8String = v8::String::New( jsonEncodedNotifications.data(), jsonEncodedNotifications.length() );
        v8CallbackFunction->Call( v8CallbackFunction, 1, &jsonEncodedNotificationsV8String );
      }
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
      NODE_SET_PROTOTYPE_METHOD( v8FunctionTemplate, "dispose", Dispose );
      
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
      : node::HandleWrap( object, (uv_handle_t *)&m_uvHandle )
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

      Plug::Manager::Instance()->loadBuiltInPlugins( pluginPaths, dgContext->getCGManager() );
      
      m_client = Client::Create( dgContext, object );

      uv_timer_init( uv_default_loop(), &m_uvHandle );
      m_uvHandle.data = this;

      //// uv_timer_init adds a loop reference. (That is, it calls uv_ref.) This
      //// is not the behavior we want in Node. Timers should not increase the
      //// ref count of the loop except when active.
      //uv_unref( uv_default_loop() );
    }

    ClientWrap::~ClientWrap()
    {
      //if (!active_)
      //  uv_ref( uv_default_loop() );
    }
    
#define UNWRAP \
  assert(!args.Holder().IsEmpty()); \
  assert(args.Holder()->InternalFieldCount() > 0); \
  ClientWrap* wrap =  \
      static_cast<ClientWrap*>(args.Holder()->GetPointerFromInternalField(0)); \
  if (!wrap) { \
    node::SetErrno(UV_EBADF); \
    return v8HandleScope.Close(v8::Integer::New(-1)); \
  }

    v8::Handle<v8::Value> ClientWrap::JSONExec( v8::Arguments const &args )
    {
      v8::HandleScope v8HandleScope;
      
      UNWRAP
      
      if ( args.Length() != 1 || !args[0]->IsString() )
        return v8::ThrowException( v8::String::New( "jsonExec: takes one string parameter (jsonEncodedCommands)" ) );
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
      v8::HandleScope v8HandleScope;
      
      UNWRAP
      
      if ( args.Length() != 1 || !args[0]->IsFunction() )
        return v8::ThrowException( v8::String::New( "setJSONNotifyCallback: takes 1 function parameter (notificationCallback)" ) );
      v8::Handle<v8::Function> v8Callback = v8::Handle<v8::Function>::Cast( args[0] );

      v8::Handle<v8::Object> v8This = args.This();
      v8This->Set( v8::String::New("notifyCallback"), v8Callback );

      wrap->m_client->notifyInitialState();
      
      return v8HandleScope.Close( v8::Handle<v8::Value>() );
    }
    
    v8::Handle<v8::Value> ClientWrap::Dispose( v8::Arguments const &args )
    {
      v8::HandleScope v8HandleScope;
      UNWRAP
      return v8HandleScope.Close( v8::Handle<v8::Value>() );
    }
  };
};
