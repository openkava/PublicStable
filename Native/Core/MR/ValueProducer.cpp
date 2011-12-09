/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ValueProducer.h>
#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace MR
  {
    bool ValueProducer::isValueProducer() const
    {
      return true;
    }

    RC::ConstHandle<RT::Desc> ValueProducer::getValueDesc() const
    {
      return m_valueDesc;
    }
    
    ValueProducer::ValueProducer( RC::ConstHandle<RT::Desc> const &valueDesc )
      : m_valueDesc( valueDesc )
    {
    }
  };
};
