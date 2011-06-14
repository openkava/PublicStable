#include "OpaqueImpl.h"

#include <Fabric/Base/JSON/Null.h>
#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Core/Util/Decoder.h>
#include <Fabric/Core/Util/Hex.h>
#include <Fabric/Core/Util/SimpleString.h>

namespace Fabric
{
  

  namespace RT
  {
    OpaqueImpl::OpaqueImpl( std::string const &codeName, size_t size )
      : Impl( codeName, DT_OPAQUE )
    {
      setSize( size );
      m_defaultData = malloc( size );
      memset( m_defaultData, 0, size );
    }
    
    OpaqueImpl::~OpaqueImpl()
    {
      free( m_defaultData );
    }
    
    void const *OpaqueImpl::getDefaultData() const
    {
      return m_defaultData;
    }
    
    void OpaqueImpl::setData( void const *src, void *dst ) const
    {
      memcpy( dst, src, getSize() );
    }
   
    void OpaqueImpl::disposeData( void *data ) const
    {
    }
    
    RC::Handle<JSON::Value> OpaqueImpl::getJSONValue( void const *data ) const
    {
      return JSON::Null::Create();
    }

    void OpaqueImpl::setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *dst ) const
    {
      if ( !jsonValue->isNull() )
        throw Exception("value is not null");
      memset( dst, 0, getSize() );
    }

    Util::Encoder &OpaqueImpl::encode( Util::Encoder &encoder, void const *src ) const
    {
      return encoder;
    }
    
    Util::Decoder &OpaqueImpl::decode( Util::Decoder &decoder, void *dst ) const
    {
      setData( m_defaultData, dst );
      return decoder;
    }  
    
    std::string OpaqueImpl::descData( void const *src ) const
    {
      return "Opaque<" + Util::hexBuf( getSize(), src ) + ">";
    }

    bool OpaqueImpl::isShallow() const
    {
      return true;
    }
    
    bool OpaqueImpl::isEquivalentTo( RC::ConstHandle<Impl> const &impl ) const
    {
      if ( !isOpaque( impl->getType() ) )
        return false;
      return getSize() == impl->getSize();
    }
  }; // namespace RT
}; // namespace FABRIC
