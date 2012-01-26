/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "ArrayProducerDesc.h"
#include "ArrayProducerImpl.h"
#include <Fabric/Base/JSON/Encoder.h>

namespace Fabric
{
  namespace RT
  {
    ArrayProducerDesc::ArrayProducerDesc(
      std::string const &name,
      RC::ConstHandle<ArrayProducerImpl> const &arrayProducerImpl,
      RC::ConstHandle<RT::Desc> const &elementDesc
      )
      : Desc( name, arrayProducerImpl )
      , m_arrayProducerImpl( arrayProducerImpl )
      , m_elementDesc( elementDesc )
    {
    }
    
    void ArrayProducerDesc::jsonDesc( JSON::ObjectEncoder &resultObjectEncoder ) const
    {
      Desc::jsonDesc( resultObjectEncoder );
      resultObjectEncoder.makeMember( "internalType" ).makeString( "arrayProducer" );
    }
    
    RC::ConstHandle<Desc> ArrayProducerDesc::getElementDesc() const
    {
      return m_elementDesc;
    }
  }
}
