/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_DG_RESOURCE_LOAD_EVENT_H
#define _FABRIC_DG_RESOURCE_LOAD_EVENT_H

#include <Fabric/Core/DG/Event.h>
#include <Fabric/Core/DG/Scope.h>

namespace Fabric
{
  namespace IO
  {
    class Stream;
  };
  
  namespace DG
  {
    class ResourceLoadEvent : public Event
    {
    public:
    
      static RC::Handle<ResourceLoadEvent> Create( std::string const &name, std::string const &url, RC::Handle<Context> const &context );
      
      virtual bool isResourceLoadEvent() const { return true; }

      void start();
    
      virtual RC::ConstHandle<JSON::Value> jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      static void jsonExecCreate( RC::ConstHandle<JSON::Value> const &arg, RC::Handle<Context> const &context );
      void jsonExecStart();
      
    protected:
    
      ResourceLoadEvent( std::string const &name, std::string const &url, RC::Handle<Context> const &context );
      
      virtual void collectErrors();

      void streamSuccess( std::string const &url, std::string const &mimeType, std::string const &filename );
      static void StreamSuccess( std::string const &url, std::string const &mimeType, std::string const &filename, RC::Handle<RC::Object> const &target )
      {
        RC::Handle<ResourceLoadEvent>::StaticCast(target)->streamSuccess( url, mimeType, filename );
      }
      
      void streamFailure( std::string const &url, std::string const &errorDesc );
      static void StreamFailure( std::string const &url, std::string const &errorDesc, RC::Handle<RC::Object> const &target )
      {
        RC::Handle<ResourceLoadEvent>::StaticCast(target)->streamFailure( url, errorDesc );
      }
      
      void fire( std::string const &url, std::string const *mimeType, void *data, size_t dataSize, std::string const *errorDesc );
      
    private:
    
      RC::ConstHandle<Context> m_context;
      NamedScope m_namedScope;
      RC::Handle<IO::Stream> m_stream;
    };
  };
};

#endif //_FABRIC_DG_RESOURCE_LOAD_EVENT_H
