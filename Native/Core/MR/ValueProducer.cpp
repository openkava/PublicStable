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
      
    void ValueProducer::produceJSON( Util::JSONGenerator &jg ) const
    {
      size_t allocSize = m_valueDesc->getAllocSize();
      void *valueData = alloca( allocSize );
      memset( valueData, 0, allocSize );
      produce( valueData );
      m_valueDesc->generateJSON( valueData, jg );
      m_valueDesc->disposeData( valueData );
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
      produceJSON( jg );
    }
  };
};
