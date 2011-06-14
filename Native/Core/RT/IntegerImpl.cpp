#include "IntegerImpl.h"

#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Core/Util/Decoder.h>
#include <Fabric/Core/Util/SimpleString.h>
#include <Fabric/Core/Util/Format.h>

namespace Fabric
{
  

  namespace RT
  {
    IntegerImpl::IntegerImpl( std::string const &codeName )
      : SimpleImpl( codeName, DT_INTEGER, sizeof(int32_t) )
    {
    }
    
    void const *IntegerImpl::getDefaultData() const
    {
      static int32_t const defaultData = 0;
      return &defaultData;
    }
    
    RC::Handle<JSON::Value> IntegerImpl::getJSONValue( void const *data ) const
    {
      return JSON::Integer::Create( getValue(data) );
    }

    void IntegerImpl::setData( void const *src, void *dst ) const
    {
      setValue( getValue( src ), dst );
    }
   
    void IntegerImpl::setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *dst ) const
    {
      if ( !jsonValue->isInteger() )
        throw Exception("value is not integer");
      RC::ConstHandle<JSON::Integer> jsonInteger = RC::ConstHandle<JSON::Integer>::StaticCast( jsonValue );
      setValue( jsonInteger->value(), dst );
    }

    Util::Encoder &IntegerImpl::encode( Util::Encoder &encoder, void const *src ) const
    {
      return encoder.put( getValue( src ) );
    }
    
    Util::Decoder &IntegerImpl::decode( Util::Decoder &decoder, void *dst ) const
    {
      int32_t int32Data;
      decoder.get( int32Data );
      setValue( int32Data, dst );
      return decoder;
    }  
    
    std::string IntegerImpl::descData( void const *data ) const
    {
      return _(data);
    }
  }; // namespace RT
}; // namespace FABRIC
