/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "StringDesc.h"
#include "StringImpl.h"
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>

namespace Fabric
{
  namespace RT
  {
    StringDesc::StringDesc( std::string const &name, RC::ConstHandle<StringImpl> const &stringImpl )
      : Desc( name, stringImpl )
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
    
    RC::Handle<JSON::Object> StringDesc::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = Desc::jsonDesc();
      result->set( "internalType", JSON::String::Create("string") );
      return result;
    }
  };
};
