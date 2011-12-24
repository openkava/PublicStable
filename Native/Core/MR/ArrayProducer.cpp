/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ArrayProducer.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ArrayProducer, Producer )
    
    ArrayProducer::ArrayProducer(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<RT::Desc> const &elementDesc
      )
      : Producer( FABRIC_GC_OBJECT_CLASS_ARG )
      , m_elementDesc( elementDesc )
    {
    }

    RC::ConstHandle<RT::Desc> ArrayProducer::getElementDesc() const
    {
      return m_elementDesc;
    }
    
    void ArrayProducer::jsonExec(
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( cmd == "getCount" )
        jsonExecGetCount( arg, resultJAG );
      else if ( cmd == "produce" )
        jsonExecProduce( arg, resultJAG );
      else Producer::jsonExec( cmd, arg, resultJAG );
    }
    
    void ArrayProducer::jsonExecGetCount(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      Util::JSONGenerator jg = resultJAG.makeElement();
      jg.makeInteger( getCount() );
    }
    
    void ArrayProducer::jsonExecProduce(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      size_t index = arg->toInteger()->value();
          
      Util::JSONGenerator jg = resultJAG.makeElement();
      createComputeState()->produceJSON( index, jg );
    }
    
    ArrayProducer::ComputeState::ComputeState( RC::ConstHandle<ArrayProducer> const &arrayProducer )
      : m_arrayProducer( arrayProducer )
      , m_count( arrayProducer->getCount() )
    {
    }
      
    void ArrayProducer::ComputeState::produceJSON( size_t index, Util::JSONGenerator &jg ) const
    {
      RC::ConstHandle<RT::Desc> elementDesc = m_arrayProducer->m_elementDesc;
      
      size_t allocSize = elementDesc->getAllocSize();
      void *valueData = alloca( allocSize );
      memset( valueData, 0, allocSize );
      produce( index, valueData );
      elementDesc->generateJSON( valueData, jg );
      elementDesc->disposeData( valueData );
    }
  };
};
