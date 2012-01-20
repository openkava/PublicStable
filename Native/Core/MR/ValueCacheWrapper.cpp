/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ValueCacheWrapper.h>
#include <Fabric/Core/MR/ValueCache.h>
#include <Fabric/Core/MR/ValueProducer.h>
#include <Fabric/Core/MR/ValueProducerWrapper.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ValueCacheWrapper, ValueProducerWrapper );
    
    RC::Handle<ValueCacheWrapper> ValueCacheWrapper::Create(
      RC::ConstHandle<ValueProducerWrapper> const &inputValueProducer
      )
    {
      return new ValueCacheWrapper( FABRIC_GC_OBJECT_MY_CLASS, inputValueProducer );
    }
    
    ValueCacheWrapper::ValueCacheWrapper(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<ValueProducerWrapper> const &inputValueProducer
      )
      : ValueProducerWrapper( FABRIC_GC_OBJECT_CLASS_ARG )
      , m_inputValueProducer( inputValueProducer )
      , m_unwrapped(
        ValueCache::Create(
          inputValueProducer->getUnwrapped()
          )
        )
    {
    }
      
    RC::ConstHandle<ValueProducer> ValueCacheWrapper::getUnwrapped() const
    {
      return m_unwrapped;
    }

    char const *ValueCacheWrapper::getKind() const
    {
      return "ValueCache";
    }
    
    void ValueCacheWrapper::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      {
        Util::JSONGenerator jg = jog.makeMember( "input" );
        m_inputValueProducer->toJSON( jg );
      }
    }
  }
}
