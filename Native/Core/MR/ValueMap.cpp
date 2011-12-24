/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ValueMap.h>
#include <Fabric/Core/KLC/ValueMapOperator.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ValueMap, ValueProducer );
    
    RC::Handle<ValueMap> ValueMap::Create(
      RC::ConstHandle<ValueProducer> const &inputValueProducer,
      RC::ConstHandle<KLC::ValueMapOperator> const &valueMapOperator,
      RC::ConstHandle<ValueProducer> const &sharedValueProducer
      )
    {
      return new ValueMap( FABRIC_GC_OBJECT_MY_CLASS, inputValueProducer, valueMapOperator, sharedValueProducer );
    }
    
    ValueMap::ValueMap(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<ValueProducer> const &inputValueProducer,
      RC::ConstHandle<KLC::ValueMapOperator> const &valueMapOperator,
      RC::ConstHandle<ValueProducer> const &sharedValueProducer
      )
      : ValueProducer( FABRIC_GC_OBJECT_CLASS_ARG, valueMapOperator->getOutputDesc() )
      , m_inputValueProducer( inputValueProducer )
      , m_valueMapOperator( valueMapOperator )
      , m_sharedValueProducer( sharedValueProducer )
    {
      RC::ConstHandle<RT::Desc> inputValueProducerValueDesc = inputValueProducer->getValueDesc();
      if ( !inputValueProducerValueDesc )
        throw Exception("input value producer is invalid");
      RC::ConstHandle<RT::Desc> valueMapOperatorInputDesc = valueMapOperator->getInputDesc();
      if ( !valueMapOperatorInputDesc )
        throw Exception("value map operator is invalid");
      if ( !valueMapOperatorInputDesc->isEquivalentTo( inputValueProducerValueDesc ) )
        throw Exception(
          "input value type ("
          + _(inputValueProducerValueDesc->getUserName())
          + ") is not equivalent to value map operator input type ("
          + _(valueMapOperatorInputDesc->getUserName()) + ")"
          );
      RC::ConstHandle<RT::Desc> valueMapOperatorSharedDesc = valueMapOperator->getSharedDesc();
      if ( valueMapOperatorSharedDesc )
      {
        RC::ConstHandle<RT::Desc> sharedValueProducerValueDesc = sharedValueProducer->getValueDesc();
        if ( !sharedValueProducerValueDesc )
          throw Exception( "value map operator requires a shared value but no shared value producer is provided" );
        if ( !sharedValueProducerValueDesc->isEquivalentTo( valueMapOperatorSharedDesc ) )
          throw Exception(
            "shared value type ("
            + _(sharedValueProducerValueDesc->getUserName())
            + ") is not equivalent to value map operator shared type ("
            + _(valueMapOperatorSharedDesc->getUserName()) + ")"
            );
      }
    }
    
    ValueMap::~ValueMap()
    {
    }

    char const *ValueMap::getKind() const
    {
      return "ValueMap";
    }
    
    void ValueMap::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      {
        Util::JSONGenerator jg = jog.makeMember( "inputValueProvider" );
        m_inputValueProducer->toJSON( jg );
      }

      {
        Util::JSONGenerator jg = jog.makeMember( "valueMapOperator" );
        m_valueMapOperator->toJSON( jg );
      }
      
      if ( m_sharedValueProducer )
      {
        Util::JSONGenerator jg = jog.makeMember( "sharedValueProducer" );
        m_sharedValueProducer->toJSON( jg );
      }
    }
      
    const RC::Handle<ValueProducer::ComputeState> ValueMap::createComputeState() const
    {
      return ComputeState::Create( this );
    }
    
    RC::Handle<ValueMap::ComputeState> ValueMap::ComputeState::Create( RC::ConstHandle<ValueMap> const &valueMap )
    {
      return new ComputeState( valueMap );
    }
    
    ValueMap::ComputeState::ComputeState( RC::ConstHandle<ValueMap> const &valueMap )
      : ValueProducer::ComputeState( valueMap )
      , m_inputComputeState( valueMap->m_inputValueProducer->createComputeState() )
      , m_inputDesc( valueMap->getValueDesc() )
      , m_operator( valueMap->m_valueMapOperator )
      , m_shared( valueMap->m_sharedValueProducer )
    {
      if ( m_operator->takesSharedValue() )
      {
        m_sharedData.resize( m_shared->getValueDesc()->getAllocSize(), 0 );
        m_shared->createComputeState()->produce( &m_sharedData[0] );
      }
    }
    
    ValueMap::ComputeState::~ComputeState()
    {
      if ( m_operator->takesSharedValue() )
        m_shared->getValueDesc()->disposeData( &m_sharedData[0] );
    }
    
    void ValueMap::ComputeState::produce( void *data ) const
    {
      size_t elementSize = m_inputDesc->getAllocSize();
      void *inputData = alloca( elementSize );
      memset( inputData, 0, elementSize );
      m_inputComputeState->produce( inputData );
      
      if ( m_operator->takesSharedValue() )
        m_operator->call( inputData, data, &m_sharedData[0] );
      else m_operator->call( inputData, data );
      
      m_inputDesc->disposeData( inputData );
    }
  };
};
