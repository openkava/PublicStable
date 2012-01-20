/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ArrayCacheWrapper.h>
#include <Fabric/Core/MR/ArrayCache.h>
#include <Fabric/Core/MR/ArrayProducerWrapper.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ArrayCacheWrapper, ArrayProducerWrapper );
    
    RC::Handle<ArrayCacheWrapper> ArrayCacheWrapper::Create(
      RC::ConstHandle<ArrayProducerWrapper> const &inputArrayProducer
      )
    {
      return new ArrayCacheWrapper( FABRIC_GC_OBJECT_MY_CLASS, inputArrayProducer );
    }
    
    ArrayCacheWrapper::ArrayCacheWrapper(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<ArrayProducerWrapper> const &inputArrayProducer
      )
      : ArrayProducerWrapper( FABRIC_GC_OBJECT_CLASS_ARG )
      , m_inputArrayProducer( inputArrayProducer )
      , m_unwrapped(
        ArrayCache::Create(
          inputArrayProducer->getUnwrapped()
          )
        )
    {
    }
      
    RC::ConstHandle<ArrayProducer> ArrayCacheWrapper::getUnwrapped() const
    {
      return m_unwrapped;
    }

    char const *ArrayCacheWrapper::getKind() const
    {
      return "ArrayCache";
    }
    
    void ArrayCacheWrapper::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      {
        Util::JSONGenerator jg = jog.makeMember( "input" );
        m_inputArrayProducer->toJSON( jg );
      }
    }
  }
}
