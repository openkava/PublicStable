/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ArrayProducerWrapper.h>
#include <Fabric/Core/MR/ArrayProducer.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Base/Exception.h>

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
      else if ( cmd == "produceAsync" )
        jsonExecProduceAsync( arg, resultJAG );
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
      RC::ConstHandle<JSON::Object> argObject = arg->toObject();
      
      Util::JSONGenerator jg = resultJAG.makeElement();

      RC::ConstHandle<JSON::Value> indexValue = argObject->maybeGet("index");
      if ( indexValue )
      {
        size_t index;
        try
        {
          index = indexValue->toInteger()->value();
        }
        catch ( Exception e )
        {
          throw "index: " + e;
        }
        
        RC::ConstHandle<JSON::Value> countValue = argObject->maybeGet("count");
        if ( countValue )
        {
          size_t count;
          try
          {
            count = countValue->toInteger()->value();
          }
          catch ( Exception e )
          {
            throw "count: " + e;
          }
          
          getUnwrapped()->createComputeState()->produceJSON( index, count, jg );
        }
        else getUnwrapped()->createComputeState()->produceJSON( index, jg );
      }
      else getUnwrapped()->createComputeState()->produceJSON( jg );
    }
    
    struct JSONProduceAsyncUserdata
    {
      RC::Handle<ArrayProducerWrapper> arrayProducerWrapper;
      Util::SimpleString *notifyJSONArg;
      Util::JSONGenerator notifyJSONArgGenerator;
      Util::JSONObjectGenerator notifyJSONArgObjectGenerator;
    
      JSONProduceAsyncUserdata( RC::Handle<ArrayProducerWrapper> const &arrayProducerWrapper_, int32_t serial )
        : arrayProducerWrapper( arrayProducerWrapper_ )
        , notifyJSONArg( new Util::SimpleString )
        , notifyJSONArgGenerator( notifyJSONArg )
        , notifyJSONArgObjectGenerator( notifyJSONArgGenerator.makeObject() )
      {
        Util::JSONGenerator notifyJSONArgSerialGenerator( notifyJSONArgObjectGenerator.makeMember( "serial" ) );
        notifyJSONArgSerialGenerator.makeInteger( serial );
      }
    };
    
    void ArrayProducerWrapper::jsonExecProduceAsync(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      RC::ConstHandle<JSON::Object> argObject = arg->toObject();
      
      int32_t serial = argObject->get("serial")->toInteger()->value();
      JSONProduceAsyncUserdata *jsonProduceAsyncUserdata = new JSONProduceAsyncUserdata( this, serial );

      RC::ConstHandle<JSON::Value> indexValue = argObject->maybeGet("index");
      if ( indexValue )
      {
        size_t index;
        try
        {
          index = indexValue->toInteger()->value();
        }
        catch ( Exception e )
        {
          throw "index: " + e;
        }
        
        RC::ConstHandle<JSON::Value> countValue = argObject->maybeGet("count");
        if ( countValue )
        {
          size_t count;
          try
          {
            count = countValue->toInteger()->value();
          }
          catch ( Exception e )
          {
            throw "count: " + e;
          }
          
          getUnwrapped()->createComputeState()->produceJSONAsync(
            index,
            count,
            jsonProduceAsyncUserdata->notifyJSONArgObjectGenerator,
            &ArrayProducerWrapper::JSONExecProduceAsyncFinishedCallback,
            jsonProduceAsyncUserdata
            );
        }
        else getUnwrapped()->createComputeState()->produceJSONAsync(
          index,
          jsonProduceAsyncUserdata->notifyJSONArgObjectGenerator,
          &ArrayProducerWrapper::JSONExecProduceAsyncFinishedCallback,
          jsonProduceAsyncUserdata
          );
      }
      else getUnwrapped()->createComputeState()->produceJSONAsync(
        jsonProduceAsyncUserdata->notifyJSONArgObjectGenerator,
        &ArrayProducerWrapper::JSONExecProduceAsyncFinishedCallback,
        jsonProduceAsyncUserdata
        );
    }
    
    void ArrayProducerWrapper::JSONExecProduceAsyncFinishedCallback( void *userdata )
    {
      JSONProduceAsyncUserdata *jsonProduceAsyncUserdata = static_cast<JSONProduceAsyncUserdata *>( userdata );
      RC::Handle<ArrayProducerWrapper> arrayProducerWrapper = jsonProduceAsyncUserdata->arrayProducerWrapper;
      Util::SimpleString *notifyJSONArg = jsonProduceAsyncUserdata->notifyJSONArg;
      delete jsonProduceAsyncUserdata;

      arrayProducerWrapper->jsonNotify( "produceAsyncFinished", 20, notifyJSONArg );
      delete notifyJSONArg;
    }
  }
}
