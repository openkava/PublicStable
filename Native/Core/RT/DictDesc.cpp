/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "DictDesc.h"
#include "DictImpl.h"
#include "ComparableDesc.h"

#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Core/Util/JSONGenerator.h>

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
    
    RC::ConstHandle<RT::DictImpl> DictDesc::getImpl() const
    {
      return m_dictImpl;
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
        
    void DictDesc::jsonDesc( Util::JSONObjectGenerator &resultJOG ) const
    {
      Desc::jsonDesc( resultJOG );
      resultJOG.makeMember( "internalType" ).makeString( "dict" );
      resultJOG.makeMember( "keyType" ).makeString( m_keyDesc->getUserName() );
      resultJOG.makeMember( "valueType" ).makeString( m_valueDesc->getUserName() );
    }

    RC::ConstHandle<ComparableDesc> DictDesc::getKeyDesc() const
    {
      return m_keyDesc;
    }
    
    RC::ConstHandle<Desc> DictDesc::getValueDesc() const
    {
      return m_valueDesc;
    }
      
    std::string DictDesc::descData( void const *data, size_t limit ) const
    {
      return m_dictImpl->descData( data, limit );
    }
  };
};
