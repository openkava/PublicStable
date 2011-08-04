#include "FloatImpl.h"

#include <Fabric/Base/JSON/Scalar.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Base/JSON/Null.h>
#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Core/Util/Decoder.h>
#include <Fabric/Core/Util/SimpleString.h>
#include <Fabric/Core/Util/Math.h>

#include <cmath>

namespace Fabric
{
  

  namespace RT
  {
    FloatImpl::FloatImpl( std::string const &codeName )
      : SimpleImpl( codeName, DT_SCALAR, sizeof(float) )
    {
    }
    
    void const *FloatImpl::getDefaultData() const
    {
      static float const defaultData = 0.0f;
      return &defaultData;
    }
    
    void FloatImpl::setData( void const *src, void *dst ) const
    {
      setValue( getValue( src ), dst );
    }
      
    RC::Handle<JSON::Value> FloatImpl::getJSONValue( void const *src ) const
    {
      float value = getValue( src );
      if ( Util::isnan( value ) || Util::isinf( value ) )
        return JSON::Null::Create();
      else return JSON::Scalar::Create( value );
    }
    
    void FloatImpl::setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *dst ) const
    {
      if ( jsonValue->isScalar() )
      {
        RC::ConstHandle<JSON::Scalar> jsonScalar = RC::ConstHandle<JSON::Scalar>::StaticCast( jsonValue );
        setValue( float( jsonScalar->value() ), dst );
      }
      else if ( jsonValue->isInteger() )
      {
        RC::ConstHandle<JSON::Integer> jsonInteger = RC::ConstHandle<JSON::Integer>::StaticCast( jsonValue );
        setValue( float( jsonInteger->value() ), dst );
      }
      else if ( jsonValue->isNull() )
      {
        setValue( Util::kNAN, dst );
      }
      else throw Exception("value is not scalar, integer or null");
    }
    
    std::string FloatImpl::descData( void const *data ) const
    {
      char buf[32];
      sprintf( buf, "%g", (float)getValue(data) );
      return std::string( buf );
    }
  };
};
