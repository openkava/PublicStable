/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/Interface.h>
#include <Fabric/Core/MR/ConstArrayProducer.h>
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
    Interface::Interface( RC::ConstHandle<RT::Manager> const &rtManager )
      : m_rtManager( rtManager )
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
      else m_gcContainer.jsonRoute( dst, dstOffset, cmd, arg, resultJAG );
    }
      
    void Interface::jsonExec(
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( cmd == "createConstArrayProducer" )
        jsonExecCreateConstArrayProducer( arg, resultJAG );
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
      constArrayProducer->reg( &m_gcContainer, id_ );
    }
  };
};
