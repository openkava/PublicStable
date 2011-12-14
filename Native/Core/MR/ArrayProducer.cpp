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
    FABRIC_GC_OBJECT_GET_CLASS_IMPL( ArrayProducer, Producer );
    
    ArrayProducer::ArrayProducer(
      GC::Object::Class const *myClass,
      GC::Container *container,
      std::string const &id_,
      RC::ConstHandle<RT::Desc> const &elementDesc
      )
      : Producer( myClass, container, id_ )
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
      jg.makeInteger( count() );
    }
    
    void ArrayProducer::jsonExecProduce(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      size_t index = arg->toInteger()->value();
          
      Util::JSONGenerator jg = resultJAG.makeElement();
      produceJSON( index, jg );
    }
  };
};
