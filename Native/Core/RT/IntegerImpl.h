#ifndef _FABRIC_RT_INTEGER_IMPL_H
#define _FABRIC_RT_INTEGER_IMPL_H

#include <Fabric/Core/RT/SimpleImpl.h>

namespace Fabric
{
  namespace RT
  {
    class IntegerImpl : public SimpleImpl
    {
      friend class Manager;
      
    public:
    
      // Impl
    
      virtual void setData( void const *value, void *data ) const;
      virtual std::string descData( void const *data ) const;
      virtual void const *getDefaultData() const;
      
      virtual RC::Handle<JSON::Value> getJSONValue( void const *data ) const;
      virtual void setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &value, void *data ) const;
    
      // IntegerImpl
    
      int32_t getValue( void const *data ) const
      {
        return *static_cast<int32_t const *>(data);
      }  
      
      void setValue( int32_t value, void *data ) const
      {
        *static_cast<int32_t *>(data) = value;
      }

    protected:

      IntegerImpl( std::string const &codeName );
    };
  };
};

#endif //_FABRIC_RT_INTEGER_IMPL_H
