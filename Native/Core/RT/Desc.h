#ifndef _FABRIC_RT_DESC_H
#define _FABRIC_RT_DESC_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Core/RT/ImplType.h>
#include <Fabric/Core/Util/UnorderedMap.h>

#include <stdint.h>

namespace Fabric
{
  namespace Util
  {
    class SimpleString;
    class Encoder;
    class Decoder;
  };
  
  namespace JSON
  {
    class Value;
    class Object;
  };
  
  namespace RT
  {
    class VariableArrayDesc;
    class FixedArrayDesc;
    class Impl;
  
    class Desc : public RC::Object
    {
      friend class Manager;
      
    public:
      
      static RC::ConstHandle<Desc> Create( std::string const &name, RC::ConstHandle<Impl> const &impl )
      {
        return new Desc( name, impl );
      }
    
      std::string const &getName() const
      {
        return m_name;
      }
      size_t getSize() const;
      RC::ConstHandle<Impl> getImpl() const;
      
      ImplType getType() const;
      virtual bool isArrayDesc() const { return false; }
      
      void const *getDefaultData() const;
      void setData( void const *value, void *data ) const;
      void disposeData( void *data ) const;
      std::string descData( void const *data ) const;
      
      RC::Handle<JSON::Value> getJSONValue( void const *data ) const;
      void setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &value, void *data ) const;

      void setKLBindingsAST( RC::ConstHandle<RC::Object> const &klBindingsAST ) const;
      RC::ConstHandle<RC::Object> getKLBindingsAST() const;
      
      bool isEquivalentTo( RC::ConstHandle< RT::Desc > const &desc ) const;
      bool isShallow() const;
      
      std::string const &desc() const
      {
        return m_name;
      }
      
      virtual RC::Handle<JSON::Object> jsonDesc() const;
      
    protected:
    
      Desc( std::string const &name, RC::ConstHandle<Impl> const &impl );
      
    private:
    
      std::string m_name;
      RC::ConstHandle<Impl> m_impl;
      
      mutable RC::ConstHandle<RC::Object> m_klBindingsAST;
    };
  };
  
  inline std::string _( RC::ConstHandle<RT::Desc> const &desc )
  {
    return desc->desc();
  }
};

#endif // _FABRIC_RT_DESC_H
