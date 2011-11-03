/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "SlicedArrayDesc.h"
#include "SlicedArrayImpl.h"

#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace RT
  {
    SlicedArrayDesc::SlicedArrayDesc( std::string const &name, RC::ConstHandle<SlicedArrayImpl> const &slicedArrayImpl, RC::ConstHandle<Desc> const &memberDesc )
      : ArrayDesc( name, slicedArrayImpl, memberDesc )
      , m_slicedArrayImpl( slicedArrayImpl )
    {
    }

    RC::ConstHandle<RT::SlicedArrayImpl> SlicedArrayDesc::getImpl() const
    {
      return RC::ConstHandle<RT::SlicedArrayImpl>::StaticCast( Desc::getImpl() );
    }
    
    void SlicedArrayDesc::jsonDesc( Util::JSONObjectGenerator &resultJOG ) const
    {
      ArrayDesc::jsonDesc( resultJOG );
      resultJOG.makeMember( "internalType" ).makeString( "slicedArray" );
      resultJOG.makeMember( "memberType" ).makeString( getMemberDesc()->getUserName() );
    }

    void SlicedArrayDesc::setNumMembers( void *data, size_t numMembers, void const *defaultMemberData ) const
    {
      return m_slicedArrayImpl->setNumMembers( data, numMembers, defaultMemberData );
    }
  };
};
