/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_V8_CLIENT_H
#define _FABRIC_V8_CLIENT_H

#include <Fabric/Core/DG/Client.h>
#include <v8/v8.h>

namespace Fabric
{
  namespace V8Ext
  {
    class Client : public DG::Client
    {
    public:
    
      static RC::Handle<Client> Create( RC::Handle<DG::Context> const &context );
      
      virtual void notify( std::string const &jsonEncodedNotifications ) const;
      
      v8::Handle<v8::Value> v8JSONExec( v8::Arguments const &args );
      static v8::Handle<v8::Value> V8JSONExec( v8::Arguments const &args );
      
      v8::Handle<v8::Value> v8SetJSONNotifyCallback( v8::Arguments const &args );
      static v8::Handle<v8::Value> V8SetJSONNotifyCallback( v8::Arguments const &args );
      
    protected:
    
      Client( RC::Handle<DG::Context> const &context );
      ~Client();
      
    private:
    
      v8::Persistent<v8::Function> m_v8Callback;
    };
  };
};

#endif //_FABRIC_V8_CLIENT_H
