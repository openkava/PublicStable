/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/Interface.h>
#include <Fabric/Core/MR/ArrayGeneratorWrapper.h>
#include <Fabric/Core/MR/ArrayMapWrapper.h>
#include <Fabric/Core/MR/ArrayTransformWrapper.h>
#include <Fabric/Core/MR/ConstArrayWrapper.h>
#include <Fabric/Core/MR/ConstValueWrapper.h>
#include <Fabric/Core/MR/ReduceWrapper.h>
#include <Fabric/Core/MR/ValueGeneratorWrapper.h>
#include <Fabric/Core/MR/ValueMapWrapper.h>
#include <Fabric/Core/MR/ValueTransformWrapper.h>
#include <Fabric/Core/KLC/ArrayGeneratorOperatorWrapper.h>
#include <Fabric/Core/KLC/ArrayMapOperatorWrapper.h>
#include <Fabric/Core/KLC/ArrayTransformOperatorWrapper.h>
#include <Fabric/Core/KLC/ReduceOperatorWrapper.h>
#include <Fabric/Core/KLC/ValueGeneratorOperatorWrapper.h>
#include <Fabric/Core/KLC/ValueMapOperatorWrapper.h>
#include <Fabric/Core/KLC/ValueTransformOperatorWrapper.h>
#include <Fabric/Core/GC/Object.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Core/Util/Parse.h>
#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Base/JSON/Decode.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace MR
  {
    Interface::Interface(
      GC::Container *gcContainer,
      RC::ConstHandle<RT::Manager> const &rtManager
      )
      : m_gcContainer( gcContainer )
      , m_rtManager( rtManager )
    {
    }
  
    void Interface::jsonRoute(
      std::vector<std::string> const &dst,
      size_t dstOffset,
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( dst.size() - dstOffset == 0 )
      {
        jsonExec( cmd, arg, resultJAG );
      }
      else m_gcContainer->jsonRoute( dst, dstOffset, cmd, arg, resultJAG );
    }
      
    void Interface::jsonExec(
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( cmd == "createConstValue" )
        jsonExecCreateConstValue( arg, resultJAG );
      else if ( cmd == "createValueGenerator" )
        jsonExecCreateValueGenerator( arg, resultJAG );
      else if ( cmd == "createValueMap" )
        jsonExecCreateValueMap( arg, resultJAG );
      else if ( cmd == "createValueTransform" )
        jsonExecCreateValueTransform( arg, resultJAG );
      else if ( cmd == "createConstArray" )
        jsonExecCreateConstArray( arg, resultJAG );
      else if ( cmd == "createArrayGenerator" )
        jsonExecCreateArrayGenerator( arg, resultJAG );
      else if ( cmd == "createArrayMap" )
        jsonExecCreateArrayMap( arg, resultJAG );
      else if ( cmd == "createArrayTransform" )
        jsonExecCreateArrayTransform( arg, resultJAG );
      else if ( cmd == "createReduce" )
        jsonExecCreateReduce( arg, resultJAG );
      else throw Exception( "unknown command: " + _(cmd) );
    }
    
    void Interface::jsonExecCreateConstArray(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      RC::ConstHandle<JSON::Object> argObject = arg->toObject();
      
      std::string id_;
      try
      {
        id_ = argObject->get( "id" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "id: " + e;
      }
      
      RC::ConstHandle<RT::Desc> elementTypeRTDesc;
      try
      {
        elementTypeRTDesc = m_rtManager->getDesc( argObject->get( "elementType" )->toString()->value() );
      }
      catch ( Exception e )
      {
        throw "elementType: " + e;
      }
      
      RC::ConstHandle<JSON::Array> dataJSONArray;
      try
      {
        RC::ConstHandle<JSON::Value> dataJSONValue = argObject->maybeGet("data");
        if ( dataJSONValue )
          dataJSONArray = dataJSONValue->toArray();
      }
      catch ( Exception e )
      {
        throw "data: " + e;
      }
      try
      {
        RC::ConstHandle<JSON::Value> jsonDataJSONValue = argObject->maybeGet("jsonData");
        if ( jsonDataJSONValue )
        {
          RC::ConstHandle<JSON::String> jsonDataJSONString = jsonDataJSONValue->toString();
          dataJSONArray = JSON::decode( jsonDataJSONString->data(), jsonDataJSONString->length() )->toArray();
        }
      }
      catch ( Exception e )
      {
        throw "jsonData: " + e;
      }
      if ( !dataJSONArray )
        throw Exception("missing data");
      
      ConstArrayWrapper::Create(
        m_rtManager,
        elementTypeRTDesc,
        dataJSONArray
        )->reg( m_gcContainer, id_ );
    }
    
    void Interface::jsonExecCreateArrayMap(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      RC::ConstHandle<JSON::Object> argObject = arg->toObject();
      
      std::string id_;
      try
      {
        id_ = argObject->get( "id" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "id: " + e;
      }
      
      RC::Handle<ArrayProducerWrapper> inputArrayProducer;
      try
      {
        inputArrayProducer = GC::DynCast<ArrayProducerWrapper>( m_gcContainer->getObject( argObject->get( "inputID" )->toString()->value() ) );
        if ( !inputArrayProducer )
          throw "must be an array producer";
      }
      catch ( Exception e )
      {
        throw "inputID: " + e;
      }
      
      RC::Handle<KLC::ArrayMapOperatorWrapper> mapOperator;
      try
      {
        mapOperator = GC::DynCast<KLC::ArrayMapOperatorWrapper>( m_gcContainer->getObject( argObject->get( "operatorID" )->toString()->value() ) );
        if ( !mapOperator )
          throw "must be a map operator";
      }
      catch ( Exception e )
      {
        throw "mapOperatorID: " + e;
      }
      
      RC::Handle<ValueProducerWrapper> sharedValueProducer;
      RC::ConstHandle<JSON::Value> sharedValueProducerIDValue = argObject->maybeGet( "sharedID" );
      if ( sharedValueProducerIDValue )
      {
        try
        {
          sharedValueProducer = GC::DynCast<ValueProducerWrapper>( m_gcContainer->getObject( sharedValueProducerIDValue->toString()->value() ) );
          if ( !sharedValueProducer )
            throw "must be a value producer";
        }
        catch ( Exception e )
        {
          throw "sharedID: " + e;
        }
      }
      
      ArrayMapWrapper::Create(
        inputArrayProducer,
        mapOperator,
        sharedValueProducer
        )->reg( m_gcContainer, id_ );
    }
    
    void Interface::jsonExecCreateValueMap(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      RC::ConstHandle<JSON::Object> argObject = arg->toObject();
      
      std::string id_;
      try
      {
        id_ = argObject->get( "id" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "id: " + e;
      }
      
      RC::Handle<ValueProducerWrapper> inputWrapper;
      try
      {
        inputWrapper = GC::DynCast<ValueProducerWrapper>( m_gcContainer->getObject( argObject->get( "inputID" )->toString()->value() ) );
        if ( !inputWrapper )
          throw "must be a value producer";
      }
      catch ( Exception e )
      {
        throw "inputID: " + e;
      }
      
      RC::Handle<KLC::ValueMapOperatorWrapper> operatorWrapper;
      try
      {
        operatorWrapper = GC::DynCast<KLC::ValueMapOperatorWrapper>( m_gcContainer->getObject( argObject->get( "operatorID" )->toString()->value() ) );
        if ( !operatorWrapper )
          throw "must be a value map operator";
      }
      catch ( Exception e )
      {
        throw "operatorID: " + e;
      }
      
      RC::Handle<ValueProducerWrapper> sharedWrapper;
      RC::ConstHandle<JSON::Value> sharedIDValue = argObject->maybeGet( "sharedID" );
      if ( sharedIDValue )
      {
        try
        {
          sharedWrapper = GC::DynCast<ValueProducerWrapper>( m_gcContainer->getObject( sharedIDValue->toString()->value() ) );
          if ( !sharedWrapper )
            throw "must be a value producer";
        }
        catch ( Exception e )
        {
          throw "sharedID: " + e;
        }
      }
      
      ValueMapWrapper::Create(
        inputWrapper,
        operatorWrapper,
        sharedWrapper
        )->reg( m_gcContainer, id_ );
    }
    
    void Interface::jsonExecCreateValueTransform(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      RC::ConstHandle<JSON::Object> argObject = arg->toObject();
      
      std::string id_;
      try
      {
        id_ = argObject->get( "id" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "id: " + e;
      }
      
      RC::Handle<ValueProducerWrapper> inputWrapper;
      try
      {
        inputWrapper = GC::DynCast<ValueProducerWrapper>( m_gcContainer->getObject( argObject->get( "inputID" )->toString()->value() ) );
        if ( !inputWrapper )
          throw "must be a value producer";
      }
      catch ( Exception e )
      {
        throw "inputID: " + e;
      }
      
      RC::Handle<KLC::ValueTransformOperatorWrapper> operatorWrapper;
      try
      {
        operatorWrapper = GC::DynCast<KLC::ValueTransformOperatorWrapper>( m_gcContainer->getObject( argObject->get( "operatorID" )->toString()->value() ) );
        if ( !operatorWrapper )
          throw "must be a value transform operator";
      }
      catch ( Exception e )
      {
        throw "operatorID: " + e;
      }
      
      RC::Handle<ValueProducerWrapper> sharedWrapper;
      RC::ConstHandle<JSON::Value> sharedIDValue = argObject->maybeGet( "sharedID" );
      if ( sharedIDValue )
      {
        try
        {
          sharedWrapper = GC::DynCast<ValueProducerWrapper>( m_gcContainer->getObject( sharedIDValue->toString()->value() ) );
          if ( !sharedWrapper )
            throw "must be a value producer";
        }
        catch ( Exception e )
        {
          throw "sharedID: " + e;
        }
      }
      
      ValueTransformWrapper::Create(
        inputWrapper,
        operatorWrapper,
        sharedWrapper
        )->reg( m_gcContainer, id_ );
    }
    
    void Interface::jsonExecCreateValueGenerator(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      RC::ConstHandle<JSON::Object> argObject = arg->toObject();
      
      std::string id_;
      try
      {
        id_ = argObject->get( "id" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "id: " + e;
      }
      
      RC::Handle<KLC::ValueGeneratorOperatorWrapper> operatorWrapper;
      try
      {
        operatorWrapper = GC::DynCast<KLC::ValueGeneratorOperatorWrapper>( m_gcContainer->getObject( argObject->get( "operatorID" )->toString()->value() ) );
        if ( !operatorWrapper )
          throw "must be a value transform operator";
      }
      catch ( Exception e )
      {
        throw "operatorID: " + e;
      }
      
      RC::Handle<ValueProducerWrapper> sharedWrapper;
      RC::ConstHandle<JSON::Value> sharedIDValue = argObject->maybeGet( "sharedID" );
      if ( sharedIDValue )
      {
        try
        {
          sharedWrapper = GC::DynCast<ValueProducerWrapper>( m_gcContainer->getObject( sharedIDValue->toString()->value() ) );
          if ( !sharedWrapper )
            throw "must be a value producer";
        }
        catch ( Exception e )
        {
          throw "sharedID: " + e;
        }
      }
      
      ValueGeneratorWrapper::Create(
        operatorWrapper,
        sharedWrapper
        )->reg( m_gcContainer, id_ );
    }
    
    void Interface::jsonExecCreateArrayTransform(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      RC::ConstHandle<JSON::Object> argObject = arg->toObject();
      
      std::string id_;
      try
      {
        id_ = argObject->get( "id" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "id: " + e;
      }
      
      RC::Handle<ArrayProducerWrapper> input;
      try
      {
        input = GC::DynCast<ArrayProducerWrapper>( m_gcContainer->getObject( argObject->get( "inputID" )->toString()->value() ) );
        if ( !input )
          throw "must be an array producer";
      }
      catch ( Exception e )
      {
        throw "inputID: " + e;
      }
      
      RC::Handle<KLC::ArrayTransformOperatorWrapper> operator_;
      try
      {
        operator_ = GC::DynCast<KLC::ArrayTransformOperatorWrapper>( m_gcContainer->getObject( argObject->get( "operatorID" )->toString()->value() ) );
        if ( !operator_ )
          throw "must be a value transform operator";
      }
      catch ( Exception e )
      {
        throw "operatorID: " + e;
      }
      
      RC::Handle<ValueProducerWrapper> sharedValueProducer;
      RC::ConstHandle<JSON::Value> sharedValueProducerIDValue = argObject->maybeGet( "sharedID" );
      if ( sharedValueProducerIDValue )
      {
        try
        {
          sharedValueProducer = GC::DynCast<ValueProducerWrapper>( m_gcContainer->getObject( sharedValueProducerIDValue->toString()->value() ) );
          if ( !sharedValueProducer )
            throw "must be a value producer";
        }
        catch ( Exception e )
        {
          throw "sharedID: " + e;
        }
      }
      
      ArrayTransformWrapper::Create(
        input,
        operator_,
        sharedValueProducer
        )->reg( m_gcContainer, id_ );
    }
    
    void Interface::jsonExecCreateReduce(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      RC::ConstHandle<JSON::Object> argObject = arg->toObject();
      
      std::string id_;
      try
      {
        id_ = argObject->get( "id" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "id: " + e;
      }
      
      RC::Handle<ArrayProducerWrapper> inputArrayProducer;
      try
      {
        inputArrayProducer = GC::DynCast<ArrayProducerWrapper>( m_gcContainer->getObject( argObject->get( "inputArrayProducerID" )->toString()->value() ) );
        if ( !inputArrayProducer )
          throw "must be an array producer";
      }
      catch ( Exception e )
      {
        throw "inputArrayProducerID: " + e;
      }
      
      RC::Handle<KLC::ReduceOperatorWrapper> reduceOperator;
      try
      {
        reduceOperator = GC::DynCast<KLC::ReduceOperatorWrapper>( m_gcContainer->getObject( argObject->get( "reduceOperatorID" )->toString()->value() ) );
        if ( !reduceOperator )
          throw "must be a reduce operator";
      }
      catch ( Exception e )
      {
        throw "reduceOperatorID: " + e;
      }
      
      RC::Handle<ValueProducerWrapper> sharedValueProducer;
      RC::ConstHandle<JSON::Value> sharedValueProducerIDValue = argObject->maybeGet( "sharedValueProducerID" );
      if ( sharedValueProducerIDValue )
      {
        try
        {
          sharedValueProducer = GC::DynCast<ValueProducerWrapper>( m_gcContainer->getObject( sharedValueProducerIDValue->toString()->value() ) );
          if ( !sharedValueProducer )
            throw "must be a value producer";
        }
        catch ( Exception e )
        {
          throw "sharedValueProducerID: " + e;
        }
      }
      
      ReduceWrapper::Create(
        inputArrayProducer,
        reduceOperator,
        sharedValueProducer
        )->reg( m_gcContainer, id_ );
    }
    
    void Interface::jsonExecCreateArrayGenerator(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      RC::ConstHandle<JSON::Object> argObject = arg->toObject();
      
      std::string id_;
      try
      {
        id_ = argObject->get( "id" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "id: " + e;
      }
      
      RC::Handle<ValueProducerWrapper> countValueProducer;
      try
      {
        countValueProducer = GC::DynCast<ValueProducerWrapper>( m_gcContainer->getObject( argObject->get( "countID" )->toString()->value() ) );
        if ( !countValueProducer )
          throw "must be a value producer";
      }
      catch ( Exception e )
      {
        throw "countID: " + e;
      }
      
      RC::Handle<KLC::ArrayGeneratorOperatorWrapper> arrayGeneratorOperator;
      try
      {
        arrayGeneratorOperator = GC::DynCast<KLC::ArrayGeneratorOperatorWrapper>( m_gcContainer->getObject( argObject->get( "operatorID" )->toString()->value() ) );
        if ( !arrayGeneratorOperator )
          throw "must be an array generator operator";
      }
      catch ( Exception e )
      {
        throw "operatorID: " + e;
      }
      
      RC::Handle<ValueProducerWrapper> sharedValueProducer;
      RC::ConstHandle<JSON::Value> sharedValueProducerIDValue = argObject->maybeGet( "sharedID" );
      if ( sharedValueProducerIDValue )
      {
        try
        {
          sharedValueProducer = GC::DynCast<ValueProducerWrapper>( m_gcContainer->getObject( sharedValueProducerIDValue->toString()->value() ) );
          if ( !sharedValueProducer )
            throw "must be a value producer";
        }
        catch ( Exception e )
        {
          throw "sharedID: " + e;
        }
      }
      
      ArrayGeneratorWrapper::Create(
        countValueProducer,
        arrayGeneratorOperator,
        sharedValueProducer
        )->reg( m_gcContainer, id_ );
    }
    
    void Interface::jsonExecCreateConstValue(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      RC::ConstHandle<JSON::Object> argObject = arg->toObject();
      
      std::string id_;
      try
      {
        id_ = argObject->get( "id" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "id: " + e;
      }
      
      RC::ConstHandle<RT::Desc> valueTypeRTDesc;
      try
      {
        valueTypeRTDesc = m_rtManager->getDesc( argObject->get( "valueType" )->toString()->value() );
      }
      catch ( Exception e )
      {
        throw "valueType: " + e;
      }
      
      RC::ConstHandle<JSON::Value> dataJSONValue;
      try
      {
        dataJSONValue = argObject->get( "data" );
      }
      catch ( Exception e )
      {
        throw "data: " + e;
      }
      
      ConstValueWrapper::Create(
        m_rtManager,
        valueTypeRTDesc,
        dataJSONValue
        )->reg( m_gcContainer, id_ );
    }
  }
}
