/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "ValueProducerDesc.h"
#include "ValueProducerImpl.h"
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace RT
  {
    ValueProducerDesc::ValueProducerDesc(
      std::string const &name,
      RC::ConstHandle<ValueProducerImpl> const &valueProducerImpl,
      RC::ConstHandle<RT::Desc> const &valueDesc
      )
      : Desc( name, valueProducerImpl )
      , m_valueProducerImpl( valueProducerImpl )
      , m_valueDesc( valueDesc )
    {
    }
    
    void ValueProducerDesc::jsonDesc( Util::JSONObjectGenerator &resultJOG ) const
    {
      Desc::jsonDesc( resultJOG );
      resultJOG.makeMember( "internalType" ).makeString( "valueProducer" );
    }
    
    RC::ConstHandle<Desc> ValueProducerDesc::getValueDesc() const
    {
      return m_valueDesc;
    }
  }
}