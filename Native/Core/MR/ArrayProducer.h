/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_ARRAY_PRODUCER_H
#define _FABRIC_MR_ARRAY_PRODUCER_H

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
    class ArrayProducer : public Producer
    {
      FABRIC_GC_OBJECT_GET_CLASS_DECL()
      
      // Virtual functions: ArrayProducer
    
    public:
    
      virtual size_t count() const = 0;
      virtual void produce( size_t index, void *data ) const = 0;
            
      // Non-virtual functions
      
    public:
    
      RC::ConstHandle<RT::Desc> getElementDesc() const;
    
    protected:
    
      ArrayProducer( GC::Object::Class const *myClass, GC::Container *container, RC::ConstHandle<RT::Desc> const &elementDesc );
    
    private:
    
      RC::ConstHandle<RT::Desc> m_elementDesc;
    };
  };
};

#endif //_FABRIC_MR_ARRAY_PRODUCER_H
