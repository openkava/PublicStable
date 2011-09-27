/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/RT/ConstStringImpl.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Core/Util/Decoder.h>
#include <Fabric/Core/Util/Format.h>

namespace Fabric
{
  namespace RT
  {
    ConstStringImpl::ConstStringImpl( std::string const &codeName )
      : Impl( codeName, DT_CONST_STRING )
    {
      setSize( sizeof(bits_t) );
    }
    
    ConstStringImpl::~ConstStringImpl()
    {
    }
    
    void const *ConstStringImpl::getDefaultData() const
    {
      static const bits_t defaultBits = { 0, 0 };
      return &defaultBits;
    }

    void ConstStringImpl::setData( void const *src, void *dst ) const
    {
      memcpy( dst, src, getAllocSize() );
    }

    RC::Handle<JSON::Value> ConstStringImpl::getJSONValue( void const *data ) const
    {
      bits_t const *bits = static_cast<bits_t const *>( data );
      return JSON::String::Create( bits->data, bits->length );
    }
    
    void ConstStringImpl::setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *data ) const
    {
      throw Exception( "cannot set constant string from a JSON value" );
    }

    void ConstStringImpl::disposeDataImpl( void *data ) const
    {
    }
    
    bool ConstStringImpl::isEquivalentTo( RC::ConstHandle<Impl> const &impl ) const
    {
      return isConstString( impl->getType() );
    }
    
    bool ConstStringImpl::isShallow() const
    {
      return true;
    }
    
    std::string ConstStringImpl::descData( void const *data ) const
    {
      bits_t const *bits = static_cast<bits_t const *>( data );
      std::string result = "\"";
      if ( bits->length > 64 )
      {
        result.append( bits->data, 64 );
        result += "...";
      }
      else result.append( bits->data, bits->length );
      result += "\"";
      return result;
    }

    std::string ConstStringImpl::toString( void const *data ) const
    {
      bits_t const *bits = static_cast<bits_t const *>( data );
      return std::string( bits->data, bits->length );
    }
  };
};
