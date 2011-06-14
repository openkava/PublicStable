#include "ByteDesc.h"

#include <Fabric/Core/RT/ByteImpl.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>

namespace Fabric
{
  namespace RT
  {
    ByteDesc::ByteDesc( std::string const &name, RC::ConstHandle<ByteImpl> const &byteImpl )
      : Desc( name, byteImpl )
      , m_byteImpl( byteImpl )
    {
    }

    uint8_t ByteDesc::getValue( void const *data ) const
    {
      return m_byteImpl->getValue( data );
    }
    
    void ByteDesc::setValue( uint8_t value, void *data ) const
    {
      m_byteImpl->setValue( value, data );
    }
    
    RC::Handle<JSON::Object> ByteDesc::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = Desc::jsonDesc();
      result->set( "internalType", JSON::String::Create("ui8") );
      return result;
    }
  };
};
