/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_VARIABLE_ARRAY_IMPL_H
#define _FABRIC_RT_VARIABLE_ARRAY_IMPL_H

#include <Fabric/Core/RT/ArrayImpl.h>
#include <Fabric/Base/Util/Bits.h>
#include <Fabric/Base/Util/AtomicSize.h>

#include <string.h>
#include <algorithm>

namespace Fabric
{
  namespace RT
  {
    class VariableArrayImpl : public ArrayImpl
    {
      friend class Manager;
      friend class Impl;
      friend class SlicedArrayImpl;
      
      struct bits_t
      {
        Util::AtomicSize refCount;
        size_t allocNumMembers;
        size_t numMembers;
        uint8_t memberDatas[0];
      };
    
    public:
    
      // Impl
      
      virtual void setData( void const *src, void *dst ) const;
      virtual void disposeData( void *data ) const;
      virtual std::string descData( void const *data ) const;
      virtual void const *getDefaultData() const;
      
      virtual RC::Handle<JSON::Value> getJSONValue( void const *data ) const;
      virtual void setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &value, void *data ) const;
      
      virtual bool isShallow() const;
      virtual bool isEquivalentTo( RC::ConstHandle< RT::Impl > const &desc ) const;

      // ArrayImpl
      
      virtual size_t getNumMembers( void const *data ) const;
      virtual void const *getMemberData( void const *data, size_t index ) const;
      virtual void *getMemberData( void *data, size_t index ) const;
      
      // VariableArrayImpl
      
      void *getBits( void *data ) const;
      void setNumMembers( void *data, size_t newNumMembers, void const *defaultMemberData = 0 ) const;
      void setMembers( void *data, size_t numMembers, void const *members ) const;
      void setMembers( void *data, size_t dstOffset, size_t numMembers, void const *members ) const;
      bool areSameData( const void *data1, const void *data2 ) const;
      
      void split( void *data ) const;
      void push( void *dst, void const *src ) const;
      void pop( void *dst, void *result ) const;
      void append( void *dst, void const *src ) const;
      
    protected:
    
      VariableArrayImpl( std::string const &codeName, RC::ConstHandle<RT::Impl> const &memberImpl );
      
      static size_t AllocNumMembersForNumMembers( size_t numMembers )
      {
        return std::max( size_t(15), Util::nextPowerOfTwoMinusOne( numMembers ) );
      }
            
      void const *getImmutableMemberData_NoCheck( void const *data, size_t index ) const
      { 
        bits_t const *bits = *reinterpret_cast<bits_t const * const *>(data);
        return bits->memberDatas + m_memberSize * index;
      }
      
      void *getMutableMemberData_NoCheck( void *data, size_t index ) const
      { 
        unshare( data );
        bits_t *bits = *reinterpret_cast<bits_t **>(data);
        return bits->memberDatas + m_memberSize * index;
      }    

      void unshare( void *data ) const
      {
        if ( (*reinterpret_cast<bits_t **>(data))->refCount.getValue() > 1 )
        {
          split( data );
        }
      }

      void copyMemberDatas( bits_t *dstBits, size_t dstOffset, bits_t const *srcBits, size_t srcOffset, size_t count, bool disposeFirst ) const
      {
        if ( count > 0 )
        {
          FABRIC_ASSERT( srcBits );
          FABRIC_ASSERT( dstBits );
          FABRIC_ASSERT( dstBits->refCount.getValue() == 1 );
          FABRIC_ASSERT( srcOffset + count <= srcBits->numMembers );
          FABRIC_ASSERT( dstOffset + count <= dstBits->numMembers );
          
          size_t byteSize = m_memberSize * count;
          
          size_t dstByteOffset = m_memberSize * dstOffset;
          uint8_t *dstMemberDataStart = dstBits->memberDatas + dstByteOffset;
          if ( disposeFirst && !m_memberIsShallow )
          {
            uint8_t *dstMemberDataEnd = dstMemberDataStart + byteSize;
            for ( uint8_t *dstMemberData = dstMemberDataStart; dstMemberData != dstMemberDataEnd; dstMemberData += m_memberSize )
              m_memberImpl->disposeData( dstMemberData );
          }
          
          size_t srcByteOffset = m_memberSize * srcOffset;
          uint8_t const *srcMemberData = srcBits->memberDatas + srcByteOffset;
          if ( !m_memberIsShallow )
          {
            uint8_t *dstMemberDataEnd = dstMemberDataStart + byteSize;
            for ( uint8_t *dstMemberData = dstMemberDataStart; dstMemberData != dstMemberDataEnd; srcMemberData += m_memberSize, dstMemberData += m_memberSize )
              m_memberImpl->setData( srcMemberData, dstMemberData );
          }
          else memcpy( dstMemberDataStart, srcMemberData, byteSize );
        }
      }

    private:

      RC::ConstHandle<Impl> m_memberImpl;
      size_t m_memberSize;
      bool m_memberIsShallow;
   };
  };
};

#endif //_FABRIC_RT_VARIABLE_ARRAY_IMPL_H
