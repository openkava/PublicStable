/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/Map.h>
#include <Fabric/Core/KLC/MapOperator.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( Map, ArrayProducer );
    
    RC::Handle<Map> Map::Create(
      RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
      RC::ConstHandle<KLC::MapOperator> const &mapOperator
      )
    {
      return new Map( FABRIC_GC_OBJECT_MY_CLASS, inputArrayProducer, mapOperator );
    }
    
    Map::Map(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
      RC::ConstHandle<KLC::MapOperator> const &mapOperator
      )
      : ArrayProducer( FABRIC_GC_OBJECT_CLASS_ARG, mapOperator->getOutputDesc() )
      , m_inputArrayProducer( inputArrayProducer )
      , m_mapOperator( mapOperator )
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
    }
    
    Map::~Map()
    {
    }

    char const *Map::getKind() const
    {
      return "Map";
    }
    
    void Map::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      {
        Util::JSONGenerator jg = jog.makeMember( "inputArrayProvider" );
        m_inputArrayProducer->toJSON( jg );
      }

      {
        Util::JSONGenerator jg = jog.makeMember( "mapOperator" );
        m_mapOperator->toJSON( jg );
      }
    }

    size_t Map::count() const
    {
      return m_inputArrayProducer->count();
    }
    
    void Map::produce( size_t index, void *data ) const
    {
      size_t elementSize = getElementDesc()->getAllocSize();
      void *inputData = alloca( elementSize );
      memset( inputData, 0, elementSize );
      m_inputArrayProducer->produce( index, inputData );
      
      m_mapOperator->call( index, inputData, data );
    }
  };
};
