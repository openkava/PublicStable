/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/Reduce.h>
#include <Fabric/Core/MR/ArrayProducer.h>
#include <Fabric/Core/KLC/ReduceOperator.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( Reduce, ValueProducer );
    
    RC::Handle<Reduce> Reduce::Create(
      RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
      RC::ConstHandle<KLC::ReduceOperator> const &reduceOperator
      )
    {
      return new Reduce( FABRIC_GC_OBJECT_MY_CLASS, inputArrayProducer, reduceOperator );
    }
    
    Reduce::Reduce(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
      RC::ConstHandle<KLC::ReduceOperator> const &reduceOperator
      )
      : ValueProducer( FABRIC_GC_OBJECT_CLASS_ARG, reduceOperator->getOutputDesc() )
      , m_inputArrayProducer( inputArrayProducer )
      , m_reduceOperator( reduceOperator )
    {
      RC::ConstHandle<RT::Desc> inputArrayProducerElementDesc = inputArrayProducer->getElementDesc();
      if ( !inputArrayProducerElementDesc )
        throw Exception("input array producer is invalid");
      RC::ConstHandle<RT::Desc> reduceOperatorInputDesc = reduceOperator->getInputDesc();
      if ( !reduceOperatorInputDesc )
        throw Exception("reduce operator is invalid");
      if ( !reduceOperatorInputDesc->isEquivalentTo( inputArrayProducerElementDesc ) )
        throw Exception(
          "input element type ("
          + _(inputArrayProducerElementDesc->getUserName())
          + ") is not equivalent to reduce operator input type ("
          + _(reduceOperatorInputDesc->getUserName()) + ")"
          );
    }
    
    Reduce::~Reduce()
    {
    }

    char const *Reduce::getKind() const
    {
      return "Reduce";
    }
    
    void Reduce::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      {
        Util::JSONGenerator jg = jog.makeMember( "inputArrayProducer" );
        m_inputArrayProducer->toJSON( jg );
      }
      
      {
        Util::JSONGenerator jg = jog.makeMember( "reduceOperator" );
        m_reduceOperator->toJSON( jg );
      }
    }
    
    void Reduce::produce( void *data ) const
    {
      RC::ConstHandle<RT::Desc> inputElementDesc = m_inputArrayProducer->getElementDesc();
      size_t inputElementSize = inputElementDesc->getAllocSize();
      void *inputData = alloca( inputElementSize );
      memset( inputData, 0, inputElementSize );
      
      size_t count = m_inputArrayProducer->count();
      for ( size_t i=0; i<count; ++i )
      {
        m_inputArrayProducer->produce( i, inputData );
        m_reduceOperator->call( i, inputData, data );
      }
      
      inputElementDesc->disposeData( inputData );
    }
  }
}
