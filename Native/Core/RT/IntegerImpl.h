/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_INTEGER_IMPL_H
#define _FABRIC_RT_INTEGER_IMPL_H

#include <Fabric/Core/RT/SimpleImpl.h>
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
    class IntegerImpl : public NumericImpl
    {
    public:
    
      // NumericImpl

      virtual bool isInteger() const { return true; }
      
      // IntegerImpl
      
      virtual bool isSigned() const { return false; }
    
    protected:
    
      IntegerImpl( std::string const &codeName, size_t size )
        : NumericImpl( codeName, DT_INTEGER, size )
      {
      }
    };
    
    template<typename T> class IntegerImplT : public IntegerImpl
    {
    public:

      // Impl
      
      void const *getDefaultData() const
      {
        static T const defaultData = 0;
        return &defaultData;
      }
      
      void setData( void const *src, void *dst ) const
      {
        setValue( getValue( src ), dst );
      }
        
      RC::Handle<JSON::Value> getJSONValue( void const *src ) const
      {
        T value = getValue( src );
        if ( T( int32_t( value ) ) != value )
          throw Exception( "value is too large for JSON representation" );
        return JSON::Integer::Create( int32_t( value ) );
      }
      
      void setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *dst ) const
      {
        if ( jsonValue->isInteger() )
        {
          RC::ConstHandle<JSON::Integer> jsonInteger = RC::ConstHandle<JSON::Integer>::StaticCast( jsonValue );
          int32_t int32Value = jsonInteger->value();
          T tValue = T( int32Value );
          if ( int32_t( tValue ) != int32Value )
            throw Exception( "value is out of range" );
          setValue( tValue, dst );
        }
        else throw Exception("value is not integer");
      }
      
      std::string descData( void const *data ) const
      {
        return _( getValue( data ) );
      }
    
      // ComparableImpl
    
      virtual uint32_t hash( void const *data ) const
      {
        return uint32_t( getValue( data ) );
      }
      
      virtual int compare( void const *lhsData, void const *rhsData ) const
      {
        T lhsValue = getValue( lhsData );
        T rhsValue = getValue( rhsData );
        if ( lhsValue < rhsValue )
          return -1;
        else if ( lhsValue == rhsValue )
          return 0;
        else return 1;
      }
      
      // IntegerImpl

      virtual bool isSigned() const
      {
        return T(-1) < 0;
      }
      
      // IntegerImplT
      
      T getValue( void const *data ) const
      {
        return *static_cast<T const *>( data );
      }
      
      void setValue( T t, void *data ) const
      {
        *static_cast<T *>( data ) = t;
      }
    
    protected:

      IntegerImplT( std::string const &codeName )
        : IntegerImpl( codeName, sizeof(T) )
      {
      }
    };
    
    class ByteImpl : public IntegerImplT<uint8_t>
    {
      friend class Manager;
      
    protected:
    
      ByteImpl( std::string const &codeName )
        : IntegerImplT<uint8_t>( codeName )
      {
      }
    };
    
    class SI32Impl : public IntegerImplT<int32_t>
    {
      friend class Manager;
      
    protected:
    
      SI32Impl( std::string const &codeName )
        : IntegerImplT<int32_t>( codeName )
      {
      }
    };
    
    class SizeImpl : public IntegerImplT<size_t>
    {
      friend class Manager;
      
    protected:
    
      SizeImpl( std::string const &codeName )
        : IntegerImplT<size_t>( codeName )
      {
      }
    };
  };
};

#endif //_FABRIC_RT_INTEGER_IMPL_H
