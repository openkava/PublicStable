/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "FixedArrayDesc.h"

#include <Fabric/Core/RT/FixedArrayImpl.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
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
    
    RC::Handle<JSON::Object> FixedArrayDesc::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = ArrayDesc::jsonDesc();
      result->set( "internalType", JSON::String::Create("fixedArray") );
      result->set( "numMembers", JSON::Integer::Create( getNumMembers() ) );
      return result;
    }
  };
};
