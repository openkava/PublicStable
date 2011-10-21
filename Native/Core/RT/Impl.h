/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_IMPL_H
#define _FABRIC_RT_IMPL_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/WeakConstHandle.h>
#include <Fabric/Core/RT/ImplType.h>
#include <Fabric/Core/Util/UnorderedMap.h>
#include <Fabric/Core/Util/Assert.h>

#include <stdint.h>
#include <map>

namespace Fabric
{
  namespace Util
  {
    class Encoder;
    class Decoder;
    class SimpleString;
  };
  
  namespace JSON
  {
    class Value;
  };
  
  namespace RT
  {
    class VariableArrayImpl;
    class SlicedArrayImpl;
    class FixedArrayImpl;
    class FixedArrayImpl;
    class ComparableImpl;
    class DictImpl;
    
    class Impl : public RC::Object
    {
    public:
      
      std::string const &getCodeName() const { return m_codeName; }
      size_t getAllocSize() const { return m_size; }
      ImplType getType() const { return m_implType; }
      
      virtual void const *getDefaultData() const = 0;
      virtual void setData( void const *value, void *data ) const = 0;
      void disposeData( void *data ) const;
      void disposeDatas( void *data, size_t count, size_t stride ) const;
      virtual void disposeDatasImpl( void *data, size_t count, size_t stride ) const = 0;
      virtual std::string descData( void const *data ) const = 0;
      
      virtual RC::Handle<JSON::Value> getJSONValue( void const *data ) const = 0;
      virtual void setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &value, void *data ) const = 0;
      
      virtual bool isEquivalentTo( RC::ConstHandle<Impl> const &impl ) const = 0;
      virtual bool isShallow() const = 0;
      
      RC::ConstHandle<FixedArrayImpl> getFixedArrayImpl( size_t length ) const;
      RC::ConstHandle<VariableArrayImpl> getVariableArrayImpl( size_t flags ) const;
      RC::ConstHandle<SlicedArrayImpl> getSlicedArrayImpl() const;
      RC::ConstHandle<DictImpl> getDictImpl( RC::ConstHandle<ComparableImpl> const &comparableImpl ) const;
      
      void setDisposeCallback( void (*disposeCallback)( void * ) ) const;
      
    protected:
    
      Impl( std::string const &codeName, ImplType type );

      void setSize( size_t size );
      
    private:
    
      std::string m_codeName;
      ImplType m_implType;
      size_t m_size;
      
      mutable Util::UnorderedMap< size_t, RC::WeakConstHandle<VariableArrayImpl> > m_variableArrayImpls;
      mutable RC::WeakConstHandle<SlicedArrayImpl> m_slicedArrayImpl;
      mutable Util::UnorderedMap< size_t, RC::WeakConstHandle<FixedArrayImpl> > m_fixedArrayImpls;
      mutable std::map< RC::WeakConstHandle<ComparableImpl>, RC::WeakConstHandle<DictImpl> > m_dictImpls;
      
      mutable void (*m_disposeCallback)( void *lValue );
    };
  };
};

#endif // _FABRIC_RT_IMPL_H
