/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "StringDesc.h"
#include "StringImpl.h"
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace RT
  {
    StringDesc::StringDesc( std::string const &name, RC::ConstHandle<StringImpl> const &stringImpl )
      : ComparableDesc( name, stringImpl )
      , m_stringImpl( stringImpl )
    {
    }

    char const *StringDesc::getValueData( void const *src ) const
    {
      return m_stringImpl->getValueData( src );
    }
    
    size_t StringDesc::getValueLength( void const *src ) const
    {
      return m_stringImpl->getValueLength( src );
    }
    
    void StringDesc::setValue( char const *cStr, size_t length, void *dst ) const
    {
      return StringImpl::SetValue( cStr, length, dst );
    }
    
    void StringDesc::jsonDesc( Util::JSONObjectGenerator &resultJOG ) const
    {
      Desc::jsonDesc( resultJOG );
      resultJOG.makeMember( "internalType" ).makeString( "string" );
    }
  };
};
