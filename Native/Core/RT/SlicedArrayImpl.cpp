/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/RT/SlicedArrayImpl.h>
#include <Fabric/Core/RT/VariableArrayImpl.h>
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
      , m_memberSize( memberImpl->getAllocSize() )
      , m_memberIsShallow( memberImpl->isShallow() )
      , m_variableArrayImpl( memberImpl->getVariableArrayImpl() )
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
      bits_t *dstBits = reinterpret_cast<bits_t *>(dst);
      dstBits->offset = srcBits->offset;
      dstBits->size = srcBits->size;
      dstBits->variableArrayBits = srcBits->variableArrayBits;
    }

    RC::Handle<JSON::Value> SlicedArrayImpl::getJSONValue( void const *data ) const
    {
      bits_t const *bits = reinterpret_cast<bits_t const *>(data);
      RC::Handle<JSON::Array> arrayValue = JSON::Array::Create( bits->size );
      for ( size_t i=0; i<bits->size; ++i )
        arrayValue->set( i, m_variableArrayImpl->getJSONValue( m_variableArrayImpl->getImmutableMemberData_NoCheck( &bits->variableArrayBits, bits->offset + i ) ) );
      return arrayValue;
    }
    
    void SlicedArrayImpl::setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *data ) const
    {
      if ( !jsonValue->isArray() )
        throw Exception( "JSON value is not array" );
      RC::ConstHandle<JSON::Array> jsonArray = RC::ConstHandle<JSON::Array>::StaticCast( jsonValue );

      bits_t *dstBits = reinterpret_cast<bits_t *>(data);
      if ( jsonArray->size() != dstBits->size )
        throw Exception( "JSON array size must equal sliced array size" );

      for ( size_t i=0; i<dstBits->size; ++i )
        //m_memberImpl->setDataFromJSONValue( jsonArray->get(i), m_variableArrayImpl->getMutableMemberData_NoCheck( &dstBits->variableArrayBits, dstBits->offset + i ) );
        m_memberImpl->setDataFromJSONValue( jsonArray->get(i), (void*)m_variableArrayImpl->getImmutableMemberData_NoCheck( &dstBits->variableArrayBits, dstBits->offset + i ) );
    }

    void SlicedArrayImpl::disposeData( void *data ) const
    {
    }
    
    std::string SlicedArrayImpl::descData( void const *data ) const
    {
      bits_t const *srcBits = reinterpret_cast<bits_t const *>( data );

      size_t numMembers = srcBits->size;
      size_t numMembersToDisplay = numMembers;
      if ( numMembersToDisplay > 16 )
        numMembersToDisplay = 16;

      std::string result = "[";
      for ( size_t i=0; i<numMembersToDisplay; ++i )
      {
        if ( result.length() > 1 )
          result += ",";
        result += getMemberImpl()->descData( m_variableArrayImpl->getImmutableMemberData_NoCheck( &srcBits->variableArrayBits, srcBits->offset + i ) );
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
      return m_variableArrayImpl->getImmutableMemberData_NoCheck( &srcBits->variableArrayBits, srcBits->offset + index );
    }
    
    void *SlicedArrayImpl::getMemberData( void *data, size_t index ) const
    { 
      bits_t *srcBits = reinterpret_cast<bits_t *>(data);
      if ( index >= srcBits->size )
        throw Exception( "index ("+_(index)+") out of range ("+_(srcBits->size)+")" );
      return (void*)m_variableArrayImpl->getImmutableMemberData_NoCheck( &srcBits->variableArrayBits, srcBits->offset + index );
      //return m_variableArrayImpl->getMutableMemberData_NoCheck( &srcBits->variableArrayBits, srcBits->offset + index );
    }

    size_t SlicedArrayImpl::getOffset( void const *data ) const
    {
      bits_t const *bits = reinterpret_cast<bits_t const *>(data);
      return bits->offset;
    }
    
    size_t SlicedArrayImpl::getSize( void const *data ) const
    {
      bits_t const *bits = reinterpret_cast<bits_t const *>(data);
      return bits->size;
    }
    
    void *SlicedArrayImpl::getVariableArrayBits( void const *data ) const
    {
      bits_t const *bits = reinterpret_cast<bits_t const *>(data);
      return bits->variableArrayBits;
    }
    
    void SlicedArrayImpl::set( size_t offset, size_t size, void *variableArrayBits, void *data ) const
    {
      bits_t *bits = reinterpret_cast<bits_t *>(data);
      bits->offset = offset;
      bits->size = size;
      bits->variableArrayBits = variableArrayBits;
    }

    RC::ConstHandle<VariableArrayImpl> SlicedArrayImpl::getVariableArrayImpl() const
    {
      return m_variableArrayImpl;
    }
  };
};
