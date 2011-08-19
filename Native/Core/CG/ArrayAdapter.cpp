/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "ArrayAdapter.h"
#include "Manager.h"

#include <Fabric/Core/RT/ArrayDesc.h>

namespace Fabric
{
  namespace CG
  {
    ArrayAdapter::ArrayAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::ArrayDesc> const &arrayDesc, Flags flags )
      : Adapter( manager, arrayDesc, flags )
      , m_arrayDesc( arrayDesc )
      , m_memberAdapter( manager->getAdapter( arrayDesc->getMemberDesc() ) )
    {
    }
    
    std::string ArrayAdapter::toString( void const *data ) const
    {
      size_t numMembers = m_arrayDesc->getNumMembers( data );
      std::string result = "[";
      for ( size_t i=0; i<numMembers; ++i )
      {
        if ( i > 0 )
          result += ",";
        result += m_memberAdapter->toString( m_arrayDesc->getMemberData( data, i ) );
      }
      return result + "]";
    }
  };
};
