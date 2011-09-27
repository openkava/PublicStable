/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "FixedArrayImpl.h"

#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Core/Util/Decoder.h>
#include <Fabric/Base/Util/SimpleString.h>
#include <Fabric/Core/Util/Format.h>

namespace Fabric
{
  namespace RT
  {
    FixedArrayImpl::FixedArrayImpl( std::string const &codeName, RC::ConstHandle<Impl> const &memberImpl, size_t length )
      : ArrayImpl( codeName, DT_FIXED_ARRAY, memberImpl )
      , m_memberImpl( memberImpl )
      , m_memberSize( memberImpl->getAllocSize() )
      , m_memberIsShallow( memberImpl->isShallow() )
      , m_length( length )
    {
      setSize( m_memberSize * m_length );

      m_defaultData = malloc( getAllocSize() );
      memset( m_defaultData, 0, getAllocSize() );
      void const *memberDefaultData = m_memberImpl->getDefaultData();
      for ( size_t i=0; i<m_length; ++i )
        m_memberImpl->setData( memberDefaultData, getMemberData( m_defaultData, i ) );
    }
    
    FixedArrayImpl::~FixedArrayImpl()
    {
      disposeData( m_defaultData );
      free( m_defaultData );
    }
    
    void const *FixedArrayImpl::getDefaultData() const
    {
      return m_defaultData;
    }

    void FixedArrayImpl::setData( void const *src, void *dst ) const
    {
      if ( !m_memberIsShallow )
      {
        for ( size_t i=0; i<m_length; ++i )
          m_memberImpl->setData( getMemberData_NoCheck( src, i ), getMemberData_NoCheck( dst, i ) );
      }
      else memcpy( dst, src, getAllocSize() );
    }

    RC::Handle<JSON::Value> FixedArrayImpl::getJSONValue( void const *data ) const
    {
      RC::Handle<JSON::Array> arrayValue = JSON::Array::Create( m_length );
      for ( size_t i = 0; i < m_length; ++i )
      {
        void const *srcMemberData = getMemberData_NoCheck( data, i );
        arrayValue->set( i, m_memberImpl->getJSONValue( srcMemberData ) );
      }
      return arrayValue;
    }
    
    void FixedArrayImpl::setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *data ) const
    {
      if ( !jsonValue->isArray() )
        throw Exception( "JSON value is not array" );
      RC::ConstHandle<JSON::Array> jsonArray = RC::ConstHandle<JSON::Array>::StaticCast( jsonValue );

      if ( jsonArray->size() != m_length )
        throw Exception( "JSON array is of wrong size (expected " + _(m_length) + ", actual " + _(jsonArray->size()) + ")" );

      for ( size_t i=0; i<m_length; ++i )
      {
        void *memberData = getMemberData_NoCheck( data, i );
        m_memberImpl->setDataFromJSONValue( jsonArray->get(i), memberData );
      }
    }

    void FixedArrayImpl::disposeDataImpl( void *data ) const
    {
      if ( !m_memberIsShallow )
      {
        for ( size_t i=0; i<m_length; ++i )
          m_memberImpl->disposeData( getMemberData_NoCheck( data, i) );
      }
    }
    
    bool FixedArrayImpl::isEquivalentTo( RC::ConstHandle<Impl> const &impl ) const
    {
      if ( !isFixedArray( impl->getType() ) )
        return false;
      RC::ConstHandle<FixedArrayImpl> fixedArrayImpl = RC::ConstHandle<FixedArrayImpl>::StaticCast( impl );
      
      return m_length == fixedArrayImpl->m_length
        && m_memberImpl->isEquivalentTo( fixedArrayImpl->m_memberImpl );
    }
    
    size_t FixedArrayImpl::getNumMembers( void const *data ) const
    {
      return m_length;
    }
          
    void const *FixedArrayImpl::getMemberData( void const *data, size_t index ) const
    { 
      if ( index >= m_length )
        throw Exception( "index out of range" );
      return getMemberData_NoCheck( data, index );
    }
    
    void *FixedArrayImpl::getMemberData( void *data, size_t index ) const
    { 
      if ( index >= m_length )
        throw Exception( "index out of range" );
      return getMemberData_NoCheck( data, index );
    }
    
    size_t FixedArrayImpl::getNumMembers() const
    {
      return m_length;
    }
    
    bool FixedArrayImpl::isShallow() const
    {
      return m_memberIsShallow;
    }
  };
};
