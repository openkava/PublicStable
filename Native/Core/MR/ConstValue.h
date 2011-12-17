/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_CONST_VALUE_H
#define _FABRIC_MR_CONST_VALUE_H

#include <Fabric/Core/MR/ValueProducer.h>

namespace Fabric
{
  namespace RT
  {
    class Manager;
  };
  
  namespace MR
  {
    class ConstValue : public ValueProducer
    {
      FABRIC_GC_OBJECT_CLASS_DECL()

    public:
    
      static RC::Handle<ConstValue> Create(
        RC::ConstHandle<RT::Manager> const &rtManager,
        RC::ConstHandle<RT::Desc> const &elementDesc,
        RC::ConstHandle<JSON::Value> const &jsonValue
        );
      
      // Virtual functions: ValueProducer
    
    public:
    
      virtual void produce( void *data ) const;
      virtual void produceJSON( Util::JSONGenerator &jg ) const;
      
    protected:
    
      ConstValue(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<RT::Manager> const &rtManager,
        RC::ConstHandle<RT::Desc> const &elementDesc,
        RC::ConstHandle<JSON::Value> const &jsonValue
        );
      ~ConstValue();
    
      virtual char const *getKind() const;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const;
    
    private:
    
      std::vector<uint8_t> m_data;
    };
  };
};

#endif //_FABRIC_MR_CONST_VALUE_H
