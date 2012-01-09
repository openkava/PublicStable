/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ValueProducerWrapper.h>
#include <Fabric/Core/MR/ValueProducer.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ValueProducerWrapper, ProducerWrapper );
    
    ValueProducerWrapper::ValueProducerWrapper(
      FABRIC_GC_OBJECT_CLASS_PARAM
      )
      : ProducerWrapper( FABRIC_GC_OBJECT_CLASS_ARG )
    {
    }
    
    void ValueProducerWrapper::jsonExec(
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( cmd == "produce" )
        jsonExecProduce( arg, resultJAG );
      else ProducerWrapper::jsonExec( cmd, arg, resultJAG );
    }
    
    void ValueProducerWrapper::jsonExecProduce(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      Util::JSONGenerator jg = resultJAG.makeElement();
      getUnwrapped()->createComputeState()->produceJSON( jg );
    }
  }
}
