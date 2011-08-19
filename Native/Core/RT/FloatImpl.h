/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_FLOAT_IMPL_H
#define _FABRIC_RT_FLOAT_IMPL_H

#include <Fabric/Core/RT/NumericImpl.h>
#include <Fabric/Core/Util/Math.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Base/JSON/Null.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Base/JSON/Scalar.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace RT
  {
    class FloatImpl : public NumericImpl
    {
    public:
    
      // NumericImpl

      virtual bool isFloat() const { return true; }
    
    protected:
    
      FloatImpl( std::string const &codeName, size_t size )
        : NumericImpl( codeName, DT_FLOAT, size )
      {
      }
    };
    
    template<typename T> class FloatImplT : public FloatImpl
    {
      friend class Manager;
      
    public:

      // Impl
      
      void const *getDefaultData() const
      {
        static T const defaultData = 0.0;
        return &defaultData;
      }
      
      void setData( void const *src, void *dst ) const
      {
        setValue( getValue( src ), dst );
      }
        
      RC::Handle<JSON::Value> getJSONValue( void const *src ) const
      {
        T value = getValue( src );
        double doubleValue = double(value);
        if ( Util::isnan( doubleValue ) || Util::isinf( doubleValue ) )
          return JSON::Null::Create();
        else return JSON::Scalar::Create( doubleValue );
      }
      
      void setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *dst ) const
      {
        if ( jsonValue->isInteger() )
        {
          RC::ConstHandle<JSON::Integer> jsonInteger = RC::ConstHandle<JSON::Integer>::StaticCast( jsonValue );
          int32_t int32Value = jsonInteger->value();
          T tValue = T( int32Value );
          setValue( tValue, dst );
        }
        else if ( jsonValue->isScalar() )
        {
          RC::ConstHandle<JSON::Scalar> jsonScalar = RC::ConstHandle<JSON::Scalar>::StaticCast( jsonValue );
          double doubleValue = jsonScalar->value();
          T tValue = T( doubleValue );
          setValue( tValue, dst );
        }
        else if ( jsonValue->isNull() )
        {
          setValue( Util::nanValue<T>(), dst );
        }
        else throw Exception( "value is not scalar, integer or null" );
      }
      
      std::string descData( void const *data ) const
      {
        return _( getValue( data ) );
      }
      
      // FloatImplT
      
      T getValue( void const *data ) const
      {
        return *static_cast<T const *>( data );
      }
      
      void setValue( T t, void *data ) const
      {
        *static_cast<T *>( data ) = t;
      }
    
    protected:

      FloatImplT( std::string const &codeName )
        : FloatImpl( codeName, sizeof(T) )
      {
      }
    };
    
    class FP32Impl : public FloatImplT<float>
    {
      friend class Manager;
      
    protected:
    
      FP32Impl( std::string const &codeName )
        : FloatImplT<float>( codeName )
      {
      }
    };
  };
};

#endif //_FABRIC_RT_FLOAT_IMPL_H
