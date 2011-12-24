/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ConstValue.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ConstValue, ValueProducer );
    
    RC::Handle<ConstValue> ConstValue::Create(
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<RT::Desc> const &valueDesc,
      RC::ConstHandle<JSON::Value> const &jsonValue
      )
    {
      return new ConstValue( FABRIC_GC_OBJECT_MY_CLASS, rtManager, valueDesc, jsonValue );
    }
    
    ConstValue::ConstValue(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<RT::Desc> const &valueDesc,
      RC::ConstHandle<JSON::Value> const &jsonValue
      )
      : ValueProducer( FABRIC_GC_OBJECT_CLASS_ARG, valueDesc )
    {
      m_data.resize( valueDesc->getAllocSize(), 0 );
      valueDesc->setDataFromJSONValue( jsonValue, &m_data[0] );
    }
    
    ConstValue::~ConstValue()
    {
      getValueDesc()->disposeData( &m_data[0] );
    }

    char const *ConstValue::getKind() const
    {
      return "ConstValue";
    }
    
    void ConstValue::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      Util::JSONGenerator jg = jog.makeMember( "data" );
      getValueDesc()->generateJSON( &m_data[0], jg );
    }
    
    const RC::Handle<ValueProducer::ComputeState> ConstValue::createComputeState() const
    {
      return ComputeState::Create( this );
    }
    
    RC::Handle<ConstValue::ComputeState> ConstValue::ComputeState::Create( RC::ConstHandle<ConstValue> const &constValue )
    {
      return new ComputeState( constValue );
    }
    
    ConstValue::ComputeState::ComputeState( RC::ConstHandle<ConstValue> const &constValue )
      : ValueProducer::ComputeState( constValue )
      , m_constValue( constValue )
    {
    }
    
    ConstValue::ComputeState::~ComputeState()
    {
    }
    
    void ConstValue::ComputeState::produce( void *data ) const
    {
      return m_constValue->getValueDesc()->setData( &m_constValue->m_data[0], data );
    }
    
    void ConstValue::ComputeState::produceJSON( Util::JSONGenerator &jg ) const
    {
      return m_constValue->getValueDesc()->generateJSON( &m_constValue->m_data[0], jg );
    }
  }
}
