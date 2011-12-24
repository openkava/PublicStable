/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ArrayMap.h>
#include <Fabric/Core/MR/ValueProducer.h>
#include <Fabric/Core/KLC/ArrayMapOperator.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ArrayMap, ArrayProducer );
    
    RC::Handle<ArrayMap> ArrayMap::Create(
      RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
      RC::ConstHandle<KLC::ArrayMapOperator> const &mapOperator,
      RC::ConstHandle<ValueProducer> const &sharedValueProducer
      )
    {
      return new ArrayMap( FABRIC_GC_OBJECT_MY_CLASS, inputArrayProducer, mapOperator, sharedValueProducer );
    }
    
    ArrayMap::ArrayMap(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
      RC::ConstHandle<KLC::ArrayMapOperator> const &mapOperator,
      RC::ConstHandle<ValueProducer> const &sharedValueProducer
      )
      : ArrayProducer( FABRIC_GC_OBJECT_CLASS_ARG, mapOperator->getOutputDesc() )
      , m_inputArrayProducer( inputArrayProducer )
      , m_mapOperator( mapOperator )
      , m_sharedValueProducer( sharedValueProducer )
    {
      RC::ConstHandle<RT::Desc> inputArrayProducerElementDesc = inputArrayProducer->getElementDesc();
      if ( !inputArrayProducerElementDesc )
        throw Exception("input array producer is invalid");
      RC::ConstHandle<RT::Desc> mapOperatorInputDesc = mapOperator->getInputDesc();
      if ( !mapOperatorInputDesc )
        throw Exception("map operator is invalid");
      if ( !mapOperatorInputDesc->isEquivalentTo( inputArrayProducerElementDesc ) )
        throw Exception(
          "input element type ("
          + _(inputArrayProducerElementDesc->getUserName())
          + ") is not equivalent to map operator input type ("
          + _(mapOperatorInputDesc->getUserName()) + ")"
          );
      RC::ConstHandle<RT::Desc> reduceOperatorSharedDesc = mapOperator->getSharedDesc();
      if ( reduceOperatorSharedDesc )
      {
        RC::ConstHandle<RT::Desc> sharedValueProducerValueDesc = sharedValueProducer->getValueDesc();
        if ( !sharedValueProducerValueDesc )
          throw Exception( "map operator requires a shared value but no shared value producer is provided" );
        if ( !sharedValueProducerValueDesc->isEquivalentTo( reduceOperatorSharedDesc ) )
          throw Exception(
            "shared value type ("
            + _(sharedValueProducerValueDesc->getUserName())
            + ") is not equivalent to map operator shared type ("
            + _(reduceOperatorSharedDesc->getUserName()) + ")"
            );
      }
    }
    
    ArrayMap::~ArrayMap()
    {
    }

    char const *ArrayMap::getKind() const
    {
      return "ArrayMap";
    }
    
    void ArrayMap::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      {
        Util::JSONGenerator jg = jog.makeMember( "inputArrayProvider" );
        m_inputArrayProducer->toJSON( jg );
      }

      {
        Util::JSONGenerator jg = jog.makeMember( "mapOperator" );
        m_mapOperator->toJSON( jg );
      }
      
      if ( m_sharedValueProducer )
      {
        Util::JSONGenerator jg = jog.makeMember( "sharedValueProducer" );
        m_sharedValueProducer->toJSON( jg );
      }
    }

    size_t ArrayMap::count() const
    {
      return m_inputArrayProducer->count();
    }
    
    void ArrayMap::produce( size_t index, void *data ) const
    {
      RC::ConstHandle<RT::Desc> inputElementDesc = m_inputArrayProducer->getElementDesc();
      
      size_t elementSize = inputElementDesc->getAllocSize();
      void *inputData = alloca( elementSize );
      memset( inputData, 0, elementSize );
      m_inputArrayProducer->produce( index, inputData );
      
      if ( m_mapOperator->takesIndex() )
      {
        if ( m_mapOperator->takesCount() )
        {
          if ( m_mapOperator->takesSharedValue() )
          {
            RC::ConstHandle<RT::Desc> sharedDesc = m_sharedValueProducer->getValueDesc();
            size_t sharedDataSize = sharedDesc->getAllocSize();
            void *sharedData = alloca( sharedDataSize );
            memset( sharedData, 0, sharedDataSize );
            
            m_sharedValueProducer->produce( sharedData );

            m_mapOperator->call( inputData, data, index, count(), sharedData );
            
            sharedDesc->disposeData( sharedData );
          }
          else m_mapOperator->call( inputData, data, index, count() );
        }
        else m_mapOperator->call( inputData, data, index );
      }
      else m_mapOperator->call( inputData, data );
      
      inputElementDesc->disposeData( inputData );
    }
  };
};
