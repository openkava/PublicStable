/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ArrayProducerWrapper.h>
#include <Fabric/Core/MR/ArrayProducer.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ArrayProducerWrapper, ProducerWrapper )
    
    ArrayProducerWrapper::ArrayProducerWrapper(
      FABRIC_GC_OBJECT_CLASS_PARAM
      )
      : ProducerWrapper( FABRIC_GC_OBJECT_CLASS_ARG )
    {
    }
    
    void ArrayProducerWrapper::jsonExec(
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( cmd == "getCount" )
        jsonExecGetCount( arg, resultJAG );
      else if ( cmd == "produce" )
        jsonExecProduce( arg, resultJAG );
      else ProducerWrapper::jsonExec( cmd, arg, resultJAG );
    }
    
    void ArrayProducerWrapper::jsonExecGetCount(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      Util::JSONGenerator jg = resultJAG.makeElement();
      jg.makeInteger( getUnwrapped()->getCount() );
    }
    
    void ArrayProducerWrapper::jsonExecProduce(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      size_t index = arg->toInteger()->value();
          
      Util::JSONGenerator jg = resultJAG.makeElement();
      getUnwrapped()->createComputeState()->produceJSON( index, jg );
    }
  }
}
