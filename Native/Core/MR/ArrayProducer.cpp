/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ArrayProducer.h>
#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace MR
  {
    bool ArrayProducer::isArrayProducer() const
    {
      return true;
    }

    RC::ConstHandle<RT::Desc> ArrayProducer::getElementDesc() const
    {
      return m_elementDesc;
    }
    
    ArrayProducer::ArrayProducer( RC::ConstHandle<RT::Desc> const &elementDesc )
      : m_elementDesc( elementDesc )
    {
    }
  };
};
