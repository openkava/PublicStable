/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ValueProducerWrapper.h>
#include <Fabric/Core/MR/ValueProducer.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Base/JSON/Integer.h>

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
      else if ( cmd == "produceAsync" )
        jsonExecProduceAsync( arg, resultJAG );
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
    
    struct JSONProduceAsyncUserdata
    {
      RC::Handle<ValueProducerWrapper> valueProducerWrapper;
      Util::SimpleString *notifyJSONArg;
      Util::JSONGenerator notifyJSONArgGenerator;
      Util::JSONObjectGenerator notifyJSONArgObjectGenerator;
    
      JSONProduceAsyncUserdata( RC::Handle<ValueProducerWrapper> const &valueProducerWrapper_, int32_t serial )
        : valueProducerWrapper( valueProducerWrapper_ )
        , notifyJSONArg( new Util::SimpleString )
        , notifyJSONArgGenerator( notifyJSONArg )
        , notifyJSONArgObjectGenerator( notifyJSONArgGenerator.makeObject() )
      {
        Util::JSONGenerator notifyJSONArgSerialGenerator( notifyJSONArgObjectGenerator.makeMember( "serial" ) );
        notifyJSONArgSerialGenerator.makeInteger( serial );
      }
    };
    
    void ValueProducerWrapper::jsonExecProduceAsync( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      JSONProduceAsyncUserdata *jsonProduceAsyncUserdata = new JSONProduceAsyncUserdata( this, arg->toInteger()->value() );
      getUnwrapped()->createComputeState()->produceJSONAsync(
        jsonProduceAsyncUserdata->notifyJSONArgObjectGenerator,
        &ValueProducerWrapper::JSONExecProduceAsyncFinishedCallback,
        jsonProduceAsyncUserdata
        );
    }
    
    void ValueProducerWrapper::JSONExecProduceAsyncFinishedCallback( void *userdata )
    {
      JSONProduceAsyncUserdata *jsonProduceAsyncUserdata = static_cast<JSONProduceAsyncUserdata *>( userdata );
      RC::Handle<ValueProducerWrapper> valueProducerWrapper = jsonProduceAsyncUserdata->valueProducerWrapper;
      Util::SimpleString *notifyJSONArg = jsonProduceAsyncUserdata->notifyJSONArg;
      delete jsonProduceAsyncUserdata;

      valueProducerWrapper->jsonNotify( "produceAsyncFinished", 20, notifyJSONArg );
      delete notifyJSONArg;
    }
  }
}
