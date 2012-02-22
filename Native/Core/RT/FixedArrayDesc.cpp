/*
 *  Copyright 2010-2012 Fabric Engine Inc. All rights reserved.
 */

#include "FixedArrayDesc.h"

#include <Fabric/Core/RT/FixedArrayImpl.h>
#include <Fabric/Base/JSON/Encoder.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  

  namespace RT
  {
    FixedArrayDesc::FixedArrayDesc( std::string const &name, RC::ConstHandle<FixedArrayImpl> const &fixedArrayImpl, RC::ConstHandle<Desc> const &memberDesc )
      : ArrayDesc( name, fixedArrayImpl, memberDesc )
      , m_fixedArrayImpl( fixedArrayImpl )
    {
    }

    size_t FixedArrayDesc::getNumMembers() const
    {
      return m_fixedArrayImpl->getNumMembers();
    }
    
    void FixedArrayDesc::jsonDesc( JSON::ObjectEncoder &resultObjectEncoder ) const
    {
      ArrayDesc::jsonDesc( resultObjectEncoder );
      resultObjectEncoder.makeMember( "internalType" ).makeString( "fixedArray" );
      resultObjectEncoder.makeMember( "numMembers" ).makeInteger( getNumMembers() );
    }
  };
};
