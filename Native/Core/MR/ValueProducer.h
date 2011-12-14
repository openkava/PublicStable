/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_VALUE_PRODUCER_H
#define _FABRIC_MR_VALUE_PRODUCER_H

#include <Fabric/Core/MR/Producer.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace RT
  {
    class Desc;
  };
  
  namespace MR
  {
    class ValueProducer : public Producer
    {
      FABRIC_GC_OBJECT_GET_CLASS_DECL()
      
      // Virtual functions: ValueProducer
    
    public:
    
      virtual void produce( void *data ) const = 0;
      
      // Non-virtual functions
      
    public:
    
      RC::ConstHandle<RT::Desc> getValueDesc() const;
    
    protected:
    
      ValueProducer(
        GC::Object::Class const *myClass,
        GC::Container *container,
        std::string const &id_,
        RC::ConstHandle<RT::Desc> const &valueDesc
        );
    
    private:
    
      RC::ConstHandle<RT::Desc> m_valueDesc;
    };
  };
};

#endif //_FABRIC_MR_VALUE_PRODUCER_H
