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
      // andrewmacp 2012-01-19:
      // It wouldn't be incorrect to do this without a mutex but it's
      // clearer and simpler to leave one in, we may remove later.
      {
        Util::Mutex::Lock mutexLock( m_mutex );
        if ( m_cacheCountExists )
          return m_cacheCount;
      }

      {
        Util::Mutex::Lock mutexLock( m_mutex );
        m_cacheCount = m_inputArrayProducer->getCount();
        m_cacheCountExists = true;
        return m_cacheCount;
      }
    }
      
    const RC::Handle<ArrayProducer::ComputeState> ArrayCache::createComputeState() const
    {
      return ComputeState::Create( this );
    }

    void ArrayCache::flush()
    {
      if ( m_cacheData.size() > 0 )
      {
        Util::Mutex::Lock mutexLock( m_mutex );
        m_inputElementDesc->disposeDatas( &m_cacheData[0], getCount(), m_inputElementDesc->getAllocSize() );
        m_cacheData.resize(0);
        m_cacheDataExists.resize(0);
        m_cacheCountExists = false;
      }
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
      {
        Util::Mutex::Lock mutexLock( m_mutex );
        if ( m_arrayCache->m_cacheDataExists.size() > index )
          if ( m_arrayCache->m_cacheDataExists[ index ] )
            m_arrayCache->m_inputElementDesc->setData( &m_arrayCache->m_cacheData[ index * m_arrayCache->m_inputElementDesc->getAllocSize() ], data );
      }

      m_inputArrayProducerComputeState->produce( index, data );

      {
        Util::Mutex::Lock mutexLock( m_mutex );
        size_t arraySize = getCount();
        size_t allocSize = m_arrayCache->m_inputElementDesc->getAllocSize();

        if ( m_arrayCache->m_cacheDataExists.size() < 1 )
        {
          m_arrayCache->m_cacheData.resize( allocSize * arraySize, 0 );
          m_arrayCache->m_cacheDataExists.resize( arraySize );
        }

        if ( index < arraySize )
        {
          m_arrayCache->m_inputElementDesc->setData( data, &m_arrayCache->m_cacheData[ index * allocSize ] );
          m_arrayCache->m_cacheDataExists[ index ] = true;
        }
      }
    }
  };
};
