#ifndef _FABRIC_RT_BYTE_IMPL_H
#define _FABRIC_RT_BYTE_IMPL_H

#include <Fabric/Core/RT/SimpleImpl.h>

namespace Fabric
{
  namespace RT
  {
    class ByteImpl : public SimpleImpl
    {
      friend class Manager;
      
    public:
    
      // Impl
      
      virtual void setData( void const *value, void *data ) const;
      virtual std::string descData( void const *data ) const;
      virtual void const *getDefaultData() const;

      virtual RC::Handle<JSON::Value> getJSONValue( void const *data ) const;
      virtual void setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &value, void *data ) const;
    
      // ByteImpl
    
      uint8_t getValue( void const *data ) const
      {
        return *static_cast<uint8_t const *>(data);
      }
      
      void setValue( uint8_t value, void *data ) const
      {
        *static_cast<uint8_t *>(data) = value;
      }

    protected:
    
      ByteImpl( std::string const &codeName );
    };
  };
};

#endif //_FABRIC_RT_BYTE_IMPL_H
