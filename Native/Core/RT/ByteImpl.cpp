#include "ByteImpl.h"

#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Core/Util/Decoder.h>
#include <Fabric/Core/Util/SimpleString.h>
#include <Fabric/Core/Util/Format.h>

namespace Fabric
{
  

  namespace RT
  {
    ByteImpl::ByteImpl( std::string const &codeName )
      : SimpleImpl( codeName, DT_BYTE, sizeof(uint8_t) )
    {
    }
    
    void const *ByteImpl::getDefaultData() const
    {
      static uint8_t const defaultData = 0;
      return &defaultData;
    }
    
    void ByteImpl::setData( void const *src, void *dst ) const
    {
      setValue( getValue( src ), dst );
    }
    
    std::string ByteImpl::descData( void const *data ) const
    {
      return _( getValue( data ) );
    }

    RC::Handle<JSON::Value> ByteImpl::getJSONValue( void const *data ) const
    {
      return JSON::Integer::Create( getValue(data) );
    }
   
    void ByteImpl::setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *dst ) const
    {
      if ( !jsonValue->isInteger() )
        throw Exception("value is not integer");
      RC::ConstHandle<JSON::Integer> jsonInteger = RC::ConstHandle<JSON::Integer>::StaticCast( jsonValue );
      setValue( (uint8_t)jsonInteger->value(), dst );
    }
  };
};
