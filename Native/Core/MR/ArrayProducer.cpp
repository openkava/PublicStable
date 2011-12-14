/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ArrayProducer.h>
#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_GET_CLASS_IMPL( ArrayProducer, Producer );
    
    ArrayProducer::ArrayProducer(
      GC::Object::Class const *myClass,
      GC::Container *container,
      std::string const &id_,
      RC::ConstHandle<RT::Desc> const &elementDesc
      )
      : Producer( myClass, container, id_ )
      , m_elementDesc( elementDesc )
    {
    }

    RC::ConstHandle<RT::Desc> ArrayProducer::getElementDesc() const
    {
      return m_elementDesc;
    }
  };
};
