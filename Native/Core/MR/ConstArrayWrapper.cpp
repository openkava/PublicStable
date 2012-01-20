/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ConstArrayWrapper.h>
#include <Fabric/Core/MR/ConstArray.h>
#include <Fabric/Core/RT/ArrayDesc.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Core/Util/JSONDecoder.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ConstArrayWrapper, ArrayProducerWrapper );
    
    RC::Handle<ConstArrayWrapper> ConstArrayWrapper::Create(
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<RT::Desc> const &elementDesc,
      RC::ConstHandle<JSON::Array> const &jsonArray
      )
    {
      return new ConstArrayWrapper( FABRIC_GC_OBJECT_MY_CLASS, rtManager, elementDesc, jsonArray );
    }
    
    RC::Handle<ConstArrayWrapper> ConstArrayWrapper::Create(
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<RT::Desc> const &elementDesc,
      Util::JSONEntityInfo const &entityInfo
      )
    {
      return new ConstArrayWrapper( FABRIC_GC_OBJECT_MY_CLASS, rtManager, elementDesc, entityInfo );
    }
    
    ConstArrayWrapper::ConstArrayWrapper(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<RT::Desc> const &elementDesc,
      RC::ConstHandle<JSON::Array> const &jsonArray
      )
      : ArrayProducerWrapper( FABRIC_GC_OBJECT_CLASS_ARG )
      , m_unwrapped( ConstArray::Create( rtManager, elementDesc, jsonArray ) )
    {
    }
    
    ConstArrayWrapper::ConstArrayWrapper(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<RT::Desc> const &elementDesc,
      Util::JSONEntityInfo const &entityInfo
      )
      : ArrayProducerWrapper( FABRIC_GC_OBJECT_CLASS_ARG )
      , m_unwrapped( ConstArray::Create( rtManager, elementDesc, entityInfo ) )
    {
    }
      
    RC::ConstHandle<ArrayProducer> ConstArrayWrapper::getUnwrapped() const
    {
      return m_unwrapped;
    }

    char const *ConstArrayWrapper::getKind() const
    {
      return "ConstArray";
    }
    
    void ConstArrayWrapper::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      {
        Util::JSONGenerator jg = jog.makeMember( "elementType" );
        jg.makeString( m_unwrapped->getElementDesc()->getUserName() );
      }
      
      {
        Util::JSONGenerator jg = jog.makeMember( "data" );
        m_unwrapped->getArrayDesc()->generateJSON( m_unwrapped->getImmutableData(), jg );
      }
    }
  }
}
