/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "BooleanDesc.h"

#include <Fabric/Core/RT/BooleanImpl.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace RT
  {
    BooleanDesc::BooleanDesc( std::string const &name, RC::ConstHandle<BooleanImpl> const &booleanImpl )
      : ComparableDesc( name, booleanImpl )
      , m_booleanImpl( booleanImpl )
    {
    }

    bool BooleanDesc::getValue( void const *data ) const
    {
      return m_booleanImpl->getValue( data );
    }
    
    void BooleanDesc::setValue( bool value, void *data ) const
    {
      m_booleanImpl->setValue( value, data );
    }
    
    void BooleanDesc::jsonDesc( Util::JSONObjectGenerator &resultJOG ) const
    {
      ComparableDesc::jsonDesc( resultJOG );
      resultJOG.makeMember( "internalType" ).makeString( "boolean" );
    }
  };
};
