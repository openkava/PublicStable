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
    ConstStringImpl::ConstStringImpl( std::string const &codeName, size_t length )
      : Impl( codeName, DT_CONST_STRING )
    {
      setSize( length );

      m_defaultData = malloc( getSize() );
      memset( m_defaultData, 0, getSize() );
    }
    
    ConstStringImpl::~ConstStringImpl()
    {
      disposeData( m_defaultData );
      free( m_defaultData );
    }
    
    void const *ConstStringImpl::getDefaultData() const
    {
      return m_defaultData;
    }

    void ConstStringImpl::setData( void const *src, void *dst ) const
    {
      memcpy( dst, src, getSize() );
    }

    RC::Handle<JSON::Value> ConstStringImpl::getJSONValue( void const *data ) const
    {
      return JSON::String::Create( static_cast<char const *>(data), getSize() );
    }
    
    void ConstStringImpl::setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *data ) const
    {
      if ( !jsonValue->isString() )
        throw Exception( "JSON value is not string" );
      RC::ConstHandle<JSON::String> jsonString = RC::ConstHandle<JSON::String>::StaticCast( jsonValue );

      if ( jsonString->length() != getSize() )
        throw Exception( "JSON string is of wrong length (expected " + _(getSize()) + ", actual " + _(jsonString->length()) + ")" );

      memcpy( data, jsonString->data(), getSize() );
    }
    
    Util::Encoder &ConstStringImpl::encode( Util::Encoder &encoder, void const *data ) const
    {
      return encoder.putData( data, getSize(), false );
    }
    
    Util::Decoder &ConstStringImpl::decode( Util::Decoder &decoder, void *data ) const
    {
      return decoder.getData( data, getSize(), false );
    }  

    void ConstStringImpl::disposeData( void *data ) const
    {
    }
    
    bool ConstStringImpl::isEquivalentTo( RC::ConstHandle<Impl> const &impl ) const
    {
      if ( !isConstString( impl->getType() ) )
        return false;
      RC::ConstHandle<ConstStringImpl> constStringImpl = RC::ConstHandle<ConstStringImpl>::StaticCast( impl );
      
      return getSize() == constStringImpl->getSize();
    }
    
    bool ConstStringImpl::isShallow() const
    {
      return true;
    }
    
    std::string ConstStringImpl::descData( void const *data ) const
    {
      char const *stringData = static_cast<char const *>(data);
      std::string result = "\"";
      if ( getLength() > 64 )
      {
        result.append( stringData, 64 );
        result += "...";
      }
      else result.append( stringData, getLength() );
      result += "\"";
      return result;
    }
  };
};
