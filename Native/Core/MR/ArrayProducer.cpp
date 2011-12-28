/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ArrayProducer.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace MR
  {
    ArrayProducer::ArrayProducer()
    {
    }
    
    ArrayProducer::ComputeState::ComputeState( RC::ConstHandle<ArrayProducer> const &arrayProducer )
      : m_arrayProducer( arrayProducer )
      , m_count( arrayProducer->getCount() )
    {
    }
      
    void ArrayProducer::ComputeState::produceJSON( size_t index, Util::JSONGenerator &jg ) const
    {
      RC::ConstHandle<RT::Desc> elementDesc = m_arrayProducer->getElementDesc();
      
      size_t allocSize = elementDesc->getAllocSize();
      void *valueData = alloca( allocSize );
      memset( valueData, 0, allocSize );
      produce( index, valueData );
      elementDesc->generateJSON( valueData, jg );
      elementDesc->disposeData( valueData );
    }
  }
}
