/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/RT/SlicedArrayImpl.h>
#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace RT
  {
    SlicedArrayImpl::SlicedArrayImpl( std::string const &codeName, RC::ConstHandle<Impl> const &memberImpl )
      : ArrayImpl( codeName, DT_SLICED_ARRAY, memberImpl )
      , m_memberImpl( memberImpl )
      , m_memberSize( memberImpl->getSize() )
      , m_memberIsShallow( memberImpl->isShallow() )
    {
      setSize( sizeof(bits_t) );
    }
    
    void const *SlicedArrayImpl::getDefaultData() const
    {
      static bits_t defaultData = { 0, 0, 0 };
      return &defaultData;
    }

    void SlicedArrayImpl::setData( void const *src, void *dst ) const
    {
      bits_t const *srcBits = reinterpret_cast<bits_t const *>(src);
      bits_t const *dstBits = reinterpret_cast<bits_t const *>(dst);
      if ( srcBits != dstBits )
      {
        if ( srcBits->size != dstBits->size )
          throw Exception( "source and destination sizes of sliced array must match" );
        uint8_t const *srcMember = srcBits->members + srcBits->offset * m_memberSize;
        uint8_t const *srcMemberEnd = srcBits->members + (srcBits->offset + srcBits->size) * m_memberSize;
        uint8_t *dstMember = dstBits->members + dstBits->offset * m_memberSize;
        while ( srcMember != srcMemberEnd )
        {
          m_memberImpl->setData( srcMember, dstMember );
          srcMember += m_memberSize;
          dstMember += m_memberSize;
        }
      }
    }

    RC::Handle<JSON::Value> SlicedArrayImpl::getJSONValue( void const *data ) const
    {
      bits_t const *srcBits = reinterpret_cast<bits_t const *>(data);
      RC::Handle<JSON::Array> arrayValue = JSON::Array::Create( srcBits->size );
      uint8_t const *srcMember = srcBits->members + srcBits->offset * m_memberSize;
      uint8_t const *srcMemberEnd = srcBits->members + (srcBits->offset + srcBits->size) * m_memberSize;
      for ( size_t i=0; srcMember!=srcMemberEnd; ++i )
      {
        arrayValue->set( i, m_memberImpl->getJSONValue( srcMember ) );
        srcMember += m_memberSize;
      }
      return arrayValue;
    }
    
    void SlicedArrayImpl::setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *data ) const
    {
      if ( !jsonValue->isArray() )
        throw Exception( "JSON value is not array" );
      RC::ConstHandle<JSON::Array> jsonArray = RC::ConstHandle<JSON::Array>::StaticCast( jsonValue );

      bits_t const *dstBits = reinterpret_cast<bits_t const *>(data);
      if ( jsonArray->size() != dstBits->size )
        throw Exception( "JSON array size must equal sliced array size" );

      uint8_t *dstMember = dstBits->members + dstBits->offset * m_memberSize;
      for ( size_t i=0; i<dstBits->size; ++i )
      {
        m_memberImpl->setDataFromJSONValue( jsonArray->get(i), dstMember );
        dstMember += m_memberSize;
      }
    }

    void SlicedArrayImpl::disposeData( void *data ) const
    {
      // [pzion 20110816] no-op: sliced arrays don't own references to their members
    }
    
    std::string SlicedArrayImpl::descData( void const *data ) const
    {
      std::string result = "[";
      bits_t const *srcBits = reinterpret_cast<bits_t const *>(data);
      size_t numMembers = srcBits->size;
      size_t numMembersToDisplay = numMembers;
      if ( numMembersToDisplay > 16 )
        numMembersToDisplay = 16;
      uint8_t const *srcMember = srcBits->members + srcBits->offset * m_memberSize;
      for ( size_t i=0; i<numMembersToDisplay; ++i )
      {
        if ( result.length() > 1 )
          result += ",";
        result += getMemberImpl()->descData( srcMember );
        srcMember += m_memberSize;
      }
      if ( numMembers > numMembersToDisplay )
        result += "...";
      result += "]";
      return result;
    }
    
    bool SlicedArrayImpl::isShallow() const
    {
      return false;
    }
    
    bool SlicedArrayImpl::isEquivalentTo( RC::ConstHandle<Impl> const &that ) const
    {
      if ( !isSlicedArray( that->getType() ) )
        return false;
      RC::ConstHandle<SlicedArrayImpl> slicedArrayImpl = RC::ConstHandle<SlicedArrayImpl>::StaticCast( that );

      return getMemberImpl()->isEquivalentTo( slicedArrayImpl->getMemberImpl() );
    }

    size_t SlicedArrayImpl::getNumMembers( void const *data ) const
    {
      FABRIC_ASSERT( data );
      bits_t const *srcBits = reinterpret_cast<bits_t const *>(data);
      return srcBits->size;
    }
    
    void const *SlicedArrayImpl::getMemberData( void const *data, size_t index ) const
    { 
      bits_t const *srcBits = reinterpret_cast<bits_t const *>(data);
      if ( index >= srcBits->size )
        throw Exception( "index ("+_(index)+") out of range ("+_(srcBits->size)+")" );
      return srcBits->members + (srcBits->offset + index) * m_memberSize;
    }
    
    void *SlicedArrayImpl::getMemberData( void *data, size_t index ) const
    { 
      bits_t const *srcBits = reinterpret_cast<bits_t const *>(data);
      if ( index >= srcBits->size )
        throw Exception( "index ("+_(index)+") out of range ("+_(srcBits->size)+")" );
      return srcBits->members + (srcBits->offset + index) * m_memberSize;
    }
  };
};
