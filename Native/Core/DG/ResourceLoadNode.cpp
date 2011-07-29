/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/DG/ResourceLoadNode.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Core/RT/OpaqueDesc.h>
#include <Fabric/Core/RT/SizeDesc.h>
#include <Fabric/Core/RT/StringDesc.h>
#include <Fabric/Core/RT/StructDesc.h>
#include <Fabric/Core/RT/ImplType.h>
#include <Fabric/Base/JSON/Value.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace DG
  {
    enum FabricResourceMemberIndices
    {
      FABRIC_RESOURCE_DATA_MEMBER_INDEX,
      FABRIC_RESOURCE_DATASIZE_MEMBER_INDEX,
      FABRIC_RESOURCE_MIMETYPE_MEMBER_INDEX,
      FABRIC_RESOURCE_EXTENSION_MEMBER_INDEX,
    };

    RC::Handle<ResourceLoadNode> ResourceLoadNode::Create( std::string const &name, RC::Handle<Context> const &context )
    {
      RC::Handle<ResourceLoadNode> node = new ResourceLoadNode( name, context );
      node->notifyDelta( node->jsonDesc() );
      return node;
    }
    
    ResourceLoadNode::ResourceLoadNode( std::string const &name, RC::Handle<Context> const &context )
      : Node( name, context )
      , m_streamGeneration( 0 )
    {
      RC::ConstHandle<RT::StringDesc> stringDesc = context->getRTManager()->getStringDesc();
      RC::ConstHandle<RT::Desc> fabricResourceDesc = context->getRTManager()->getDesc("FabricResource");

#if defined(FABRIC_BUILD_DEBUG)
      FABRIC_ASSERT( fabricResourceDesc && RT::isStruct( fabricResourceDesc->getType() ) );
      RC::ConstHandle<RT::StructDesc> fabricResourceStructDesc = RC::ConstHandle<RT::StructDesc>::StaticCast( fabricResourceDesc );
      FABRIC_ASSERT( fabricResourceStructDesc->getMemberInfo( FABRIC_RESOURCE_DATA_MEMBER_INDEX ).name == "data" );
      FABRIC_ASSERT( fabricResourceStructDesc->getMemberInfo( FABRIC_RESOURCE_DATASIZE_MEMBER_INDEX ).name == "dataSize" );
      FABRIC_ASSERT( fabricResourceStructDesc->getMemberInfo( FABRIC_RESOURCE_MIMETYPE_MEMBER_INDEX ).name == "mimeType" );
      FABRIC_ASSERT( fabricResourceStructDesc->getMemberInfo( FABRIC_RESOURCE_EXTENSION_MEMBER_INDEX ).name == "extension" );
#endif

      addMember( "url", stringDesc, stringDesc->getDefaultData() );
      addMember( "resource", fabricResourceDesc, fabricResourceDesc->getDefaultData() );
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
      void const *urlMember = getConstData( "url", 0 );
      RC::ConstHandle<RT::StringDesc> stringDesc = getContext()->getRTManager()->getStringDesc();
      bool sameURL = stringDesc->getValueLength( urlMember ) == m_streamURL.size() && memcmp( m_streamURL.c_str(), stringDesc->getValueData( urlMember ), m_streamURL.size() ) == 0;

      if( sameURL )
      {
        bool loadingFinished = !m_stream;
        if( loadingFinished )
        {
          // [JeromeCG 20110727] The resource member might have been modified by some operators; set it back if it is the case
          setData( &m_streamMimeType, m_streamData.empty() ? NULL : &(m_streamData[0]), m_streamData.size(), 0, false );
        }
      }
      else
      {
        // [JeromeCG 20110727] Note: we use a generation because if there was a previous stream created for a previous URL we cannot destroy it; 
        // we create a new one in parallel instead of waiting its completion.
        ++m_streamGeneration;
        m_streamURL = stringDesc->getValueData( urlMember );
        std::vector<uint8_t> freeMe;
        m_streamData.swap( freeMe );
        m_streamMimeType.clear();

        if( m_streamURL.empty() )
        {
          setData( &m_streamMimeType, NULL, 0, 0, false );
        }
        else
        {
          m_stream = getContext()->getIOManager()->createStream(
            m_streamURL,
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
        if ( offset + size > m_streamData.size() )
        {
          m_streamData.resize( offset + size, 0 );
        }
        memcpy( &m_streamData[offset], data, size );
      }
    }

    void ResourceLoadNode::streamEnd( std::string const &url, std::string const &mimeType, void *userData )
    {
      if( (size_t)userData != m_streamGeneration )
        return;

      m_streamMimeType = mimeType;
      setData( &m_streamMimeType, m_streamData.empty() ? NULL : &(m_streamData[0]), m_streamData.size(), 0, true );
      m_stream = NULL;
    }

    void ResourceLoadNode::streamFailure( std::string const &url, std::string const &errorDesc, void *userData )
    {
      if( (size_t)userData != m_streamGeneration )
        return;

      setData( 0, 0, 0, &errorDesc, true );
    }

    void ResourceLoadNode::setData(
      std::string const *mimeType,
      void *data,
      size_t dataSize,
      std::string const *errorDesc,
      bool notify
      )
    {
      RC::ConstHandle<RT::Desc> fabricResourceDesc = getContext()->getRTManager()->getDesc("FabricResource");
      RC::ConstHandle<RT::StructDesc> fabricResourceStructDesc = RC::ConstHandle<RT::StructDesc>::StaticCast( fabricResourceDesc );

      RC::ConstHandle<RT::SizeDesc> sizeDesc = getContext()->getRTManager()->getSizeDesc();
      RC::ConstHandle<RT::StringDesc> stringDesc = getContext()->getRTManager()->getStringDesc();
      RC::ConstHandle<RT::OpaqueDesc> dataDesc = getContext()->getRTManager()->getDataDesc();

      if( errorDesc )
      {
        void *resourceData = getMutableData( "resource", 0 );
        void* nullPtr = NULL;
        dataDesc->setData( &nullPtr, fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_DATA_MEMBER_INDEX ) );
        sizeDesc->setValue( 0, fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_DATASIZE_MEMBER_INDEX ) );
        stringDesc->setValue( NULL, 0, fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_MIMETYPE_MEMBER_INDEX ) );
        stringDesc->setValue( NULL, 0, fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_EXTENSION_MEMBER_INDEX ) );
      }
      else
      {
        std::string extension;
        size_t extensionPos = m_streamURL.rfind('.');
        if( extensionPos != std::string::npos )
          extension = m_streamURL.substr( extensionPos+1 );
        else
        {
          extensionPos = mimeType->rfind('/');
          if( extensionPos != std::string::npos )
            extension = mimeType->substr( extensionPos+1 );
        }

        bool changed = false;
        const void *prevResourceData = getConstData( "resource", 0 );

        const void* prevData = fabricResourceStructDesc->getMemberData( prevResourceData, FABRIC_RESOURCE_DATA_MEMBER_INDEX );
        const void* prevDataSize = fabricResourceStructDesc->getMemberData( prevResourceData, FABRIC_RESOURCE_DATASIZE_MEMBER_INDEX );
        const void* prevMimeTypeData = fabricResourceStructDesc->getMemberData( prevResourceData, FABRIC_RESOURCE_MIMETYPE_MEMBER_INDEX );
        const void* prevExtensionData = fabricResourceStructDesc->getMemberData( prevResourceData, FABRIC_RESOURCE_EXTENSION_MEMBER_INDEX );

        if( *(const void **)prevData != data )
          changed = true;
        else if( sizeDesc->getValue( prevDataSize ) != dataSize )
          changed = true;
        else if ( stringDesc->getValueLength( prevMimeTypeData ) != mimeType->length() || memcmp( mimeType->data(), stringDesc->getValueData( prevMimeTypeData ), mimeType->length() ) != 0 )
          changed = true;
        else if ( stringDesc->getValueLength( prevExtensionData ) != extension.length() || memcmp( extension.data(), stringDesc->getValueData( prevExtensionData ), extension.length() ) != 0 )
          changed = true;

        if( changed )
        {
          void* resourceData = getMutableData( "resource", 0 );
          dataDesc->setData( &data, fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_DATA_MEMBER_INDEX ) );
          sizeDesc->setValue( dataSize, fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_DATASIZE_MEMBER_INDEX ) );
          stringDesc->setValue( mimeType->data(), mimeType->length(), fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_MIMETYPE_MEMBER_INDEX ) );
          stringDesc->setValue( extension.data(), extension.length(), fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_EXTENSION_MEMBER_INDEX ) );

          if( notify )
          {
            std::vector<std::string> src;
            src.push_back( "DG" );
            src.push_back( getName() );

            getContext()->jsonNotify( src, "resourceLoaded", RC::ConstHandle<JSON::Value>() );
          }
        }
      }
    }
  };
};
