/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "ArrayDesc.h"

#include <Fabric/Core/RT/ArrayImpl.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>

namespace Fabric
{
  namespace RT
  {
    ArrayDesc::ArrayDesc( std::string const &name, RC::ConstHandle<ArrayImpl> const &arrayImpl, RC::ConstHandle<Desc> const &memberDesc )
      : Desc( name, arrayImpl )
      , m_arrayImpl( arrayImpl )
      , m_memberDesc( memberDesc )
    {
    }
    
    RC::ConstHandle<ArrayImpl> ArrayDesc::getImpl() const
    {
      return m_arrayImpl;
    }
    
    RC::ConstHandle<Desc> ArrayDesc::getMemberDesc() const
    {
      return m_memberDesc;
    }

    size_t ArrayDesc::getNumMembers( void const *data ) const
    {
      return m_arrayImpl->getNumMembers( data );
    }
    
    void const *ArrayDesc::getMemberData( void const *data, size_t index ) const
    {
      return m_arrayImpl->getMemberData( data, index );
    }
    
    void *ArrayDesc::getMemberData( void *data, size_t index ) const
    {
      return m_arrayImpl->getMemberData( data, index );
    }
    
    RC::Handle<JSON::Object> ArrayDesc::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = Desc::jsonDesc();
      result->set( "memberType", JSON::String::Create( getMemberDesc()->getUserName() ) );
      return result;
    }
  };
};
