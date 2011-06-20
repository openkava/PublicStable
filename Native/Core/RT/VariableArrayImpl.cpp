#include "VariableArrayImpl.h"

#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Core/Util/Decoder.h>
#include <Fabric/Core/Util/SimpleString.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Base/Config.h>

#include <algorithm>

namespace Fabric
{
  

  namespace RT
  {
    VariableArrayImpl::VariableArrayImpl( std::string const &codeName, RC::ConstHandle<Impl> const &memberImpl )
      : ArrayImpl( codeName, DT_VARIABLE_ARRAY, memberImpl )
      , m_memberImpl( memberImpl )
      , m_memberSize( memberImpl->getSize() )
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
      
    Util::Encoder &VariableArrayImpl::encode( Util::Encoder &encoder, void const *data ) const
    {
      size_t numMembers = getNumMembers( data );
      
      uint64_t uint64NumMembers = numMembers;
      encoder.put( uint64NumMembers );
      
      for ( size_t i=0; i<numMembers; ++i )
        getMemberImpl()->encode( encoder, getMemberData( data, i ) );
      
      return encoder;
    }
    
    Util::Decoder &VariableArrayImpl::decode( Util::Decoder &decoder, void *data ) const
    {
      uint64_t uint64NumMembers;
      decoder.get( uint64NumMembers );
      size_t numMembers = size_t( uint64NumMembers );
      if ( uint64_t(numMembers) != uint64NumMembers )
        throw Exception( "array is too large for architecture" );
      setNumMembers( data, numMembers );
      
      for ( size_t i=0; i<numMembers; ++i )
        getMemberImpl()->decode( decoder, getMemberData( data, i ) );
      
      return decoder;
    }  

    void VariableArrayImpl::disposeData( void *data ) const
    {
      bits_t *bits = *static_cast<bits_t **>(data);
      if ( bits && bits->refCount.decrementAndGetValue() == 0 )
      {
        if ( !m_memberIsShallow )
        {
          uint8_t *memberData = bits->memberDatas;
          uint8_t *memberDataEnd = memberData + m_memberSize * bits->numMembers;
          for ( ; memberData != memberDataEnd; memberData += m_memberSize )
            getMemberImpl()->disposeData( memberData );
        }
        
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
      
    void VariableArrayImpl::split( void *data ) const
    {
      bits_t *&bits = *reinterpret_cast<bits_t **>(data);
      bits_t *srcBits = bits;
      size_t allocNumMembers = AllocNumMembersForNumMembers( srcBits->numMembers );
      bits = static_cast<bits_t *>( malloc( sizeof(bits_t) + m_memberSize * allocNumMembers ) );
      bits->refCount = 1;
      bits->allocNumMembers = allocNumMembers;
      bits->numMembers = srcBits->numMembers;
      if ( !m_memberIsShallow )
        memset( bits->memberDatas, 0, m_memberSize * srcBits->numMembers );
      copyMemberDatas( bits, 0, srcBits, 0, srcBits->numMembers, false );
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

      return getMemberImpl()->isEquivalentTo( variableArrayImpl->getMemberImpl() );
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
      return getMemberData_NoCheck( data, index );
    }
    
    void *VariableArrayImpl::getMemberData( void *data, size_t index ) const
    { 
      bits_t *&bits = *reinterpret_cast<bits_t **>(data);
      size_t numMembers = bits? bits->numMembers: 0;
      if ( index >= numMembers )
        throw Exception( "index ("+_(index)+") out of range ("+_(numMembers)+")" );
      return getMemberData_NoCheck( data, index );
    }

    void VariableArrayImpl::setMembers( void *data, size_t numMembers, void const *members ) const
    {
      setNumMembers( data, numMembers );
      for ( size_t i=0; i<numMembers; ++i )
      {
        void *memberData = getMemberData( data, i );
        getMemberImpl()->setData( &((uint8_t const *)members)[i*m_memberSize], memberData );
      }
    }
    
    void VariableArrayImpl::setNumMembers( void *data, size_t newNumMembers, void const *defaultMemberData ) const
    {
      bits_t *&bits = *reinterpret_cast<bits_t **>(data);
      size_t oldNumMembers = bits? bits->numMembers: 0;
      size_t oldAllocNumMembers = bits? bits->allocNumMembers: 0;
      if ( oldNumMembers != newNumMembers )
      {
        if ( bits && bits->refCount.getValue() == 1 )
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
            memberData = bits->memberDatas + m_memberSize * oldNumMembers;
            memberDataEnd = bits->memberDatas + m_memberSize * newNumMembers;
            memset( memberData, 0, memberDataEnd - memberData );
            for ( ; memberData!=memberDataEnd; memberData += m_memberSize )
              getMemberImpl()->setData( defaultMemberData, memberData );
          }
        }
        else
        {
          if ( oldNumMembers )
            bits->refCount.decrementAndGetValue();
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
    }
  };
};
