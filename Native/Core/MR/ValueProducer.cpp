/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ValueProducer.h>
#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_MR_OBJECT_GET_CLASS_IMPL( ValueProducer, Producer );
    
    ValueProducer::ValueProducer(
      Object::Class const *myClass,
      RC::ConstHandle<RT::Desc> const &valueDesc
      )
      : Producer( myClass )
      , m_valueDesc( valueDesc )
    {
    }

    RC::ConstHandle<RT::Desc> ValueProducer::getValueDesc() const
    {
      return m_valueDesc;
    }
  };
};
