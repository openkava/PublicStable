/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ArrayCache.h>
#include <Fabric/Core/MR/ArrayOutputOperator.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace MR
  {
    RC::Handle<ArrayCache> ArrayCache::Create(
      RC::ConstHandle<ArrayProducer> const &inputArrayProducer
      )
    {
      return new ArrayCache( inputArrayProducer );
    }
    
    ArrayCache::ArrayCache(
      RC::ConstHandle<ArrayProducer> const &inputArrayProducer
      )
      : m_inputArrayProducer( inputArrayProducer )
      , m_mutex( "ArrayCache" )
    {
      RC::ConstHandle<RT::Desc> inputElementDesc = inputArrayProducer->getElementDesc();
      if ( !inputElementDesc )
        throw Exception("input is invalid");
      m_inputElementDesc = inputElementDesc;
    }

    ArrayCache::~ArrayCache()
    {
    }
    
    RC::ConstHandle<RT::Desc> ArrayCache::getElementDesc() const
    {
      return m_inputElementDesc;
    }

    size_t ArrayCache::getCount() const
    {
      return m_inputArrayProducer->getCount();
    }
      
    const RC::Handle<ArrayProducer::ComputeState> ArrayCache::createComputeState() const
    {
      return ComputeState::Create( this );
    }

    void ArrayCache::flush()
    {
    }
    
    RC::Handle<ArrayCache::ComputeState> ArrayCache::ComputeState::Create( RC::ConstHandle<ArrayCache> const &arrayCache )
    {
      return new ComputeState( arrayCache );
    }
    
    ArrayCache::ComputeState::ComputeState( RC::ConstHandle<ArrayCache> const &arrayCache )
      : ArrayProducer::ComputeState( arrayCache )
      , m_arrayCache( arrayCache )
      , m_mutex( arrayCache->m_mutex )
      , m_inputArrayProducerComputeState( arrayCache->m_inputArrayProducer->createComputeState() )
    {
    }
    
    void ArrayCache::ComputeState::produce( size_t index, void *data ) const
    {
      m_inputArrayProducerComputeState->produce( index, data );
    }
  };
};
