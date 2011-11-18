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
#include <Fabric/Core/Util/JSONGenerator.h>

#include <algorithm>

namespace Fabric
{
  namespace RT
  {
    VariableArrayImpl::VariableArrayImpl( std::string const &codeName, RC::ConstHandle<Impl> const &memberImpl )
      : ArrayImpl( codeName, DT_VARIABLE_ARRAY, memberImpl )
      , m_memberImpl( memberImpl )
      , m_memberSize( memberImpl->getAllocSize() )
      , m_memberIsShallow( memberImpl->isShallow() )
    {
      setSize( sizeof(bits_t) );
    }
    
    void const *VariableArrayImpl::getDefaultData() const
    {
      static bits_t defaultData = { 0, 0, 0 };
      return &defaultData;
    }

    void VariableArrayImpl::setData( void const *_src, void *_dst ) const
    {
      bits_t const *src = reinterpret_cast<bits_t const *>(_src);
      bits_t *dst = reinterpret_cast<bits_t *>(_dst);
      
      if ( dst->numMembers > src->numMembers )
        disposeDatas( dst->memberDatas + src->numMembers * m_memberSize, dst->numMembers - src->numMembers, m_memberSize );
      dst->numMembers = src->numMembers;

      dst->allocNumMembers = AllocNumMembersForNumMembers( src->numMembers );
      if ( dst->memberDatas )
        dst->memberDatas = reinterpret_cast<uint8_t *>( realloc( dst->memberDatas, dst->allocNumMembers * m_memberSize ) );
      else
        dst->memberDatas = reinterpret_cast<uint8_t *>( malloc( dst->allocNumMembers * m_memberSize ) );
      
      size_t totalSize = src->numMembers * m_memberSize;
      uint8_t const *srcMemberData = src->memberDatas;
      uint8_t const * const srcMemberDataEnd = srcMemberData + totalSize;
      uint8_t *dstMemberData = dst->memberDatas;
      memset( dstMemberData, 0, totalSize );
      while ( srcMemberData != srcMemberDataEnd )
      {
        m_memberImpl->setData( srcMemberData, dstMemberData );
        srcMemberData += m_memberSize;
        dstMemberData += m_memberSize;
      }
    }

    RC::Handle<JSON::Value> VariableArrayImpl::getJSONValue( void const *data ) const
    {
      size_t numMembers = getNumMembers(data);
      
      RC::Handle<JSON::Array> arrayValue = JSON::Array::Create( numMembers );
      for ( size_t i = 0; i < numMembers; ++i )
      {
        void const *srcMemberData = getImmutableMemberData_NoCheck( data, i );
        arrayValue->set( i, m_memberImpl->getJSONValue( srcMemberData ) );
      }
      return arrayValue;
    }
    
    void VariableArrayImpl::generateJSON( void const *data, Util::JSONGenerator &jsonGenerator ) const
    {
      size_t numMembers = getNumMembers(data);
      
      Util::JSONArrayGenerator jsonArrayGenerator = jsonGenerator.makeArray();
      for ( size_t i = 0; i < numMembers; ++i )
      {
        void const *memberData = getImmutableMemberData_NoCheck( data, i );
        Util::JSONGenerator elementJG = jsonArrayGenerator.makeElement();
        m_memberImpl->generateJSON( memberData, elementJG );
      }
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
        void *memberData = getMutableMemberData_NoCheck( data, i );
        getMemberImpl()->setDataFromJSONValue( jsonArray->get(i), memberData );
      }
    }

    void VariableArrayImpl::disposeDatasImpl( void *_data, size_t count, size_t stride ) const
    {
      uint8_t *data = static_cast<uint8_t *>( _data );
      uint8_t * const dataEnd = data + count * stride;
      while ( data != dataEnd )
      {
        bits_t *bits = reinterpret_cast<bits_t *>(data);
        m_memberImpl->disposeDatas( bits->memberDatas, bits->numMembers, m_memberSize );
        free( bits->memberDatas );
        data += stride;
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
        result += m_memberImpl->descData( getImmutableMemberData_NoCheck( data, i ) );
      }
      if ( numMembers > numMembersToDisplay )
        result += "...";
      result += "]";
      return result;
    }

    void VariableArrayImpl::push( void *dst, void const *src ) const
    {
      size_t oldNumMembers = getNumMembers( dst );
      setNumMembers( dst, oldNumMembers + 1 );
      getMemberImpl()->setData( src, getMutableMemberData_NoCheck( dst, oldNumMembers ) );
    }
    
    void VariableArrayImpl::pop( void *dst, void *result ) const
    {
      size_t oldNumMembers = getNumMembers( dst );
      if ( result )
        getMemberImpl()->setData( getImmutableMemberData_NoCheck( (void const *)dst, oldNumMembers - 1 ), result );
      setNumMembers( dst, oldNumMembers - 1 );
    }
    
    void VariableArrayImpl::append( void *dst, void const *src ) const
    {
      bits_t const *srcBits = reinterpret_cast<bits_t const *>(src);
      if ( srcBits )
      {
        bits_t *dstBits = static_cast<bits_t *>( dst );
        size_t oldNumMembers = dstBits->numMembers;
        size_t newNumMembers = oldNumMembers + srcBits->numMembers;
        setNumMembers( dst, newNumMembers );
        copyMemberDatas( dstBits, oldNumMembers, srcBits, 0, srcBits->numMembers, false );
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

      return getMemberImpl()->isEquivalentTo( variableArrayImpl->getMemberImpl() );
    }

    size_t VariableArrayImpl::getNumMembers( void const *data ) const
    {
      FABRIC_ASSERT( data );
      bits_t const *bits = static_cast<bits_t const *>(data);
      return bits->numMembers;
    }
    
    void const *VariableArrayImpl::getMemberData( void const *data, size_t index ) const
    { 
      bits_t const *bits = reinterpret_cast<bits_t const *>(data);
      size_t numMembers = bits->numMembers;
      if ( index >= numMembers )
        throw Exception( "index ("+_(index)+") out of range ("+_(numMembers)+")" );
      return getImmutableMemberData_NoCheck( data, index );
    }
    
    void *VariableArrayImpl::getMemberData( void *data, size_t index ) const
    { 
      bits_t *bits = reinterpret_cast<bits_t *>(data);
      size_t numMembers = bits->numMembers;
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
        bits_t *bits = reinterpret_cast<bits_t *>(data);
        memcpy( bits->memberDatas + dstOffset*m_memberSize, members, numMembers*m_memberSize );
      }
    }

    void VariableArrayImpl::setNumMembers( void *data, size_t newNumMembers, void const *defaultMemberData ) const
    {
      //Util::Timer ft;
      bits_t *bits = reinterpret_cast<bits_t *>(data);
      size_t oldNumMembers = bits->numMembers;
      size_t oldAllocNumMembers = bits->allocNumMembers;
      if ( oldNumMembers != newNumMembers )
      {
        if ( newNumMembers < oldNumMembers )
          getMemberImpl()->disposeDatas( bits->memberDatas + m_memberSize * newNumMembers, oldNumMembers - newNumMembers, m_memberSize );
          
        if ( newNumMembers == 0 )
        {
          free( bits->memberDatas );
          bits->allocNumMembers = 0;
          bits->memberDatas = 0;
        }
        else
        {
          if ( newNumMembers > oldAllocNumMembers )
          {
            size_t newAllocNumMembers = AllocNumMembersForNumMembers( newNumMembers );
            if ( oldNumMembers )
            {
              size_t size = m_memberSize * newAllocNumMembers;
              //FABRIC_LOG( "realloc(%u)", (unsigned)size );
              bits->memberDatas = static_cast<uint8_t *>( realloc( bits->memberDatas, size ) );
            }
            else
            {
              size_t size = m_memberSize * newAllocNumMembers;
              //FABRIC_LOG( "malloc(%u)", (unsigned)size );
              bits->memberDatas = static_cast<uint8_t *>( malloc( size ) );
            }
            bits->allocNumMembers = newAllocNumMembers;
          }
        }
        bits->numMembers = newNumMembers;
        
        if ( oldNumMembers < newNumMembers )
        {
          if ( !defaultMemberData )
            defaultMemberData = getMemberImpl()->getDefaultData();
          size_t memberSize = getMemberImpl()->getAllocSize();
          uint8_t *memberData = bits->memberDatas + m_memberSize * oldNumMembers;
          uint8_t *memberDataEnd = bits->memberDatas + m_memberSize * newNumMembers;
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
      //FABRIC_LOG( "VariableArrayImpl::setNumMembers: %fms", ft.getElapsedMS() );
    }
  };
};
