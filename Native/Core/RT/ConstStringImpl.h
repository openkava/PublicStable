/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_CONST_STRING_IMPL_H
#define _FABRIC_RT_CONST_STRING_IMPL_H

#include <Fabric/Core/RT/Impl.h>

namespace Fabric
{
  namespace RT
  {
    class ConstStringImpl : public Impl
    {
      friend class Manager;
      friend class Impl;
      
      struct bits_t
      {
        char const *data;
        size_t length;
      };
      
    public:
          
      // Impl
    
      virtual void setData( void const *src, void *dst ) const;
      virtual void disposeData( void *data ) const;
      virtual std::string descData( void const *data ) const;
      virtual void const *getDefaultData() const;
      
      virtual RC::Handle<JSON::Value> getJSONValue( void const *data ) const;
      virtual void setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &value, void *data ) const;

      virtual bool isEquivalentTo( RC::ConstHandle<RT::Impl> const &impl ) const;
      virtual bool isShallow() const;
      
      // ConstStringImpl

      char const *getValueData( void const *src ) const
      {
        bits_t const *bits = static_cast<bits_t const *>( src );
        return bits->data;
      }
      
      size_t getValueLength( void const *src ) const
      {
        bits_t const *bits = static_cast<bits_t const *>( src );
        return bits->length;
      }
      
      std::string toString( void const *data ) const;
            
    protected:
    
      ConstStringImpl( std::string const &codeName );
      ~ConstStringImpl();
   };
  };
};

#endif //_FABRIC_RT_CONST_STRING_IMPL_H
