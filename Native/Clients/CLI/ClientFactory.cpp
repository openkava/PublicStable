/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Clients/CLI/ClientFactory.h>
#include <Fabric/Clients/CLI/Client.h>
#include <Fabric/Clients/CLI/LogCollector.h>
#include <Fabric/Core/Build.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/RT/StringDesc.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/DG/Debug.h>
#include <Fabric/Core/RT/NumericDesc.h>
#include <Fabric/Core/RT/StructDesc.h>
#include <Fabric/Core/MT/IdleTaskQueue.h>
#include <Fabric/Core/IO/Helpers.h>
#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Core/IO/Stream.h>
#include <Fabric/Core/OCL/OCL.h>
#include <Fabric/Core/Plug/Manager.h>

#include <v8/v8.h>
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
    
    static void ClientV8ObjectWeakReferenceCallback( v8::Persistent<v8::Value> value, void * )
    {
      //FABRIC_LOG( "ClientV8ObjectWeakReferenceCallback()" );
      
      v8::Handle<v8::Object> v8ClientObject = v8::Handle<v8::Object>::Cast( value );
      
      Client *client = static_cast<Client *>( v8ClientObject->GetPointerFromInternalField( 0 ) );
      client->release();
      
      value.Dispose();
    }

    static v8::Handle<v8::Value> CreateClientV8FunctionCallback( v8::Arguments const &args )
    {
      //FABRIC_LOG( "CreateClientV8FunctionCallback()" );
      
      v8::HandleScope handleScope;
      
      v8::Handle<v8::String> jsonExecV8String = v8::String::New("jsonExec");
      v8::Handle<v8::String> setJSONNotifyCallbackV8String = v8::String::New("setJSONNotifyCallback");
      v8::Handle<v8::String> wrapFabricClientV8String = v8::String::New("wrapFabricClient");
      v8::Handle<v8::String> disposeV8String = v8::String::New("dispose");
                
      v8::Persistent<v8::ObjectTemplate> v8ClientObjectTemplate;
      if ( v8ClientObjectTemplate.IsEmpty() )
      {
        v8ClientObjectTemplate = v8::Persistent<v8::ObjectTemplate>( v8::ObjectTemplate::New() );
        v8ClientObjectTemplate->SetInternalFieldCount( 1 );
        v8ClientObjectTemplate->Set( jsonExecV8String, v8::FunctionTemplate::New( &Client::V8JSONExec ) );
        v8ClientObjectTemplate->Set( setJSONNotifyCallbackV8String, v8::FunctionTemplate::New( &Client::V8SetJSONNotifyCallback ) );
        v8ClientObjectTemplate->Set( wrapFabricClientV8String, v8::FunctionTemplate::New( &Client::V8WrapFabricClient ) );
        v8ClientObjectTemplate->Set( disposeV8String, v8::FunctionTemplate::New( &Client::V8Dispose ) );
      }
      
      RC::Handle<IO::Manager> ioManager = IOManager::Create();
      std::vector<std::string> pluginDirs;
      RC::Handle<DG::Context> dgContext = DG::Context::Create( ioManager, pluginDirs );
      OCL::registerTypes( dgContext->getRTManager() );

      RC::Handle<Client> client = Client::Create( dgContext );
      Plug::Manager::Instance()->loadBuiltInPlugins( pluginDirs, dgContext->getCGManager() );

      v8::Persistent<v8::Object> v8ClientObject = v8::Persistent<v8::Object>::New( v8ClientObjectTemplate->NewInstance() );
      v8ClientObject->SetPointerInInternalField( 0, client.take() );
      v8ClientObject.MakeWeak( 0, &ClientV8ObjectWeakReferenceCallback );
      
      v8::TryCatch try_catch;
      std::string const &wrapFabricClientJSSource = DG::Context::GetWrapFabricClientJSSource();
      v8::Handle<v8::String> wrapFabricClientJSSourceV8String = v8::String::New( wrapFabricClientJSSource.data(), wrapFabricClientJSSource.length() );
      v8::Handle<v8::Script> wrapFabricClientJSSourceV8Script = v8::Script::Compile( wrapFabricClientJSSourceV8String, v8::String::New("FABRIC.Wrappers.js") );
      FABRIC_ASSERT( !wrapFabricClientJSSourceV8Script.IsEmpty() );
      v8::Handle<v8::Value> wrapFabricClientV8Value = wrapFabricClientJSSourceV8Script->Run();
      FABRIC_ASSERT( !wrapFabricClientV8Value.IsEmpty() );
      FABRIC_ASSERT( wrapFabricClientV8Value->IsFunction() );
      v8::Handle<v8::Function> wrapFabricClientV8Function = v8::Handle<v8::Function>::Cast( wrapFabricClientV8Value );
      std::vector< v8::Handle<v8::Value> > argv;
      argv.push_back( v8ClientObject );
      v8::Handle<v8::Value> result = wrapFabricClientV8Function->Call( args.This(), argv.size(), &argv[0] );
      return handleScope.Close( result );
    }
    
    v8::Handle<v8::Value> CreateClientV8Function()
    {
      //FABRIC_LOG( "CreateClientV8Function()" );
      
      v8::HandleScope handleScope;
      
      v8::Persistent<v8::FunctionTemplate> createClientV8FunctionTemplate;
      if ( createClientV8FunctionTemplate.IsEmpty() )
      {
        createClientV8FunctionTemplate = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( &CreateClientV8FunctionCallback ) );
      }
      
      return handleScope.Close( createClientV8FunctionTemplate->GetFunction() );
    }
  };
};
