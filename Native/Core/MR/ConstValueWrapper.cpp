/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ConstValue.h>
#include <Fabric/Core/MR/ConstValueWrapper.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ConstValueWrapper, ValueProducerWrapper );
    
    RC::Handle<ConstValueWrapper> ConstValueWrapper::Create(
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<RT::Desc> const &valueDesc,
      RC::ConstHandle<JSON::Value> const &jsonValue
      )
    {
      return new ConstValueWrapper( FABRIC_GC_OBJECT_MY_CLASS, rtManager, valueDesc, jsonValue );
    }
    
    ConstValueWrapper::ConstValueWrapper(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<RT::Desc> const &valueDesc,
      RC::ConstHandle<JSON::Value> const &jsonValue
      )
      : ValueProducerWrapper( FABRIC_GC_OBJECT_CLASS_ARG )
      , m_unwrapped(
        ConstValue::Create(
          rtManager,
          valueDesc,
          jsonValue
        )
      )
    {
    }
      
    RC::ConstHandle<ValueProducer> ConstValueWrapper::getUnwrapped() const
    {
      return m_unwrapped;
    }

    char const *ConstValueWrapper::getKind() const
    {
      return "ConstValue";
    }
    
    void ConstValueWrapper::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      Util::JSONGenerator jg = jog.makeMember( "data" );
      m_unwrapped->getValueDesc()->generateJSON( m_unwrapped->getImmutableData(), jg );
    }
  }
}
