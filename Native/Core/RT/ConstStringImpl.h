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
      
    public:
          
      // Impl
    
      virtual void setData( void const *src, void *dst ) const;
      virtual void disposeData( void *data ) const;
      virtual std::string descData( void const *data ) const;
      virtual void const *getDefaultData() const;
      
      virtual RC::Handle<JSON::Value> getJSONValue( void const *data ) const;
      virtual void setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &value, void *data ) const;
      
      virtual Util::Encoder &encode( Util::Encoder &encoder, void const *data ) const;
      virtual Util::Decoder &decode( Util::Decoder &decoder, void *data ) const;

      virtual bool isEquivalentTo( RC::ConstHandle<RT::Impl> const &impl ) const;
      virtual bool isShallow() const;
      
      // ConstStringImpl
      
      size_t getLength() const
      {
        return getSize();
      }
            
    protected:
    
      ConstStringImpl( std::string const &codeName, size_t length );
      ~ConstStringImpl();
      
    private:
    
      void *m_defaultData;
   };
  };
};

#endif //_FABRIC_RT_CONST_STRING_IMPL_H
