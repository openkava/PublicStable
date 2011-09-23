/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_DESC_H
#define _FABRIC_RT_DESC_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Core/RT/ImplType.h>

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
      
      static RC::ConstHandle<Desc> Create( std::string const &userName, RC::ConstHandle<Impl> const &impl )
      {
        return new Desc( userName, impl );
      }
    
      std::string const &getUserName() const
      {
        return m_userName;
      }
      size_t getAllocSize() const;
      RC::ConstHandle<Impl> getImpl() const;
      
      ImplType getType() const;
      virtual bool isArrayDesc() const { return false; }
      
      void const *getDefaultData() const;
      void setData( void const *value, void *data ) const;
      void disposeData( void *data ) const;
      std::string descData( void const *data ) const;
      std::string toString( void const *data ) const;
      
      RC::Handle<JSON::Value> getJSONValue( void const *data ) const;
      void setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &value, void *data ) const;

      void setKLBindingsAST( RC::ConstHandle<RC::Object> const &klBindingsAST ) const;
      RC::ConstHandle<RC::Object> getKLBindingsAST() const;
      
      bool isEquivalentTo( RC::ConstHandle< RT::Desc > const &desc ) const;
      bool isShallow() const;
      
      virtual RC::Handle<JSON::Object> jsonDesc() const;
      
    protected:
    
      Desc( std::string const &userName, RC::ConstHandle<Impl> const &impl );
      
    private:
    
      std::string m_userName;
      RC::ConstHandle<Impl> m_impl;
      
      mutable RC::ConstHandle<RC::Object> m_klBindingsAST;
    };
  };
};

#endif // _FABRIC_RT_DESC_H
