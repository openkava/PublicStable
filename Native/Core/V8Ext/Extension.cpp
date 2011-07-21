/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "Extension.h"
#include "Client.h"

#include <Fabric/Core/Build.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/IO/Stream.h>
#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Core/OCL/OCL.h>
#include <Fabric/Core/Plug/Manager.h>

namespace Fabric
{
  namespace V8Ext
  {
    class IOStream : public IO::Stream
    {
    public:
    
      static RC::Handle<IOStream> Create(
        std::string const &url,
        SuccessCallback successCallback,
        FailureCallback failureCallback,
        RC::Handle<RC::Object> const &target
        )
      {
        return new IOStream( url, successCallback, failureCallback, target );
      }

      virtual void start()
      {
        if ( !m_started )
        {
          m_started = true;
          size_t colonIndex = m_url.find( ':' );
          if ( colonIndex == m_url.length() )
            indicateFailure( m_url, "malformed URL" );
          else
          {
            std::string method = m_url.substr( 0, colonIndex );
            if ( method != "file" )
              indicateFailure( m_url, "unsupported method " + _(method) );
            else
            {
              std::string filename = m_url.substr( colonIndex+1, m_url.length() - colonIndex - 1 );
              if ( filename.length() == 0 )
                indicateFailure( m_url, "empty filename" );
              else
                indicateSuccess( m_url, "text/plain", filename );
            }
          }
        }
      }
      
    protected:
    
      IOStream(
        std::string const &url,
        SuccessCallback successCallback,
        FailureCallback failureCallback,
        RC::Handle<RC::Object> const &target
        )
        : IO::Stream( successCallback, failureCallback, target )
        , m_url( url )
        , m_started( false )
      {
      }
      
    private:
    
      std::string m_url;
      bool m_started;
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
        IO::Stream::SuccessCallback successCallback,
        IO::Stream::FailureCallback failureCallback,
        RC::Handle<RC::Object> const &target
        ) const
      {
        return IOStream::Create( url, successCallback, failureCallback, target );
      }
    
    protected:
    
      IOManager()
      {
      }
    };
    
    Extension::Extension( std::string const &pluginsDir )
      : v8::Extension( "Fabric", "native function createFabricClient();" )
    {
      v8::HandleScope handleScope;
      
      m_pluginDirs.push_back( pluginsDir );
      
      m_v8CreateFabricClientFunctionName = v8::Persistent<v8::String>::New( v8::String::New( "createFabricClient" ) );
      v8::Handle<v8::FunctionTemplate> v8CreateFabricClientFunctionTemplate = v8::FunctionTemplate::New( &Extension::CreateFabricClient, v8::External::Wrap(this) );
      m_v8CreateFabricClientFunctionTemplate = v8::Persistent<v8::FunctionTemplate>::New( v8CreateFabricClientFunctionTemplate );
      
      v8::Handle<v8::ObjectTemplate> v8ClientObjectTemplate = v8::ObjectTemplate::New();
      v8ClientObjectTemplate->SetInternalFieldCount( 1 );
      v8ClientObjectTemplate->Set( "jsonExec", v8::FunctionTemplate::New( &Client::V8JSONExec ) );
      v8ClientObjectTemplate->Set( "setJSONNotifyCallback", v8::FunctionTemplate::New( &Client::V8SetJSONNotifyCallback ) );
      v8ClientObjectTemplate->Set( "dispose", v8::FunctionTemplate::New( &Client::V8Dispose ) );
      m_v8ClientObjectTemplate = v8::Persistent<v8::ObjectTemplate>::New( v8ClientObjectTemplate );
    }
    
    Extension::~Extension()
    {
      m_v8CreateFabricClientFunctionTemplate.Dispose();
      m_v8CreateFabricClientFunctionName.Dispose();
      m_v8ClientObjectTemplate.Dispose();
    }
      
    v8::Handle<v8::FunctionTemplate> Extension::GetNativeFunction( v8::Handle<v8::String> name )
    {
      if ( name->Equals( m_v8CreateFabricClientFunctionName ) )
        return m_v8CreateFabricClientFunctionTemplate;
      else return v8::Extension::GetNativeFunction( name );
    }
    
    v8::Handle<v8::Value> Extension::createFabricClient( v8::Arguments const &args ) const
    {
      v8::HandleScope handleScope;
            
      RC::Handle<IO::Manager> ioManager = IOManager::Create();
      RC::Handle<DG::Context> dgContext = DG::Context::Create( ioManager, m_pluginDirs );
      OCL::registerTypes( dgContext->getRTManager() );

      Client *client = Client::Create( dgContext ).take();
      Plug::Manager::Instance()->loadBuiltInPlugins( m_pluginDirs );
      
      v8::Persistent<v8::Object> v8ClientObject = v8::Persistent<v8::Object>::New( m_v8ClientObjectTemplate->NewInstance() );
      v8ClientObject->SetPointerInInternalField( 0, client );
      v8ClientObject.MakeWeak( 0, &Extension::ClientWeakReferenceCallback );
      return handleScope.Close( v8ClientObject );
    }
    
    v8::Handle<v8::Value> Extension::CreateFabricClient( v8::Arguments const &args )
    {
      return static_cast<Extension const *>( v8::External::Unwrap( args.Data() ) )->createFabricClient( args );
    }

    void Extension::ClientWeakReferenceCallback( v8::Persistent<v8::Value> value, void * )
    {
      v8::Handle<v8::Object> v8ClientObject = v8::Handle<v8::Object>::Cast( value );
      
      Client *client = static_cast<Client *>( v8ClientObject->GetPointerFromInternalField( 0 ) );
      client->release();
      
      value.Dispose();
    }
  };
};
