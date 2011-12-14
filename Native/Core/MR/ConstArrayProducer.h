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
    class ConstArrayProducer : public ArrayProducer
    {
      FABRIC_GC_OBJECT_GET_CLASS_DECL()
      
      // Virtual functions: ArrayProducer
    
    public:
    
      static RC::Handle<ConstArrayProducer> Create(
        GC::Container *container,
        std::string const &id_,
        RC::ConstHandle<RT::Manager> const &rtManager,
        RC::ConstHandle<RT::Desc> const &elementDesc,
        RC::ConstHandle<JSON::Array> const &jsonArray
        );
    
      virtual size_t count() const;
      virtual void produce( size_t index, void *data ) const;
      virtual void produceJSON( size_t index, Util::JSONGenerator &jg ) const;
            
    protected:
    
      ConstArrayProducer(
        GC::Object::Class const *myClass,
        GC::Container *container,
        std::string const &id_,
        RC::ConstHandle<RT::Manager> const &rtManager,
        RC::ConstHandle<RT::Desc> const &elementDesc,
        RC::ConstHandle<JSON::Array> const &jsonArray
        );
      ~ConstArrayProducer();
    
      virtual char const *getKind() const;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const;
    
    private:
    
      RC::ConstHandle<RT::VariableArrayDesc> m_variableArrayDesc;
      std::vector<uint8_t> m_data;
    };
  };
};

#endif //_FABRIC_MR_CONST_ARRAY_PRODUCER_H
