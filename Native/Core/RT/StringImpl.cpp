#include "StringImpl.h"

#include <Fabric/Base/JSON/String.h>
#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Core/Util/Decoder.h>
#include <Fabric/Core/Util/SimpleString.h>

#include <llvm/Intrinsics.h>

namespace Fabric
{
  

  namespace RT
  {
    StringImpl::StringImpl( std::string const &codeName )
      : Impl( codeName, DT_STRING )
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
    
    void StringImpl::setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *dst ) const
    {
      if ( !jsonValue->isString() )
        throw Exception( "JSON value is not a string" );
      RC::ConstHandle<JSON::String> jsonString = RC::ConstHandle<JSON::String>::StaticCast( jsonValue );
      SetValue( jsonString->data(), jsonString->length(), dst );
    }

    Util::Encoder &StringImpl::encode( Util::Encoder &encoder, void const *src ) const
    {
      FABRIC_ASSERT( src );
      bits_t const *bits = reinterpret_cast<bits_t const *>( src );
      if ( bits )
        return encoder.putString( bits->cStr, bits->length );
      
      return encoder.putString( 0, 0 );
    }
    
    Util::Decoder &StringImpl::decode( Util::Decoder &decoder, void *dst ) const
    {
      FABRIC_ASSERT( dst );
      
      size_t length;
      decoder.getStringLength( length );

      Prepare( length, false, dst );
      bits_t *bits = *reinterpret_cast<bits_t **>( dst );
      if ( length )
      {
        FABRIC_ASSERT( bits && bits->refCount.getValue() == 1 && length+1 <= bits->allocSize );
        decoder.getStringData( bits->cStr, length );
        bits->length = length;
      }
      else FABRIC_ASSERT( !bits );

      return decoder;
    }  

    void StringImpl::disposeData( void *dst ) const
    {
      bits_t *bits = *reinterpret_cast<bits_t **>( dst );
      if ( bits && bits->refCount.decrementAndGetValue() == 0 )
        free( bits );
    }
    
    std::string StringImpl::descData( void const *data ) const
    {
      std::string result = "\"";
      size_t length = getValueLength(data);
      if ( length > 64 )
      {
        result.append( length, 64 );
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
  };
};
