/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ConstArray.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/RT/VariableArrayDesc.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ConstArray, ArrayProducer );
    
    RC::Handle<ConstArray> ConstArray::Create(
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<RT::Desc> const &elementDesc,
      RC::ConstHandle<JSON::Array> const &jsonArray
      )
    {
      return new ConstArray( FABRIC_GC_OBJECT_MY_CLASS, rtManager, elementDesc, jsonArray );
    }
    
    ConstArray::ConstArray(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<RT::Desc> const &elementDesc,
      RC::ConstHandle<JSON::Array> const &jsonArray
      )
      : ArrayProducer( FABRIC_GC_OBJECT_CLASS_ARG, elementDesc )
      , m_variableArrayDesc( rtManager->getVariableArrayOf( elementDesc ) )
    {
      m_data.resize( m_variableArrayDesc->getAllocSize(), 0 );
      m_variableArrayDesc->setDataFromJSONValue( jsonArray, &m_data[0] );
    }
    
    ConstArray::~ConstArray()
    {
      m_variableArrayDesc->disposeData( &m_data[0] );
    }

    char const *ConstArray::getKind() const
    {
      return "ConstArray";
    }
    
    void ConstArray::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      Util::JSONGenerator jg = jog.makeMember( "data" );
      m_variableArrayDesc->generateJSON( &m_data[0], jg );
    }

    size_t ConstArray::count() const
    {
      return m_variableArrayDesc->getNumMembers( &m_data[0] );
    }
    
    void ConstArray::produce( size_t index, void *data ) const
    {
      return getElementDesc()->setData( m_variableArrayDesc->getMemberData( &m_data[0], index ), data );
    }
    
    void ConstArray::produceJSON( size_t index, Util::JSONGenerator &jg ) const
    {
      return getElementDesc()->generateJSON( m_variableArrayDesc->getMemberData( &m_data[0], index ), jg );
    }
  }
}
