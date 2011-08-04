#include "SizeImpl.h"

#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Core/Util/Decoder.h>
#include <Fabric/Core/Util/SimpleString.h>

namespace Fabric
{
  

  namespace RT
  {
    SizeImpl::SizeImpl( std::string const &codeName )
      : SimpleImpl( codeName, DT_SIZE, sizeof(size_t) )
    {
    }
    
    void const *SizeImpl::getDefaultData() const
    {
      static size_t const defaultData = 0;
      return &defaultData;
    }
    
    void SizeImpl::setData( void const *src, void *dst ) const
    {
      setValue( getValue( src ), dst );
    }
   
    RC::Handle<JSON::Value> SizeImpl::getJSONValue( void const *data ) const
    {
      size_t value = getValue(data);
      if ( size_t( int32_t( value ) ) != value )
        throw Exception( "value is too large for JSON representation" );
      return JSON::Integer::Create( value );
    }

    void SizeImpl::setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *dst ) const
    {
      if ( !jsonValue->isInteger() )
        throw Exception("value is not integer");
      RC::ConstHandle<JSON::Integer> jsonInteger = RC::ConstHandle<JSON::Integer>::StaticCast( jsonValue );
      setValue( jsonInteger->value(), dst );
    }
    
    std::string SizeImpl::descData( void const *data ) const
    {
      char buf[21];
      snprintf( buf, 21, "%u", (unsigned)getValue(data) );
      return std::string( buf );
    }
  }; // namespace RT
}; // namespace FABRIC
