/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_OPAQUE_IMPL_H
#define _FABRIC_RT_OPAQUE_IMPL_H

#include <Fabric/Core/RT/Impl.h>

namespace Fabric
{
  namespace RT
  {
    class OpaqueImpl : public Impl
    {
      friend class Manager;
      
    public:
    
      // Impl
    
      virtual void setData( void const *value, void *data ) const;
      virtual std::string descData( void const *data ) const;
      virtual void const *getDefaultData() const;
      virtual void disposeData( void *data ) const;
      
      virtual RC::Handle<JSON::Value> getJSONValue( void const *data ) const;
      virtual void setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &value, void *data ) const;
      
      virtual bool isShallow() const;
      virtual bool isEquivalentTo( RC::ConstHandle<Impl> const &impl ) const;
    
    protected:

      OpaqueImpl( std::string const &codeName, size_t size );
      ~OpaqueImpl();
      
    private:
    
      void *m_defaultData;
    };
  };
};

#endif //_FABRIC_RT_OPAQUE_IMPL_H
