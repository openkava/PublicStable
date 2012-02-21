/*
 *  Copyright 2010-2012 Fabric Technologies Inc. All rights reserved.
 */

#include "OpaqueDesc.h"
#include <Fabric/Core/RT/OpaqueImpl.h>
#include <Fabric/Base/JSON/Encoder.h>

namespace Fabric
{
  namespace RT
  {
    OpaqueDesc::OpaqueDesc( std::string const &name, RC::ConstHandle<OpaqueImpl> const &opaqueImpl )
      : Desc( name, opaqueImpl )
      , m_opaqueImpl( opaqueImpl )
    {
    }
    
    void OpaqueDesc::jsonDesc( JSON::ObjectEncoder &resultObjectEncoder ) const
    {
      Desc::jsonDesc( resultObjectEncoder );
      resultObjectEncoder.makeMember( "internalType" ).makeString( "opaque" );
    }
  };
};
