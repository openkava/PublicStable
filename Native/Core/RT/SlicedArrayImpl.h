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
    class VariableArrayImpl;
    
    class SlicedArrayImpl : public ArrayImpl
    {
      friend class Manager;
      friend class Impl;
      
      struct bits_t
      {
        size_t offset;
        size_t size;
        void *variableArrayBits;
      };
    
    public:
    
      // Impl
      
      virtual void setData( void const *src, void *dst ) const;
      virtual void disposeDatasImpl( void *data, size_t count, size_t stride ) const;
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
      
      size_t getOffset( void const *data ) const;
      size_t getSize( void const *data ) const;
      void setNumMembers( void *data, size_t numMembers, void const *defaultMemberData ) const;
      
    protected:
    
      SlicedArrayImpl( std::string const &codeName, RC::ConstHandle<RT::Impl> const &memberImpl );

    private:

      RC::ConstHandle<Impl> m_memberImpl;
      size_t m_memberSize;
      bool m_memberIsShallow;
      RC::ConstHandle<VariableArrayImpl> m_variableArrayImpl;
   };
  };
};

#endif //_FABRIC_RT_SLICED_ARRAY_IMPL_H
