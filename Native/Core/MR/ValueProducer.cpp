/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ValueProducer.h>
#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ValueProducer, Producer );
    
    ValueProducer::ValueProducer(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<RT::Desc> const &valueDesc
      )
      : Producer( FABRIC_GC_OBJECT_CLASS_ARG )
      , m_valueDesc( valueDesc )
    {
    }

    RC::ConstHandle<RT::Desc> ValueProducer::getValueDesc() const
    {
      return m_valueDesc;
    }
  };
};
