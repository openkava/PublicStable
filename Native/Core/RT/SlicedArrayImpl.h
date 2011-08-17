/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_SLICED_ARRAY_IMPL_H
#define _FABRIC_RT_SLICED_ARRAY_IMPL_H

#include <Fabric/Core/RT/ArrayImpl.h>
#include <Fabric/Base/Util/Bits.h>
#include <Fabric/Base/Util/AtomicSize.h>

#include <string.h>
#include <algorithm>

namespace Fabric
{
  namespace RT
  {
    class SlicedArrayImpl : public ArrayImpl
    {
      friend class Manager;
      friend class Impl;
      
      struct bits_t
      {
        size_t offset;
        size_t size;
        uint8_t *members;
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
      virtual bool isEquivalentTo( RC::ConstHandle<RT::Impl> const &desc ) const;

      // ArrayImpl
      
      virtual size_t getNumMembers( void const *data ) const;
      virtual void const *getMemberData( void const *data, size_t index ) const;
      virtual void *getMemberData( void *data, size_t index ) const;
      
      // SlicedArrayImpl
      
      size_t getOffset( void const *data )
      {
        bits_t const *bits = reinterpret_cast<bits_t const *>(data);
        return bits->offset;
      }
      size_t getSize( void const *data )
      {
        bits_t const *bits = reinterpret_cast<bits_t const *>(data);
        return bits->size;
      }
      void *getMembers( void const *data )
      {
        bits_t const *bits = reinterpret_cast<bits_t const *>(data);
        return bits->members;
      }
      void set( size_t offset, size_t size, void *members, void *data ) const
      {
        bits_t *bits = reinterpret_cast<bits_t *>(data);
        bits->offset = offset;
        bits->size = size;
        bits->members = reinterpret_cast<uint8_t *>( members );
      }
      
    protected:
    
      SlicedArrayImpl( std::string const &codeName, RC::ConstHandle<RT::Impl> const &memberImpl );

    private:

      RC::ConstHandle<Impl> m_memberImpl;
      size_t m_memberSize;
      bool m_memberIsShallow;
   };
  };
};

#endif //_FABRIC_RT_SLICED_ARRAY_IMPL_H
