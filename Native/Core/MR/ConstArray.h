/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_CONST_ARRAY_PRODUCER_H
#define _FABRIC_MR_CONST_ARRAY_PRODUCER_H

#include <Fabric/Core/MR/ArrayProducer.h>

#include <vector>
#include <stdint.h>

namespace Fabric
{
  namespace JSON
  {
    class Array;
  };
  
  namespace RT
  {
    class Manager;
    class VariableArrayDesc;
  };
  
  namespace MR
  {
    class ConstArray : public ArrayProducer
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
    public:
    
      static RC::Handle<ConstArray> Create(
        RC::ConstHandle<RT::Manager> const &rtManager,
        RC::ConstHandle<RT::Desc> const &elementDesc,
        RC::ConstHandle<JSON::Array> const &jsonArray
        );
      
      // Virtual functions: ArrayProducer
    
    public:
    
      virtual size_t count() const;
      virtual void produce( size_t index, void *data ) const;
      virtual void produceJSON( size_t index, Util::JSONGenerator &jg ) const;
            
    protected:
    
      ConstArray(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<RT::Manager> const &rtManager,
        RC::ConstHandle<RT::Desc> const &elementDesc,
        RC::ConstHandle<JSON::Array> const &jsonArray
        );
      ~ConstArray();
    
      virtual char const *getKind() const;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const;
    
    private:
    
      RC::ConstHandle<RT::VariableArrayDesc> m_variableArrayDesc;
      std::vector<uint8_t> m_data;
    };
  };
};

#endif //_FABRIC_MR_CONST_ARRAY_PRODUCER_H
