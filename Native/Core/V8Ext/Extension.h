/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_V8_EXTENSION_H
#define _FABRIC_V8_EXTENSION_H

#include <v8/v8.h>
#include <Fabric/Base/RC/Handle.h>
#include <string>
#include <vector>

namespace Fabric
{
  namespace DG
  {
    class Context;
  };
  
  namespace V8Ext
  {
    class Extension : public v8::Extension
    {
    public:
    
      Extension( std::string const &pluginsDir );
      ~Extension();
    
      virtual v8::Handle<v8::FunctionTemplate> GetNativeFunction( v8::Handle<v8::String> name );
      
    protected:
    
      v8::Handle<v8::Value> createFabricClient( v8::Arguments const &args ) const;
      static v8::Handle<v8::Value> CreateFabricClient( v8::Arguments const &args );

      static v8::Handle<v8::Value> Exec( v8::Arguments const &args );

      static void ClientWeakReferenceCallback( v8::Persistent<v8::Value> value, void * );
    
    private:
    
      std::vector<std::string> m_pluginDirs;
      
      v8::Persistent<v8::String> m_v8CreateFabricClientFunctionName;
      v8::Persistent<v8::FunctionTemplate> m_v8CreateFabricClientFunctionTemplate;
      
      v8::Persistent<v8::ObjectTemplate> m_v8ClientObjectTemplate;
    };
  };
};

#endif //_FABRIC_V8_EXTENSION_H
