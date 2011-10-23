/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "FixedArrayDesc.h"

#include <Fabric/Core/RT/FixedArrayImpl.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Core/Util/JSONGenerator.h>

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
    
    void FixedArrayDesc::jsonDesc( Util::JSONObjectGenerator &resultJOG ) const
    {
      ArrayDesc::jsonDesc( resultJOG );
      resultJOG.makeMember( "internalType" ).makeString( "fixedArray" );
      resultJOG.makeMember( "numMembers" ).makeInteger( getNumMembers() );
    }
  };
};
