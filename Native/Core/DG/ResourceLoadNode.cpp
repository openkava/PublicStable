/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/DG/ResourceLoadNode.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Core/RT/StructDesc.h>
#include <Fabric/Core/RT/StringDesc.h>
#include <Fabric/Core/RT/ImplType.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Base/JSON/Value.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace DG
  {
    RC::Handle<ResourceLoadNode> ResourceLoadNode::Create( std::string const &name, RC::Handle<Context> const &context )
    {
      RC::Handle<ResourceLoadNode> node = new ResourceLoadNode( name, context );
      node->notifyDelta( node->jsonDesc() );
      return node;
    }
    
    ResourceLoadNode::ResourceLoadNode( std::string const &name, RC::Handle<Context> const &context )
      : Node( name, context )
      , m_streamGeneration( 0 )
      , m_fabricResourceStreamData( context->getRTManager() )
    {
      RC::ConstHandle<RT::StringDesc> stringDesc = context->getRTManager()->getStringDesc();

      addMember( "url", stringDesc, stringDesc->getDefaultData() );
      addMember( "resource", m_fabricResourceStreamData.getDesc(), m_fabricResourceStreamData.getDesc()->getDefaultData() );
    }

    void ResourceLoadNode::jsonExecCreate( RC::ConstHandle<JSON::Value> const &arg, RC::Handle<Context> const &context )
    {
      Create( arg->toString()->value(), context );
    }

    void ResourceLoadNode::evaluateLocal( void *userdata )
    {
      // [JeromeCG 20110727]Important: Url streaming task must be run in main thread only since it might use some thread-sensitive APIs such as NPAPI's stream interface
      MT::executeParallel( 1, &ResourceLoadNode::EvaluateResource, (void *)this, true );
      Node::evaluateLocal( userdata );
    }

    void ResourceLoadNode::evaluateResource()
    {
      void const *urlMemberData = getConstData( "url", 0 );
      bool sameURL = m_fabricResourceStreamData.isURLEqualTo( urlMemberData );

      if( sameURL )
      {
        bool loadingFinished = !m_stream;
        if( loadingFinished )
        {
          // [JeromeCG 20110727] The resource member might have been modified by some operators; set it back if it is the case
          setResourceData( 0, false );
        }
      }
      else
      {
        // [JeromeCG 20110727] Note: we use a generation because if there was a previous stream created for a previous URL we cannot destroy it; 
        // we create a new one in parallel instead of waiting its completion.
        ++m_streamGeneration;
        m_fabricResourceStreamData.setURL( urlMemberData );
        m_fabricResourceStreamData.resizeData( 0 );
        m_fabricResourceStreamData.setMIMEType( "" );

        std::string url = m_fabricResourceStreamData.getURL();
        if( url.empty() )
        {
          setResourceData( 0, false );
        }
        else
        {
          m_stream = getContext()->getIOManager()->createStream(
            url,
            &ResourceLoadNode::StreamData,
            &ResourceLoadNode::StreamEnd,
            &ResourceLoadNode::StreamFailure,
            this,
            (void*)m_streamGeneration
            );
        }
      }
    }

    void ResourceLoadNode::streamData( std::string const &url, std::string const &mimeType, size_t offset, size_t size, void const *data, void *userData )
    {
      if( (size_t)userData != m_streamGeneration )
        return;

      if( size )
      {
        size_t prevSize = m_fabricResourceStreamData.getDataSize();
        if ( offset + size > prevSize )
          m_fabricResourceStreamData.resizeData( offset + size );
        m_fabricResourceStreamData.setData( offset, size, data );
      }
    }

    void ResourceLoadNode::streamEnd( std::string const &url, std::string const &mimeType, void *userData )
    {
      if( (size_t)userData != m_streamGeneration )
        return;

      m_fabricResourceStreamData.setMIMEType( mimeType );
      setResourceData( 0, true );
      m_stream = NULL;
    }

    void ResourceLoadNode::streamFailure( std::string const &url, std::string const &errorDesc, void *userData )
    {
      if( (size_t)userData != m_streamGeneration )
        return;

      m_fabricResourceStreamData.resizeData( 0 );
      setResourceData( &errorDesc, true );
    }

    void ResourceLoadNode::setResourceData(
      std::string const *errorDesc,
      bool notify
      )
    {
      std::string extension;
      std::string url = m_fabricResourceStreamData.getURL();

      size_t extensionPos = url.rfind('.');
      if( extensionPos != std::string::npos )
        extension = url.substr( extensionPos+1 );
      else
      {
        std::string mimeType = m_fabricResourceStreamData.getMIMEType();
        extensionPos = mimeType.rfind('/');
        if( extensionPos != std::string::npos )
          extension = mimeType.substr( extensionPos+1 );
      }
      m_fabricResourceStreamData.setExtension( extension );

      if( !m_fabricResourceStreamData.isEqualTo( getConstData( "resource", 0 ) ) )
      {
        void *resourceDataMember = getMutableData( "resource", 0 );
        m_fabricResourceStreamData.getDesc()->setData( m_fabricResourceStreamData.get(), resourceDataMember );

        if( errorDesc )
        {
          if ( getContext()->getLogCollector() )
          {
            getContext()->getLogCollector()->add( ( "ResourceLoadNode " + getName() + ": error loading " + url + ": " + *errorDesc ).c_str() );
          }
        }
        if( notify )
        {
          std::vector<std::string> src;
          src.push_back( "DG" );
          src.push_back( getName() );

          getContext()->jsonNotify( src, "resourceLoaded", RC::ConstHandle<JSON::Value>() );
        }
      }
    }
  };
};
