/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "StructImpl.h"
#include "Desc.h"

#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Core/Util/Decoder.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace RT
  {
    StructImpl::StructImpl( std::string const &codeName, StructMemberInfoVector const &memberInfos )
      : Impl( codeName, DT_STRUCT )
      , m_memberInfos( memberInfos )
      , m_numMembers( memberInfos.size() )
    {
      m_isShallow = true;
      m_memberOffsets.push_back( 0 );
      for ( size_t i=0; i<m_memberInfos.size(); ++i )
      {
        StructMemberInfo const &memberInfo = getMemberInfo(i);
        m_memberOffsets.push_back( m_memberOffsets.back() + memberInfo.desc->getSize() );
        m_nameToIndexMap.insert( NameToIndexMap::value_type( memberInfo.name, i ) );
        m_isShallow = m_isShallow && memberInfo.desc->isShallow();
      }
      
      setSize( m_memberOffsets[m_numMembers] );
      
      // [pzion 20101118] This can't be combined in the loop above
      // because the loop has a side effect of computing total size
      // which is needed to create the default instance
      size_t size = m_memberOffsets[m_memberInfos.size()];
      m_defaultData = malloc( size );
      memset( m_defaultData, 0, size );
      for ( size_t i=0; i<m_numMembers; ++i )
      {
        StructMemberInfo &memberInfo = m_memberInfos[i];
        if ( !memberInfo.defaultData.size() )
        {
          memberInfo.defaultData.resize( memberInfo.desc->getSize(), 0 );
          memberInfo.desc->setData( memberInfo.desc->getDefaultData(), &memberInfo.defaultData[0] );
        }
        void const *srcMemberData = &memberInfo.defaultData[0];
        void *dstMemberData = static_cast<uint8_t *>(m_defaultData) + m_memberOffsets[i];
        memberInfo.desc->setData( srcMemberData, dstMemberData );
      }
    }
    
    StructImpl::~StructImpl()
    {
      disposeData( m_defaultData );
      free( m_defaultData );
    }
    
    void const *StructImpl::getDefaultData() const
    {
      return m_defaultData;
    }
    
    void StructImpl::setData( void const *srcData, void *dstData ) const
    {
      if ( m_isShallow )
        memcpy( dstData, srcData, getSize() );
      else
      {
        for ( size_t i=0; i<m_numMembers; ++i )
        {
          StructMemberInfo const &memberInfo = m_memberInfos[i];
          size_t memberOffset = m_memberOffsets[i];
          void const *srcMemberData = static_cast<uint8_t const *>(srcData) + memberOffset;
          void *dstMemberData = static_cast<uint8_t *>(dstData) + memberOffset;
          memberInfo.desc->setData( srcMemberData, dstMemberData );
        }
      }
    }
    
    RC::Handle<JSON::Value> StructImpl::getJSONValue( void const *data ) const
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      for ( size_t i=0; i<m_numMembers; ++i )
      {
        StructMemberInfo const &memberInfo = m_memberInfos[i];
        void const *memberData = static_cast<uint8_t const *>(data) + m_memberOffsets[i];
        result->set( memberInfo.name, memberInfo.desc->getJSONValue( memberData ) );
      }
      return result;
    }
    
    void StructImpl::setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *data ) const
    {
      if ( !jsonValue->isObject() )
        throw Exception("value is not an object");
      RC::ConstHandle<JSON::Object> jsonObject = RC::ConstHandle<JSON::Object>::StaticCast( jsonValue );
      
      for ( size_t i=0; i<m_numMembers; ++i )
      {
        StructMemberInfo const &memberInfo = m_memberInfos[i];

        RC::ConstHandle<JSON::Value> memberValue = jsonObject->maybeGet( memberInfo.name );
        if ( !memberValue )
          throw Exception( "value has no member named '" + memberInfo.name + "'" );
          
        void *memberData = static_cast<uint8_t *>(data) + m_memberOffsets[i];
        memberInfo.desc->setDataFromJSONValue( memberValue, memberData );
      }
    }

    void StructImpl::disposeData( void *data ) const
    {
      if ( !m_isShallow )
      {
        for ( size_t i=0; i<m_numMembers; ++i )
        {
          StructMemberInfo const &memberInfo = m_memberInfos[i];
          void *memberData = static_cast<uint8_t *>(data) + m_memberOffsets[i];
          memberInfo.desc->disposeData( memberData );
        }
      }
    }
    
    std::string StructImpl::descData( void const *data ) const
    {
      std::string result = "{";
      for ( size_t i=0; i<m_numMembers; ++i )
      {
        StructMemberInfo const &memberInfo = m_memberInfos[i];
        void const *memberData = static_cast<uint8_t const *>(data) + m_memberOffsets[i];
        if ( i > 0 )
          result += ",";
        result += memberInfo.name;
        result += ":";
        result += memberInfo.desc->descData( memberData );
      }
      result += "}";
      return result;
    }
    
    bool StructImpl::isShallow() const
    {
      return m_isShallow;
    }

    bool StructImpl::isEquivalentTo( RC::ConstHandle<Impl> const &thatImpl ) const
    {
      if ( !isStruct( thatImpl->getType() ) )
        return false;
      RC::ConstHandle<StructImpl> thatStructImpl = RC::ConstHandle<StructImpl>::StaticCast( thatImpl );
      
      size_t thisNumMembers = getNumMembers();
      size_t thatNumMembers = thatStructImpl->getNumMembers();
      if ( thisNumMembers != thatNumMembers )
        return false;
      
      for ( size_t i=0; i<thisNumMembers; ++i )
      {
        StructMemberInfo const &thisMemberInfo = getMemberInfo( i );
        StructMemberInfo const &thatMemberInfo = thatStructImpl->getMemberInfo( i );
        if ( thisMemberInfo.name != thatMemberInfo.name
          || !thisMemberInfo.desc->isEquivalentTo( thatMemberInfo.desc )
          //|| thisMemberInfo.defaultData != thatMemberInfo.defaultData
          )
          return false;
      }
      
      return true;
    }
  };
};
