/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ValueProducer.h>
#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_GET_CLASS_IMPL( ValueProducer, Producer );
    
    ValueProducer::ValueProducer(
      GC::Object::Class const *myClass,
      GC::Container *container,
      std::string const &id_,
      RC::ConstHandle<RT::Desc> const &valueDesc
      )
      : Producer( myClass, container, id_ )
      , m_valueDesc( valueDesc )
    {
    }

    RC::ConstHandle<RT::Desc> ValueProducer::getValueDesc() const
    {
      return m_valueDesc;
    }
  };
};
