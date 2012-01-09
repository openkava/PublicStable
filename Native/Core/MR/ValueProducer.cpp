/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ValueProducer.h>
#include <Fabric/Core/RT/Desc.h>

#include <alloca.h>
#include <string.h>

namespace Fabric
{
  namespace MR
  {
    ValueProducer::ValueProducer()
    {
    }
    
    ValueProducer::ComputeState::ComputeState( RC::ConstHandle<ValueProducer> const &valueProducer )
      : m_valueProducer( valueProducer )
    {
    }
    
    void ValueProducer::ComputeState::produceJSON( Util::JSONGenerator &jg ) const
    {
      RC::ConstHandle<RT::Desc> valueDesc = m_valueProducer->getValueDesc();
      
      size_t allocSize = valueDesc->getAllocSize();
      void *valueData = alloca( allocSize );
      memset( valueData, 0, allocSize );
      produce( valueData );
      valueDesc->generateJSON( valueData, jg );
      valueDesc->disposeData( valueData );
    }
  };
};
