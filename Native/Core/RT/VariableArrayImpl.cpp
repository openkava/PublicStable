/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "VariableArrayImpl.h"

#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Core/Util/Decoder.h>
#include <Fabric/Base/Util/SimpleString.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/Timer.h>
#include <Fabric/Base/Config.h>
#include <Fabric/Base/Util/Bits.h>

#include <algorithm>

namespace Fabric
{
  namespace RT
  {
    VariableArrayImpl::VariableArrayImpl( std::string const &codeName, size_t flags, RC::ConstHandle<Impl> const &memberImpl )
      : ArrayImpl( codeName, DT_VARIABLE_ARRAY, memberImpl )
      , m_flags( flags )
      , m_memberImpl( memberImpl )
      , m_memberSize( memberImpl->getAllocSize() )
      , m_memberIsShallow( memberImpl->isShallow() )
    {
      setSize( sizeof(bits_t *) );
    }
    
    void const *VariableArrayImpl::getDefaultData() const
    {
      static bits_t *defaultData = 0;
      return &defaultData;
    }

    void VariableArrayImpl::setData( void const *src, void *dst ) const
    {
      bits_t *srcBits = *reinterpret_cast<bits_t * const *>(src);
      bits_t *&dstBits = *reinterpret_cast<bits_t **>(dst);
      if ( srcBits != dstBits )
      {
        if ( dstBits && dstBits->refCount.decrementAndGetValue() == 0 )
        {
          if ( !m_memberIsShallow )
          {
            uint8_t *dstMemberData = dstBits->memberDatas;
            uint8_t *dstMemberDataEnd = dstMemberData + dstBits->numMembers * m_memberSize;
            for ( ; dstMemberData != dstMemberDataEnd; dstMemberData += m_memberSize )
              getMemberImpl()->disposeData( dstMemberData );
          }
          free( dstBits );
        }
        
        dstBits = srcBits;
        
        if ( dstBits )
          dstBits->refCount.getValueAndIncrement();
      }
    }

    RC::Handle<JSON::Value> VariableArrayImpl::getJSONValue( void const *data ) const
    {
      size_t numMembers = getNumMembers(data);
      
      RC::Handle<JSON::Array> arrayValue = JSON::Array::Create( numMembers );
      for ( size_t i = 0; i < numMembers; ++i )
      {
        void const *srcMemberData = getMemberData( data, i );
        arrayValue->set( i, getMemberImpl()->getJSONValue( srcMemberData ) );
      }
      return arrayValue;
    }
    
    void VariableArrayImpl::setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *data ) const
    {
      if ( !jsonValue->isArray() )
        throw Exception( "JSON value is not array" );
      RC::ConstHandle<JSON::Array> jsonArray = RC::ConstHandle<JSON::Array>::StaticCast( jsonValue );

      size_t numMembers = jsonArray->size();
      setNumMembers( data, numMembers );

      for ( size_t i=0; i<numMembers; ++i )
      {
        void *memberData = getMemberData( data, i );
        getMemberImpl()->setDataFromJSONValue( jsonArray->get(i), memberData );
      }
    }

    void VariableArrayImpl::disposeDataImpl( void *data ) const
    {
      bits_t *bits = *static_cast<bits_t **>(data);
      if ( bits && bits->refCount.decrementAndGetValue() == 0 )
      {
        uint8_t *memberData = bits->memberDatas;
        uint8_t *memberDataEnd = memberData + m_memberSize * bits->numMembers;
        for ( ; memberData != memberDataEnd; memberData += m_memberSize )
          getMemberImpl()->disposeData( memberData );
        
        free( bits );
      }
    }
    
    std::string VariableArrayImpl::descData( void const *data ) const
    {
      std::string result = "[";
      size_t numMembers = getNumMembers(data);
      size_t numMembersToDisplay = numMembers;
      if ( numMembersToDisplay > 16 )
        numMembersToDisplay = 16;
      for ( size_t i=0; i<numMembersToDisplay; ++i )
      {
        if ( result.length() > 1 )
          result += ",";
        result += getMemberImpl()->descData( getMemberData( data, i ) );
      }
      if ( numMembers > numMembersToDisplay )
        result += "...";
      result += "]";
      return result;
    }

    bool VariableArrayImpl::areSameData( const void *data1, const void *data2 ) const
    {
      FABRIC_ASSERT( data1 && data2 );
      bits_t const *bits1 = *static_cast<bits_t const * const *>(data1);
      bits_t const *bits2 = *static_cast<bits_t const * const *>(data2);
      return bits1 == bits2;
    }
      
    void VariableArrayImpl::split( void *data ) const
    {
      FABRIC_ASSERT( m_flags & FLAG_COPY_ON_WRITE );
      
      bits_t *&bits = *reinterpret_cast<bits_t **>(data);
      
      bits_t *srcBits = bits;
      
      size_t allocNumMembers = AllocNumMembersForNumMembers( srcBits->numMembers );
      bits_t *dstBits = static_cast<bits_t *>( malloc( sizeof(bits_t) + m_memberSize * allocNumMembers ) );
      dstBits->refCount = 1;
      dstBits->allocNumMembers = allocNumMembers;
      dstBits->numMembers = srcBits->numMembers;
      if ( !m_memberIsShallow )
        memset( dstBits->memberDatas, 0, m_memberSize * srcBits->numMembers );
      copyMemberDatas( dstBits, 0, srcBits, 0, srcBits->numMembers, false );
      bits = dstBits;
      
      if ( srcBits->refCount.decrementAndGetValue() == 0 )
      {
        if ( !m_memberIsShallow )
        {
          uint8_t *memberData = srcBits->memberDatas;
          uint8_t *memberDataEnd = memberData + m_memberSize * srcBits->numMembers;
          for ( ; memberData != memberDataEnd; memberData += m_memberSize )
            getMemberImpl()->disposeData( memberData );
        }
        
        free( srcBits );
      }
    }

    void VariableArrayImpl::push( void *dst, void const *src ) const
    {
      size_t oldNumMembers = getNumMembers( dst );
      setNumMembers( dst, oldNumMembers + 1 );
      getMemberImpl()->setData( src, getMemberData( dst, oldNumMembers ) );
    }
    
    void VariableArrayImpl::pop( void *dst, void *result ) const
    {
      size_t oldNumMembers = getNumMembers( dst );
      if ( result )
        getMemberImpl()->setData( getMemberData( (void const *)dst, oldNumMembers - 1 ), result );
      setNumMembers( dst, oldNumMembers - 1 );
    }
    
    void VariableArrayImpl::append( void *dst, void const *src ) const
    {
      bits_t *srcBits = *reinterpret_cast<bits_t * const *>(src);
      if ( srcBits )
      {
        bits_t *&dstBits = *static_cast<bits_t **>( dst );
        if ( dstBits )
        {
          size_t oldNumMembers = dstBits->numMembers;
          size_t newNumMembers = oldNumMembers + srcBits->numMembers;
          setNumMembers( dst, newNumMembers );
          copyMemberDatas( dstBits, oldNumMembers, srcBits, 0, srcBits->numMembers, false );
        }
        else
        {
          dstBits = srcBits;
          dstBits->refCount.getValueAndIncrement();
        }
      }
    }
    
    bool VariableArrayImpl::isShallow() const
    {
      return false;
    }
    
    bool VariableArrayImpl::isEquivalentTo( RC::ConstHandle<Impl> const &that ) const
    {
      if ( !isVariableArray( that->getType() ) )
        return false;
      RC::ConstHandle<VariableArrayImpl> variableArrayImpl = RC::ConstHandle<VariableArrayImpl>::StaticCast( that );

      return m_flags == variableArrayImpl->m_flags
        && getMemberImpl()->isEquivalentTo( variableArrayImpl->getMemberImpl() );
    }

    size_t VariableArrayImpl::getNumMembers( void const *data ) const
    {
      FABRIC_ASSERT( data );
      bits_t const *bits = *static_cast<bits_t const * const *>(data);
      return bits? bits->numMembers: 0;
    }
    
    void const *VariableArrayImpl::getMemberData( void const *data, size_t index ) const
    { 
      bits_t const *bits = *reinterpret_cast<bits_t const * const *>(data);
      size_t numMembers = bits? bits->numMembers: 0;
      if ( index >= numMembers )
        throw Exception( "index ("+_(index)+") out of range ("+_(numMembers)+")" );
      return getImmutableMemberData_NoCheck( data, index );
    }
    
    void *VariableArrayImpl::getMemberData( void *data, size_t index ) const
    { 
      bits_t *&bits = *reinterpret_cast<bits_t **>(data);
      size_t numMembers = bits? bits->numMembers: 0;
      if ( index >= numMembers )
        throw Exception( "index ("+_(index)+") out of range ("+_(numMembers)+")" );
      return getMutableMemberData_NoCheck( data, index );
    }

    void VariableArrayImpl::setMembers( void *data, size_t numMembers, void const *members ) const
    {
      setNumMembers( data, numMembers );
      setMembers( data, 0, numMembers, members );
    }

    void VariableArrayImpl::setMembers( void *data, size_t dstOffset, size_t numMembers, void const *members ) const
    {
      FABRIC_ASSERT( numMembers + dstOffset <= getNumMembers( data ) );
      if ( !m_memberIsShallow )
      {
        for ( size_t i=0; i<numMembers; ++i )
        {
          void *memberData = getMemberData( data, i+dstOffset );
          getMemberImpl()->setData( &((uint8_t const *)members)[i*m_memberSize], memberData );
        }
      }
      else
      {
        if ( isCopyOnWrite() )
          unshare( data );
        bits_t *&bits = *reinterpret_cast<bits_t **>(data);
        memcpy( bits->memberDatas + dstOffset*m_memberSize, members, numMembers*m_memberSize );
      }
    }

    void VariableArrayImpl::setNumMembers( void *data, size_t newNumMembers, void const *defaultMemberData ) const
    {
      //Util::Timer ft;
      bits_t *&bits = *reinterpret_cast<bits_t **>(data);
      size_t oldNumMembers = bits? bits->numMembers: 0;
      size_t oldAllocNumMembers = bits? bits->allocNumMembers: 0;
      if ( oldNumMembers != newNumMembers )
      {
        if ( bits && ( !isCopyOnWrite() || bits->refCount.getValue() == 1 ) )
        {
          uint8_t *memberData, *memberDataEnd;
          
          if ( !m_memberIsShallow && newNumMembers < oldNumMembers )
          {
            memberData = bits->memberDatas + m_memberSize * newNumMembers;
            memberDataEnd = bits->memberDatas + m_memberSize * oldNumMembers;
            for ( ; memberData!=memberDataEnd; memberData += m_memberSize )
              getMemberImpl()->disposeData( memberData );
          }
          
          if ( newNumMembers == 0 )
          {
            free( bits );
            bits = 0;
          }
          else
          {
            if ( newNumMembers > oldAllocNumMembers )
            {
              size_t newAllocNumMembers = AllocNumMembersForNumMembers( newNumMembers );
              if ( oldNumMembers )
                bits = static_cast<bits_t *>( realloc( bits, sizeof(bits_t) + m_memberSize * newAllocNumMembers ) );
              else
              {
                bits = static_cast<bits_t *>( malloc( sizeof(bits_t) + m_memberSize * newAllocNumMembers ) );
                bits->refCount.setValue( 1 );
              }
              bits->allocNumMembers = newAllocNumMembers;
            }
            bits->numMembers = newNumMembers;
          }
          
          if ( oldNumMembers < newNumMembers )
          {
            if ( !defaultMemberData )
              defaultMemberData = getMemberImpl()->getDefaultData();
            size_t memberSize = getMemberImpl()->getAllocSize();
            memberData = bits->memberDatas + m_memberSize * oldNumMembers;
            memberDataEnd = bits->memberDatas + m_memberSize * newNumMembers;
            memset( memberData, 0, memberDataEnd - memberData );
            // [pzion 20110908] Very special case: if the default data is
            // all 0, we just memset to 0 and leave it at that
            if ( !Util::IsZero( memberSize, defaultMemberData ) )
            {
              for ( ; memberData!=memberDataEnd; memberData += m_memberSize )
              {
                if ( !m_memberIsShallow )
                  getMemberImpl()->setData( defaultMemberData, memberData );
                else
                  memcpy( memberData, defaultMemberData, memberSize );
              }
            }
          }
        }
        else
        {
          if ( oldNumMembers )
          {
            if ( bits->refCount.decrementAndGetValue() == 0 )
            {
              if ( !m_memberIsShallow )
              {
                uint8_t *dstMemberData = bits->memberDatas;
                uint8_t *dstMemberDataEnd = dstMemberData + bits->numMembers * m_memberSize;
                for ( ; dstMemberData != dstMemberDataEnd; dstMemberData += m_memberSize )
                  getMemberImpl()->disposeData( dstMemberData );
              }
              free( bits );
            }
          }

          if ( newNumMembers )
          {
            bits_t const *srcBits = bits;
            size_t allocNumMembers = AllocNumMembersForNumMembers( newNumMembers );
            bits = static_cast<bits_t *>( malloc( sizeof(bits_t) + m_memberSize * allocNumMembers ) );
            bits->refCount.setValue( 1 );
            bits->allocNumMembers = allocNumMembers;
            bits->numMembers = newNumMembers;
            memset( bits->memberDatas, 0, m_memberSize * newNumMembers );
            size_t count = std::min( oldNumMembers, newNumMembers );
            copyMemberDatas( bits, 0, srcBits, 0, count, false );
            
            if ( count < newNumMembers )
            {
              if ( !defaultMemberData )
                defaultMemberData = getMemberImpl()->getDefaultData();
              uint8_t *dstMemberData = bits->memberDatas + m_memberSize * count;
              uint8_t *dstMemberDataEnd = bits->memberDatas + m_memberSize * newNumMembers;
              for ( ; dstMemberData != dstMemberDataEnd; dstMemberData += m_memberSize )
                getMemberImpl()->setData( defaultMemberData, dstMemberData );
            }
          }
          else bits = 0;
        }
      }
      //FABRIC_LOG( "VariableArrayImpl::setNumMembers: %fms", ft.getElapsedMS() );
    }
  };
};
