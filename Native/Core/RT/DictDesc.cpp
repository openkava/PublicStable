/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "DictDesc.h"
#include "DictImpl.h"
#include "ComparableDesc.h"

#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace RT
  {
    DictDesc::DictDesc(
      std::string const &name,
      RC::ConstHandle<DictImpl> const &dictImpl,
      RC::ConstHandle<ComparableDesc> const &keyDesc,
      RC::ConstHandle<Desc> const &valueDesc
      )
      : Desc( name, dictImpl )
      , m_dictImpl( dictImpl )
      , m_keyDesc( keyDesc )
      , m_valueDesc( valueDesc )
    {
    }
    
    void const *DictDesc::getImmutable( void const *data, void const *keyData ) const
    {
      return m_dictImpl->getImmutable( data, keyData );
    }
    
    void *DictDesc::getMutable( void *data, void const *keyData ) const
    {
      return m_dictImpl->getMutable( data, keyData );
    }
    
    size_t DictDesc::getSize( void const *data ) const
    {
      return m_dictImpl->getSize( data );
    }    
        
    RC::Handle<JSON::Object> DictDesc::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = Desc::jsonDesc();
      result->set( "internalType", JSON::String::Create("dict") );
      result->set( "keyType", JSON::String::Create( m_keyDesc->getUserName() ) );
      result->set( "valueType", JSON::String::Create( m_valueDesc->getUserName() ) );
      return result;
    }
  };
};
