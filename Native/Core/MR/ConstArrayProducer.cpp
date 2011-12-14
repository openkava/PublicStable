/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ConstArrayProducer.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/RT/VariableArrayDesc.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_GET_CLASS_IMPL( ConstArrayProducer, ArrayProducer );
    
    RC::Handle<ConstArrayProducer> ConstArrayProducer::Create(
      GC::Container *container,
      std::string const &id_,
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<RT::Desc> const &elementDesc,
      RC::ConstHandle<JSON::Array> const &jsonArray
      )
    {
      return new ConstArrayProducer( GetClass(), container, id_, rtManager, elementDesc, jsonArray );
    }
    
    ConstArrayProducer::ConstArrayProducer(
      GC::Object::Class const *myClass,
      GC::Container *container,
      std::string const &id_,
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<RT::Desc> const &elementDesc,
      RC::ConstHandle<JSON::Array> const &jsonArray
      )
      : ArrayProducer( myClass, container, id_, elementDesc )
      , m_variableArrayDesc( rtManager->getVariableArrayOf( elementDesc ) )
    {
      m_data.resize( m_variableArrayDesc->getAllocSize(), 0 );
      m_variableArrayDesc->setDataFromJSONValue( jsonArray, &m_data[0] );
    }
    
    ConstArrayProducer::~ConstArrayProducer()
    {
      m_variableArrayDesc->disposeData( &m_data[0] );
    }

    char const *ConstArrayProducer::getKind() const
    {
      return "ConstArrayProducer";
    }
    
    void ConstArrayProducer::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      Util::JSONGenerator jg = jog.makeMember( "data" );
      m_variableArrayDesc->generateJSON( &m_data[0], jg );
    }

    size_t ConstArrayProducer::count() const
    {
      return m_variableArrayDesc->getNumMembers( &m_data[0] );
    }
    
    void ConstArrayProducer::produce( size_t index, void *data ) const
    {
      return m_variableArrayDesc->setData( m_variableArrayDesc->getMemberData( &m_data[0], index ), data );
    }
  };
};
