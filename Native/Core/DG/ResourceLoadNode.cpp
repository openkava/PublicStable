/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/DG/ResourceLoadNode.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Core/RT/IntegerDesc.h>
#include <Fabric/Core/RT/OpaqueDesc.h>
#include <Fabric/Core/RT/StringDesc.h>
#include <Fabric/Core/RT/StructDesc.h>
#include <Fabric/Core/RT/ImplType.h>
#include <Fabric/Base/JSON/Value.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Core/RT/VariableArrayImpl.h>

namespace Fabric
{
  namespace DG
  {
    enum FabricResourceMemberIndices
    {
      FABRIC_RESOURCE_DATA_MEMBER_INDEX,
      FABRIC_RESOURCE_MIMETYPE_MEMBER_INDEX,
      FABRIC_RESOURCE_EXTENSION_MEMBER_INDEX,
      FABRIC_RESOURCE_URL_MEMBER_INDEX,
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
      FABRIC_ASSERT( fabricResourceStructDesc->getMemberInfo( FABRIC_RESOURCE_MIMETYPE_MEMBER_INDEX ).name == "mimeType" );
      FABRIC_ASSERT( fabricResourceStructDesc->getMemberInfo( FABRIC_RESOURCE_EXTENSION_MEMBER_INDEX ).name == "extension" );
      FABRIC_ASSERT( fabricResourceStructDesc->getMemberInfo( FABRIC_RESOURCE_URL_MEMBER_INDEX ).name == "url" );
#endif

      addMember( "url", stringDesc, stringDesc->getDefaultData() );
      addMember( "resource", fabricResourceDesc, fabricResourceDesc->getDefaultData() );

      m_byteVariableArrayDesc = context->getRTManager()->getVariableArrayOf( context->getRTManager()->getByteDesc() );
      size_t arraySize = m_byteVariableArrayDesc->getSize();
      m_byteVariableArrayStreamData = malloc( arraySize );
      memset( m_byteVariableArrayStreamData, 0, arraySize );
    }

    ResourceLoadNode::~ResourceLoadNode()
    {
      m_byteVariableArrayDesc->disposeData( m_byteVariableArrayStreamData );
      free( m_byteVariableArrayStreamData );
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
          setData( &m_streamMimeType, m_byteVariableArrayStreamData, 0, false );
        }
      }
      else
      {
        // [JeromeCG 20110727] Note: we use a generation because if there was a previous stream created for a previous URL we cannot destroy it; 
        // we create a new one in parallel instead of waiting its completion.
        ++m_streamGeneration;
        m_streamURL = stringDesc->getValueData( urlMember );

        m_byteVariableArrayDesc->setNumMembers( m_byteVariableArrayStreamData, 0, NULL );
        m_streamMimeType.clear();

        if( m_streamURL.empty() )
        {
          setData( &m_streamMimeType, m_byteVariableArrayStreamData, 0, false );
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
        size_t prevSize = m_byteVariableArrayDesc->getImpl()->getNumMembers( m_byteVariableArrayStreamData );
        if ( offset + size > prevSize )
        {
          m_byteVariableArrayDesc->setNumMembers( m_byteVariableArrayStreamData, offset + size, NULL );
        }
        m_byteVariableArrayDesc->getImpl()->setMembers( m_byteVariableArrayStreamData, offset, size, data );
      }
    }

    void ResourceLoadNode::streamEnd( std::string const &url, std::string const &mimeType, void *userData )
    {
      if( (size_t)userData != m_streamGeneration )
        return;

      m_streamMimeType = mimeType;
      setData( &m_streamMimeType, m_byteVariableArrayStreamData, 0, true );
      m_stream = NULL;
    }

    void ResourceLoadNode::streamFailure( std::string const &url, std::string const &errorDesc, void *userData )
    {
      if( (size_t)userData != m_streamGeneration )
        return;

      m_byteVariableArrayDesc->setNumMembers( m_byteVariableArrayStreamData, 0, NULL );
      setData( 0, m_byteVariableArrayStreamData, &errorDesc, true );
    }

    void ResourceLoadNode::setData(
      std::string const *mimeType,
      void *byteVariableArrayData,
      std::string const *errorDesc,
      bool notify
      )
    {
      RC::ConstHandle<RT::Desc> fabricResourceDesc = getContext()->getRTManager()->getDesc("FabricResource");
      RC::ConstHandle<RT::StructDesc> fabricResourceStructDesc = RC::ConstHandle<RT::StructDesc>::StaticCast( fabricResourceDesc );
      RC::ConstHandle<RT::SizeDesc> sizeDesc = getContext()->getRTManager()->getSizeDesc();
      RC::ConstHandle<RT::StringDesc> stringDesc = getContext()->getRTManager()->getStringDesc();

      if( errorDesc )
      {
        void *resourceData = getMutableData( "resource", 0 );
        m_byteVariableArrayDesc->setNumMembers( fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_DATA_MEMBER_INDEX ), 0 );
        stringDesc->setValue( NULL, 0, fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_MIMETYPE_MEMBER_INDEX ) );
        stringDesc->setValue( NULL, 0, fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_EXTENSION_MEMBER_INDEX ) );
        stringDesc->setValue( NULL, 0, fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_URL_MEMBER_INDEX ) );
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
        const void* prevMimeTypeData = fabricResourceStructDesc->getMemberData( prevResourceData, FABRIC_RESOURCE_MIMETYPE_MEMBER_INDEX );
        const void* prevExtensionData = fabricResourceStructDesc->getMemberData( prevResourceData, FABRIC_RESOURCE_EXTENSION_MEMBER_INDEX );
        const void* prevUrlData = fabricResourceStructDesc->getMemberData( prevResourceData, FABRIC_RESOURCE_URL_MEMBER_INDEX );

        // [JeromeCG 20110831] Question: can we access the .data() and .dataSize() like opaque adapters? Would be cleaner for memcmp...
        size_t prevDataSize = m_byteVariableArrayDesc->getImpl()->getNumMembers( prevData );
        size_t dataSize = m_byteVariableArrayDesc->getImpl()->getNumMembers( byteVariableArrayData );
        void const* prevDataPtr = prevDataSize == 0 ? NULL : m_byteVariableArrayDesc->getImpl()->getMemberData( prevData, 0 );
        void const* dataPtr = dataSize == 0 ? NULL : m_byteVariableArrayDesc->getImpl()->getMemberData( byteVariableArrayData, 0 );

        if( !m_byteVariableArrayDesc->getImpl()->shareSameData( prevData, byteVariableArrayData ) && ( prevDataSize != dataSize || memcmp( prevDataPtr, dataPtr, prevDataSize ) != 0 ) )
          changed = true;
        else if ( stringDesc->getValueLength( prevMimeTypeData ) != mimeType->length() || memcmp( mimeType->data(), stringDesc->getValueData( prevMimeTypeData ), mimeType->length() ) != 0 )
          changed = true;
        else if ( stringDesc->getValueLength( prevExtensionData ) != extension.length() || memcmp( extension.data(), stringDesc->getValueData( prevExtensionData ), extension.length() ) != 0 )
          changed = true;
        else if ( stringDesc->getValueLength( prevUrlData ) != m_streamURL.length() || memcmp( m_streamURL.data(), stringDesc->getValueData( prevUrlData ), m_streamURL.length() ) != 0 )
          changed = true;

        if( changed )
        {
          void* resourceData = getMutableData( "resource", 0 );
          m_byteVariableArrayDesc->setData( byteVariableArrayData, fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_DATA_MEMBER_INDEX ) );
          stringDesc->setValue( mimeType->data(), mimeType->length(), fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_MIMETYPE_MEMBER_INDEX ) );
          stringDesc->setValue( extension.data(), extension.length(), fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_EXTENSION_MEMBER_INDEX ) );
          stringDesc->setValue( m_streamURL.data(), m_streamURL.length(), fabricResourceStructDesc->getMemberData( resourceData, FABRIC_RESOURCE_URL_MEMBER_INDEX ) );

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
