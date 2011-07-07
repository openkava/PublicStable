#ifndef _FABRIC_RT_IMPL_H
#define _FABRIC_RT_IMPL_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/WeakConstHandle.h>
#include <Fabric/Core/RT/ImplType.h>
#include <Fabric/Core/Util/UnorderedMap.h>
#include <Fabric/Core/Util/Assert.h>

#include <stdint.h>

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
    class FixedArrayImpl;
    
    class Impl : public RC::Object
    {
    public:
      
      std::string const &getCodeName() const { return m_codeName; }
      size_t getSize() const { return m_size; }
      ImplType getType() const { return m_implType; }
      
      virtual void const *getDefaultData() const = 0;
      virtual void setData( void const *value, void *data ) const = 0;
      virtual void disposeData( void *data ) const = 0;
      virtual std::string descData( void const *data ) const = 0;
      
      virtual RC::Handle<JSON::Value> getJSONValue( void const *data ) const = 0;
      virtual void setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &value, void *data ) const = 0;
      
      virtual Util::Encoder &encode( Util::Encoder &encoder, void const *data ) const = 0;
      virtual Util::Decoder &decode( Util::Decoder &decoder, void *data ) const = 0;
      
      virtual bool isEquivalentTo( RC::ConstHandle<Impl> const &impl ) const = 0;
      virtual bool isShallow() const = 0;
      
      RC::ConstHandle<FixedArrayImpl> getFixedArrayImpl( size_t length ) const;
      RC::ConstHandle<VariableArrayImpl> getVariableArrayImpl() const;
      
    protected:
    
      Impl( std::string const &codeName, ImplType type );

      void setSize( size_t size );
      
    private:
    
      std::string m_codeName;
      ImplType m_implType;
      size_t m_size;
      
      mutable RC::WeakConstHandle<VariableArrayImpl> m_variableArrayImpl;
      mutable Util::UnorderedMap< size_t, RC::WeakConstHandle<FixedArrayImpl> > m_fixedArrayImpls;  
    };
  };
};

#endif // _FABRIC_RT_IMPL_H
