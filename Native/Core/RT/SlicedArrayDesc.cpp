/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "SlicedArrayDesc.h"
#include "SlicedArrayImpl.h"

#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/Exception.h>

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
    
    RC::Handle<JSON::Object> SlicedArrayDesc::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = ArrayDesc::jsonDesc();
      result->set( "internalType", JSON::String::Create("slicedArray") );
      result->set( "memberType", JSON::String::Create( getMemberDesc()->getName() ) );
      return result;
    }
  };
};
