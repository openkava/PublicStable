/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ConstArray.h>
#include <Fabric/Core/RT/FixedArrayDesc.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/RT/VariableArrayDesc.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace MR
  {
    RC::Handle<ConstArray> ConstArray::Create(
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<RT::Desc> const &elementDesc,
      RC::ConstHandle<JSON::Array> const &jsonArray
      )
    {
      return new ConstArray( rtManager, elementDesc, jsonArray );
    }
    
    RC::Handle<ConstArray> ConstArray::Create(
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<RT::ArrayDesc> const &arrayDesc,
      void const *data
      )
    {
      return new ConstArray( rtManager, arrayDesc, data );
    }
    
    ConstArray::ConstArray(
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<RT::Desc> const &elementDesc,
      RC::ConstHandle<JSON::Array> const &jsonArray
      )
    {
      m_fixedArrayDesc = rtManager->getFixedArrayOf( elementDesc, jsonArray->size() );
      m_data.resize( m_fixedArrayDesc->getAllocSize(), 0 );
      m_fixedArrayDesc->setDataFromJSONValue( jsonArray, &m_data[0] );
    }
    
    ConstArray::ConstArray(
      RC::ConstHandle<RT::Manager> const &rtManager,
        RC::ConstHandle<RT::ArrayDesc> const &arrayDesc,
        void const *data
      )
    {
      RC::ConstHandle<RT::Desc> elementDesc = arrayDesc->getMemberDesc();
      size_t count = arrayDesc->getNumMembers( data );

      m_fixedArrayDesc = rtManager->getFixedArrayOf( elementDesc, count );
      m_data.resize( m_fixedArrayDesc->getAllocSize(), 0 );
      for ( size_t i=0; i<count; ++i )
        elementDesc->setData( arrayDesc->getMemberData( data, i ), m_fixedArrayDesc->getMemberData( &m_data[0], i ) );
    }

    ConstArray::~ConstArray()
    {
      m_fixedArrayDesc->disposeData( &m_data[0] );
    }
    
    RC::ConstHandle<RT::Desc> ConstArray::getElementDesc() const
    {
      return m_fixedArrayDesc->getMemberDesc();
    }
    
    size_t ConstArray::getCount() const
    {
      return m_fixedArrayDesc->getNumMembers();
    }
      
    const RC::Handle<ArrayProducer::ComputeState> ConstArray::createComputeState() const
    {
      return ComputeState::Create( this );
    }

    RC::Handle<ConstArray::ComputeState> ConstArray::ComputeState::Create( RC::ConstHandle<ConstArray> const &constArray )
    {
      return new ComputeState( constArray );
    }
    
    ConstArray::ComputeState::ComputeState( RC::ConstHandle<ConstArray> const &constArray )
      : ArrayProducer::ComputeState( constArray )
      , m_constArray( constArray )
    {
    }
    
    void ConstArray::ComputeState::produce( size_t index, void *data ) const
    {
      return m_constArray->getElementDesc()->setData( m_constArray->m_fixedArrayDesc->getMemberData( &m_constArray->m_data[0], index ), data );
    }
    
    void ConstArray::ComputeState::produceJSON( size_t index, Util::JSONGenerator &jg ) const
    {
      return m_constArray->getElementDesc()->generateJSON( m_constArray->m_fixedArrayDesc->getMemberData( &m_constArray->m_data[0], index ), jg );
    }

    RC::ConstHandle<RT::ArrayDesc> ConstArray::getArrayDesc() const
    {
      return m_fixedArrayDesc;
    }
    
    void const *ConstArray::getImmutableData() const
    {
      return &m_data[0];
    }

    void ConstArray::flush()
    {
    }
  }
}
