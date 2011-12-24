/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ValueTransform.h>
#include <Fabric/Core/KLC/ValueTransformOperator.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ValueTransform, ValueProducer );
    
    RC::Handle<ValueTransform> ValueTransform::Create(
      RC::ConstHandle<ValueProducer> const &inputValueProducer,
      RC::ConstHandle<KLC::ValueTransformOperator> const &valueTransformOperator,
      RC::ConstHandle<ValueProducer> const &sharedValueProducer
      )
    {
      return new ValueTransform( FABRIC_GC_OBJECT_MY_CLASS, inputValueProducer, valueTransformOperator, sharedValueProducer );
    }
    
    ValueTransform::ValueTransform(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<ValueProducer> const &inputValueProducer,
      RC::ConstHandle<KLC::ValueTransformOperator> const &valueTransformOperator,
      RC::ConstHandle<ValueProducer> const &sharedValueProducer
      )
      : ValueProducer( FABRIC_GC_OBJECT_CLASS_ARG, valueTransformOperator->getValueDesc() )
      , m_inputValueProducer( inputValueProducer )
      , m_valueTransformOperator( valueTransformOperator )
      , m_sharedValueProducer( sharedValueProducer )
    {
      RC::ConstHandle<RT::Desc> inputValueProducerValueDesc = inputValueProducer->getValueDesc();
      if ( !inputValueProducerValueDesc )
        throw Exception("input value producer is invalid");
      RC::ConstHandle<RT::Desc> valueTransformOperatorValueDesc = valueTransformOperator->getValueDesc();
      if ( !valueTransformOperatorValueDesc )
        throw Exception("value transform operator is invalid");
      if ( !valueTransformOperatorValueDesc->isEquivalentTo( inputValueProducerValueDesc ) )
        throw Exception(
          "input value type ("
          + _(inputValueProducerValueDesc->getUserName())
          + ") is not equivalent to value transform operator value type ("
          + _(valueTransformOperatorValueDesc->getUserName()) + ")"
          );
      RC::ConstHandle<RT::Desc> valueTransformOperatorSharedDesc = valueTransformOperator->getSharedDesc();
      if ( valueTransformOperatorSharedDesc )
      {
        RC::ConstHandle<RT::Desc> sharedValueProducerValueDesc = sharedValueProducer->getValueDesc();
        if ( !sharedValueProducerValueDesc )
          throw Exception( "value map operator requires a shared value but no shared value producer is provided" );
        if ( !sharedValueProducerValueDesc->isEquivalentTo( valueTransformOperatorSharedDesc ) )
          throw Exception(
            "shared value type ("
            + _(sharedValueProducerValueDesc->getUserName())
            + ") is not equivalent to value map operator shared type ("
            + _(valueTransformOperatorSharedDesc->getUserName()) + ")"
            );
      }
    }
    
    ValueTransform::~ValueTransform()
    {
    }

    char const *ValueTransform::getKind() const
    {
      return "ValueTransform";
    }
    
    void ValueTransform::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      {
        Util::JSONGenerator jg = jog.makeMember( "inputValueProvider" );
        m_inputValueProducer->toJSON( jg );
      }

      {
        Util::JSONGenerator jg = jog.makeMember( "valueTransformOperator" );
        m_valueTransformOperator->toJSON( jg );
      }
      
      if ( m_sharedValueProducer )
      {
        Util::JSONGenerator jg = jog.makeMember( "sharedValueProducer" );
        m_sharedValueProducer->toJSON( jg );
      }
    }
      
    const RC::Handle<ValueProducer::ComputeState> ValueTransform::createComputeState() const
    {
      return ComputeState::Create( this );
    }
    
    RC::Handle<ValueTransform::ComputeState> ValueTransform::ComputeState::Create( RC::ConstHandle<ValueTransform> const &valueTransform )
    {
      return new ComputeState( valueTransform );
    }
    
    ValueTransform::ComputeState::ComputeState( RC::ConstHandle<ValueTransform> const &valueTransform )
      : ValueProducer::ComputeState( valueTransform )
      , m_valueTransform( valueTransform )
      , m_inputValueProducerComputeState( valueTransform->m_inputValueProducer->createComputeState() )
    {
      if ( m_valueTransform->m_valueTransformOperator->takesSharedValue() )
      {
        RC::ConstHandle<ValueProducer> sharedValueProducer = m_valueTransform->m_sharedValueProducer;
        m_sharedData.resize( sharedValueProducer->getValueDesc()->getAllocSize(), 0 );
        sharedValueProducer->createComputeState()->produce( &m_sharedData[0] );
      }
    }
    
    ValueTransform::ComputeState::~ComputeState()
    {
      if ( m_valueTransform->m_valueTransformOperator->takesSharedValue() )
      {
        RC::ConstHandle<ValueProducer> sharedValueProducer = m_valueTransform->m_sharedValueProducer;
        sharedValueProducer->getValueDesc()->disposeData( &m_sharedData[0] );
      }
    }
    
    void ValueTransform::ComputeState::produce( void *data ) const
    {
      m_inputValueProducerComputeState->produce( data );
      
      RC::ConstHandle<KLC::ValueTransformOperator> valueTransformOperator = m_valueTransform->m_valueTransformOperator;
      if ( valueTransformOperator->takesSharedValue() )
        valueTransformOperator->call( data, &m_sharedData[0] );
      else valueTransformOperator->call( data );
    }
  };
};
