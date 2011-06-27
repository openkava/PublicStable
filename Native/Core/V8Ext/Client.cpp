/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/V8Ext/Client.h>

#include <string>

namespace Fabric
{
  namespace V8Ext
  {
    RC::Handle<Client> Client::Create( RC::Handle<DG::Context> const &context )
    {
      return new Client( context );
    }
    
    Client::Client( RC::Handle<DG::Context> const &context )
      : DG::Client( context )
    {
    }

    Client::~Client()
    {
      m_v8Callback.Dispose();
    }
    
    void Client::notify( std::string const &jsonEncodedNotifications ) const
    {
      if ( !m_v8Callback.IsEmpty() )
      {
        v8::HandleScope v8HandleScope;
        v8::Handle<v8::Value> jsonEncodedNotificationsV8String = v8::String::New( jsonEncodedNotifications.data(), jsonEncodedNotifications.length() );
        m_v8Callback->Call( m_v8Callback, 1, &jsonEncodedNotificationsV8String );
      }
    }
    
    v8::Handle<v8::Value> Client::v8JSONExec( v8::Arguments const &args )
    {
      v8::HandleScope v8HandleScope;
      if ( args.Length() != 1 || !args[0]->IsString() )
        return v8::ThrowException( v8::String::New( "v8JSONExec: takes one string parameter (jsonEncodedCommands)" ) );
      v8::Handle<v8::String> v8JSONEncodedCommands = v8::Handle<v8::String>::Cast( args[0] );
      v8::String::Utf8Value v8JSONEncodedCommandsUtf8Value( v8JSONEncodedCommands );
      std::string jsonEncodedResults = jsonExec( *v8JSONEncodedCommandsUtf8Value, v8JSONEncodedCommandsUtf8Value.length() );
      v8::Handle<v8::String> v8JSONEncodedResults = v8::String::New( jsonEncodedResults.data(), jsonEncodedResults.length() );
      return v8HandleScope.Close( v8JSONEncodedResults );
    }
    
    v8::Handle<v8::Value> Client::V8JSONExec( v8::Arguments const &args )
    {
      return static_cast<Client *>( args.This()->GetPointerFromInternalField( 0 ) )->v8JSONExec( args );
    }
      
    v8::Handle<v8::Value> Client::v8SetJSONNotifyCallback( v8::Arguments const &args )
    {
      if ( args.Length() != 1 || !args[0]->IsFunction() )
        return v8::ThrowException( v8::String::New( "takes 1 function parameter (notificationCallback)" ) );
      v8::Handle<v8::Function> v8Callback = v8::Handle<v8::Function>::Cast( args[0] );

      m_v8Callback.Dispose();
      m_v8Callback = v8::Persistent<v8::Function>::New( v8Callback );
      notifyInitialState();
      
      return v8::Handle<v8::Value>();
    }
    
    v8::Handle<v8::Value> Client::V8SetJSONNotifyCallback( v8::Arguments const &args )
    {
      return static_cast<Client *>( args.This()->GetPointerFromInternalField( 0 ) )->v8SetJSONNotifyCallback( args );
    }
    
    v8::Handle<v8::Value> Client::V8Dispose( v8::Arguments const &args )
    {
      Client *client = static_cast<Client *>( args.This()->GetPointerFromInternalField( 0 ) );
      args.This()->SetPointerInInternalField( 0, 0 );
      client->release();
    }
  };
};
