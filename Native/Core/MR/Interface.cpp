/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/Interface.h>
#include <Fabric/Core/MR/ConstArrayProducer.h>
#include <Fabric/Core/MR/Map.h>
#include <Fabric/Core/KLC/MapOperator.h>
#include <Fabric/Core/GC/Object.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Core/Util/Parse.h>
#include <Fabric/Base/JSON/Array.h>
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
      if ( cmd == "createConstArrayProducer" )
        jsonExecCreateConstArrayProducer( arg, resultJAG );
      else if ( cmd == "createMap" )
        jsonExecCreateMap( arg, resultJAG );
      else throw Exception( "unknown command: " + _(cmd) );
    }
    
    void Interface::jsonExecCreateConstArrayProducer(
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
        dataJSONArray = argObject->get( "data" )->toArray();
      }
      catch ( Exception e )
      {
        throw "data: " + e;
      }
      
      RC::Handle<ConstArrayProducer> constArrayProducer = ConstArrayProducer::Create(
        m_rtManager,
        elementTypeRTDesc,
        dataJSONArray
        );
      constArrayProducer->reg( m_gcContainer, id_ );
    }
    
    void Interface::jsonExecCreateMap(
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
      
      RC::Handle<ArrayProducer> inputArrayProducer;
      try
      {
        inputArrayProducer = GC::DynCast<ArrayProducer>( m_gcContainer->getObject( argObject->get( "inputArrayProducerID" )->toString()->value() ) );
        if ( !inputArrayProducer )
          throw "must be an array producer";
      }
      catch ( Exception e )
      {
        throw "inputArrayProducerID: " + e;
      }
      
      RC::Handle<KLC::MapOperator> mapOperator;
      try
      {
        mapOperator = GC::DynCast<KLC::MapOperator>( m_gcContainer->getObject( argObject->get( "mapOperatorID" )->toString()->value() ) );
        if ( !mapOperator )
          throw "must be a map operator";
      }
      catch ( Exception e )
      {
        throw "mapOperatorID: " + e;
      }
      
      RC::Handle<Map> map = Map::Create(
        inputArrayProducer,
        mapOperator
        );
      map->reg( m_gcContainer, id_ );
    }
  };
};
