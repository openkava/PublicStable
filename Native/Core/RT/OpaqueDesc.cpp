/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "OpaqueDesc.h"
#include <Fabric/Core/RT/OpaqueImpl.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace RT
  {
    OpaqueDesc::OpaqueDesc( std::string const &name, RC::ConstHandle<OpaqueImpl> const &opaqueImpl )
      : Desc( name, opaqueImpl )
      , m_opaqueImpl( opaqueImpl )
    {
    }
    
    void OpaqueDesc::jsonDesc( Util::JSONObjectGenerator &resultJOG ) const
    {
      Desc::jsonDesc( resultJOG );
      resultJOG.makeMember( "internalType" ).makeString( "opaque" );
    }
  };
};
