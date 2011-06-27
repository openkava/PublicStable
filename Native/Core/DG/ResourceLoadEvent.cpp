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
    
      m_stream = context->getIOManager()->createStream(
        url,
        &ResourceLoadEvent::StreamSuccess,
        &ResourceLoadEvent::StreamFailure,
        this
        );
    }
    
    void ResourceLoadEvent::start()
    {
      m_stream->start();
    }
    
    void ResourceLoadEvent::streamSuccess( std::string const &url, std::string const &mimeType, std::string const &filename )
    {
      FILE *fp = fopen( filename.c_str(), "rb" );
      if ( fp == NULL )
        throw Exception( "unable to open file "+_(filename) );
      
      size_t allocSize = 65536;
      uint8_t *data = static_cast<uint8_t *>( malloc(allocSize) );
      size_t dataSize = 0;
      for (;;)
      {
        static const size_t maxReadSize = 65536;
        if ( dataSize + maxReadSize > allocSize )
        {
          allocSize = dataSize + maxReadSize;
          data = static_cast<uint8_t *>( realloc( data, allocSize ) );
        }
        int readSize = fread( &data[dataSize], 1, maxReadSize, fp );
        dataSize += std::max<int>( readSize, 0 );
        if ( readSize < maxReadSize )
          break;
      }
      
      fclose( fp );
      
      fire( url, &mimeType, data, dataSize, 0 );
      
      free( data );
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
