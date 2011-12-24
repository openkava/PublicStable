/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ValueProducer.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ValueProducer, Producer );
    
    ValueProducer::ValueProducer(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<RT::Desc> const &valueDesc
      )
      : Producer( FABRIC_GC_OBJECT_CLASS_ARG )
      , m_valueDesc( valueDesc )
    {
    }

    RC::ConstHandle<RT::Desc> ValueProducer::getValueDesc() const
    {
      return m_valueDesc;
    }
    
    void ValueProducer::jsonExec(
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( cmd == "produce" )
        jsonExecProduce( arg, resultJAG );
      else Producer::jsonExec( cmd, arg, resultJAG );
    }
    
    void ValueProducer::jsonExecProduce(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      Util::JSONGenerator jg = resultJAG.makeElement();
      createComputeState()->produceJSON( jg );
    }
    
    ValueProducer::ComputeState::ComputeState( RC::ConstHandle<ValueProducer> const &valueProducer )
      : m_valueProducer( valueProducer )
    {
    }
    
    void ValueProducer::ComputeState::produceJSON( Util::JSONGenerator &jg ) const
    {
      RC::ConstHandle<RT::Desc> valueDesc = m_valueProducer->m_valueDesc;
      
      size_t allocSize = valueDesc->getAllocSize();
      void *valueData = alloca( allocSize );
      memset( valueData, 0, allocSize );
      produce( valueData );
      valueDesc->generateJSON( valueData, jg );
      valueDesc->disposeData( valueData );
    }
  };
};
