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
      //Ensure that FabricResource is structured as expected
      FABRIC_ASSERT( fabricResourceDesc && RT::isStruct( fabricResourceDesc->getType() ) );
      RC::ConstHandle<RT::StructDesc> fabricResourceStructDesc = RC::ConstHandle<RT::StructDesc>::StaticCast( fabricResourceDesc );
      FABRIC_ASSERT( fabricResourceStructDesc->getMemberInfo( FABRIC_RESOURCE_DATA_MEMBER_INDEX ).name == "data" );
      FABRIC_ASSERT( fabricResourceStructDesc->getMemberInfo( FABRIC_RESOURCE_DATASIZE_MEMBER_INDEX ).name == "dataSize" );
      FABRIC_ASSERT( fabricResourceStructDesc->getMemberInfo( FABRIC_RESOURCE_MIMETYPE_MEMBER_INDEX ).name == "mimeType" );
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
      //Important: stream-base task must be run in main thread only
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
        if( !m_stream )//Loading has finished
        {
          //The resource member might have been modified by some operators; set it back if it is the case
          setData( &m_streamMimeType, m_streamData.empty() ? NULL : &(m_streamData[0]), m_streamData.size(), 0, false );
        }
      }
      else
      {
        //Note: we use a generation because if there was a previous stream created for a previous URL we cannot destroy it; 
        //we create a new one in parallel instead of waiting its completion.
        ++m_streamGeneration;
        m_streamURL = stringDesc->getValueData( urlMember );
        m_streamData.swap( std::vector<uint8_t>() );
        m_streamMimeType.clear();

        if( m_streamURL.empty() )
        {
          //Empty URL: free the resource
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

        dataDesc->setData( NULL, fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_DATA_MEMBER_INDEX ) );
        sizeDesc->setValue( 0, fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_DATASIZE_MEMBER_INDEX ) );
        stringDesc->setValue( NULL, 0, fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_MIMETYPE_MEMBER_INDEX ) );
      }
      else
      {
        //Important: only set if different
        bool changed = false;
        const void *currentResourceData = getConstData( "resource", 0 );

        if( *(const void **)fabricResourceStructDesc->getMemberData( currentResourceData, FABRIC_RESOURCE_DATA_MEMBER_INDEX ) != data )
          changed = true;
        else if( sizeDesc->getValue( fabricResourceStructDesc->getMemberData( currentResourceData, FABRIC_RESOURCE_DATASIZE_MEMBER_INDEX ) ) != dataSize )
          changed = true;
        else
        {
          const void* mimeTypeData = fabricResourceStructDesc->getMemberData( currentResourceData, FABRIC_RESOURCE_MIMETYPE_MEMBER_INDEX );
          if( stringDesc->getValueLength( mimeTypeData ) != mimeType->length() || memcmp( mimeType->data(), stringDesc->getValueData( mimeTypeData ), mimeType->length() ) != 0 )
            changed = true;
        }

        if( changed )
        {
          void* resourceData = getMutableData( "resource", 0 );
          dataDesc->setData( &data, fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_DATA_MEMBER_INDEX ) );
          sizeDesc->setValue( dataSize, fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_DATASIZE_MEMBER_INDEX ) );
          stringDesc->setValue( mimeType->c_str(), mimeType->size(), fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_MIMETYPE_MEMBER_INDEX ) );

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
