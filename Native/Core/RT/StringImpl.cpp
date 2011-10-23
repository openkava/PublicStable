/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "StringImpl.h"

#include <Fabric/Base/JSON/String.h>
#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Core/Util/Decoder.h>
#include <Fabric/Base/Util/SimpleString.h>
#include <Fabric/Core/Util/JSONGenerator.h>

#include <llvm/Intrinsics.h>

namespace Fabric
{
  namespace RT
  {
    StringImpl::StringImpl( std::string const &codeName )
      : ComparableImpl( codeName, DT_STRING )
    {
      setSize( sizeof(bits_t *) );
    }
    
    void const *StringImpl::getDefaultData() const
    {
      static bits_t *defaultData = 0;
      return &defaultData;
    }
    
    void StringImpl::setData( void const *src, void *dst ) const
    {
      FABRIC_ASSERT( src );
      FABRIC_ASSERT( dst );
      
      bits_t *srcBits = *static_cast<bits_t * const *>( src );
      bits_t *&dstBits = *static_cast<bits_t **>( dst );
      if ( dstBits != srcBits )
      {
        if ( dstBits && dstBits->refCount.decrementAndGetValue() == 0 )
          free( dstBits );
        
        dstBits = srcBits;
        
        if ( dstBits )
          dstBits->refCount.increment();
      }
    }
     
    RC::Handle<JSON::Value> StringImpl::getJSONValue( void const *src ) const
    {
      return JSON::String::Create( getValueData(src), getValueLength(src) );
    }
    
    void StringImpl::generateJSON( void const *data, Util::JSONGenerator &jsonGenerator ) const
    {
      jsonGenerator.makeString( getValueData(data), getValueLength(data) );
    }
    
    void StringImpl::setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *dst ) const
    {
      if ( !jsonValue->isString() )
        throw Exception( "JSON value is not a string" );
      RC::ConstHandle<JSON::String> jsonString = RC::ConstHandle<JSON::String>::StaticCast( jsonValue );
      SetValue( jsonString->data(), jsonString->length(), dst );
    }

    void StringImpl::disposeDatasImpl( void *dst, size_t count, size_t stride ) const
    {
      uint8_t *data = static_cast<uint8_t *>( dst );
      uint8_t * const dataEnd = data + count * stride;
      while ( data != dataEnd )
      {
        bits_t *bits = *reinterpret_cast<bits_t **>( data );
        if ( bits && bits->refCount.decrementAndGetValue() == 0 )
          free( bits );
        data += stride;
      }
    }
    
    std::string StringImpl::descData( void const *data ) const
    {
      std::string result = "\"";
      size_t length = getValueLength(data);
      if ( length > 64 )
      {
        result.append( getValueData(data), 64 );
        result += "...";
      }
      else if ( length )
        result += getValueData(data);
      result += "\"";
      return result;
    }
    
    bool StringImpl::isEquivalentTo( RC::ConstHandle<Impl> const &impl ) const
    {
      return isString( impl->getType() );
    }
    
    bool StringImpl::isShallow() const
    {
      return false;
    }

    size_t StringImpl::hash( void const *data ) const
    {
      // [pzion 20111014] This hash function is referred to as djb2.
      // Thanks Manon!!
      size_t stringLength = getValueLength( data );
      uint8_t const *stringData = reinterpret_cast<uint8_t const *>( getValueData( data ) );
      size_t result = 5381;
      while ( stringLength > 0 )
      {
        result = ((result << 5) + result) ^ uint32_t(*stringData);
        --stringLength;
        ++stringData;
      }
      return result;
    }
    
    int StringImpl::compare( void const *lhsData, void const *rhsData ) const
    {
      char const *lhsStringData = getValueData( lhsData );
      size_t lhsStringLength = getValueLength( lhsData );
      char const *rhsStringData = getValueData( rhsData );
      size_t rhsStringLength = getValueLength( rhsData );
      size_t minStringLength = std::min( lhsStringLength, rhsStringLength );
      int cmp = memcmp( lhsStringData, rhsStringData, minStringLength );
      if ( cmp == 0 )
      {
        if ( lhsStringLength < rhsStringLength )
          return -1;
        else if ( lhsStringLength == rhsStringLength )
          return 0;
        else return 1;
      }
      else return cmp;
    }
  };
};
