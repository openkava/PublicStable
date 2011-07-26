/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/DG/ResourceLoadEvent.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Core/IO/Stream.h>
#include <Fabric/Core/RT/SizeDesc.h>
#include <Fabric/Core/RT/StringDesc.h>
#include <Fabric/Core/RT/OpaqueDesc.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Core/Util/Hex.h>

namespace Fabric
{
  

  namespace DG
  {
    RC::Handle<ResourceLoadEvent> ResourceLoadEvent::Create( std::string const &name, std::string const &url, RC::Handle<Context> const &context )
    {
      return new ResourceLoadEvent( name, url, context );
    }
    
    ResourceLoadEvent::ResourceLoadEvent( std::string const &name, std::string const &url, RC::Handle<Context> const &context )
      : Event( name, context )
      , m_context( context.ptr() )
      , m_url( url )
      , m_namedScope( "resource", this )
    {
      RC::ConstHandle<RT::SizeDesc> sizeDesc = m_context->getRTManager()->getSizeDesc();
      RC::ConstHandle<RT::StringDesc> stringDesc = m_context->getRTManager()->getStringDesc();
      RC::ConstHandle<RT::OpaqueDesc> dataDesc = m_context->getRTManager()->getDataDesc();
      
      addMember( "url", stringDesc, stringDesc->getDefaultData() );
      addMember( "mimeType", stringDesc, stringDesc->getDefaultData() );
      addMember( "data", dataDesc, dataDesc->getDefaultData() );
      addMember( "dataSize", sizeDesc, sizeDesc->getDefaultData() );
      addMember( "errorDesc", stringDesc, stringDesc->getDefaultData() );
    }
    
    void ResourceLoadEvent::start()
    {
      if( m_stream.isNull() )
      {
        m_stream = m_context->getIOManager()->createStream(
          m_url,
          &ResourceLoadEvent::StreamData,
          &ResourceLoadEvent::StreamEnd,
          &ResourceLoadEvent::StreamFailure,
          this
          );
      }
    }
    
    void ResourceLoadEvent::streamData( std::string const &url, std::string const &mimeType, size_t offset, size_t size, void const *data )
    {
      if( size )
      {
        if ( offset + size > m_streamData.size() )
        {
          m_streamData.resize( offset + size, 0 );
        }
        memcpy( &m_streamData[offset], data, size );
      }
    }

    void ResourceLoadEvent::streamEnd( std::string const &url, std::string const &mimeType )
    {
      fire( url, &mimeType, m_streamData.empty() ? NULL : &(m_streamData[0]), m_streamData.size(), 0 );
      m_streamData.swap( std::vector<uint8_t>() );
    }

    void ResourceLoadEvent::streamFailure( std::string const &url, std::string const &errorDesc )
    {
      fire( url, 0, 0, 0, &errorDesc );
    }

    void ResourceLoadEvent::fire(
      std::string const &url,
      std::string const *mimeType,
      void *data,
      size_t dataSize,
      std::string const *errorDesc
      )
    {
      RC::ConstHandle<RT::SizeDesc> sizeDesc = m_context->getRTManager()->getSizeDesc();
      RC::ConstHandle<RT::StringDesc> stringDesc = m_context->getRTManager()->getStringDesc();
      RC::ConstHandle<RT::OpaqueDesc> dataDesc = m_context->getRTManager()->getDataDesc();
      
      void *urlData = getMutableData( "url", 0 );
      stringDesc->setValue( url.data(), url.length(), urlData );
      
      void *mimeTypeData = getMutableData( "mimeType", 0 );
      if ( mimeType )
        stringDesc->setValue( mimeType->data(), mimeType->length(), mimeTypeData );
      else stringDesc->setData( stringDesc->getDefaultData(), mimeTypeData );
      
      void *dataData = getMutableData( "data", 0 );
      dataDesc->setData( &data, dataData );
      
      void *dataSizeData = getMutableData( "dataSize", 0 );
      sizeDesc->setData( &dataSize, dataSizeData );
      
      void *errorDescData = getMutableData( "errorDesc", 0 );
      if ( errorDesc )
        stringDesc->setValue( errorDesc->data(), errorDesc->length(), errorDescData );
      else stringDesc->setData( stringDesc->getDefaultData(), errorDescData );
      
      Event::fire( &m_namedScope, 0, 0 );
    }
    
    void ResourceLoadEvent::collectErrors()
    {
      Event::collectErrors( &m_namedScope );
    }

    RC::ConstHandle<JSON::Value> ResourceLoadEvent::jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Value> result;

      if ( cmd == "start" )
        jsonExecStart();
      else result = Event::jsonExec( cmd, arg );
      
      return result;
    }

    void ResourceLoadEvent::jsonExecCreate( RC::ConstHandle<JSON::Value> const &arg, RC::Handle<Context> const &context )
    {
      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      
      std::string name;
      try
      {
        name = argJSONObject->get( "name" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "'name': " + e;
      }
      
      std::string url;
      try
      {
        url = argJSONObject->get( "url" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "'url': " + e;
      }
      
      Create( name, url, context );
    }
    
    void ResourceLoadEvent::jsonExecStart()
    {
      start();
    }
  };
};
