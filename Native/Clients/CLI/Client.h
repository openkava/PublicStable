/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_V8_CLIENT_H
#define _FABRIC_V8_CLIENT_H

#include <Fabric/Core/DG/Client.h>

#include <v8.h>
#include <uv.h>
#include <handle_wrap.h>

namespace Fabric
{
  namespace CLI
  {
    class Client : public DG::Client
    {
    public:
    
      static RC::Handle<Client> Create( RC::Handle<DG::Context> const &context, v8::Handle<v8::Object> clientWrap );
      
      virtual void notify( Util::SimpleString const &jsonEncodedNotifications ) const;
      void notifyInitialState() const
      {
        DG::Client::notifyInitialState();
      }
      
    protected:
    
      Client( RC::Handle<DG::Context> const &context, v8::Handle<v8::Object> clientWrap );
      ~Client();
      
    private:
    
      v8::Persistent<v8::Object> m_clientWrap;
    };
    
    class ClientWrap : public node::HandleWrap
    {
    public:
    
      static void Initialize( v8::Handle<v8::Object> target );
      
    protected:
    
      ClientWrap( v8::Handle<v8::Object> target );
      ~ClientWrap();
      
      static v8::Handle<v8::Value> New( v8::Arguments const &args );
      static v8::Handle<v8::Value> JSONExec( v8::Arguments const &args );
      static v8::Handle<v8::Value> SetJSONNotifyCallback( v8::Arguments const &args );
      static v8::Handle<v8::Value> Dispose( v8::Arguments const &args );
    
    private:
    
      RC::Handle<Client> m_client;
      uv_timer_t m_uvHandle;
    };
  };
};

#endif //_FABRIC_V8_CLIENT_H
