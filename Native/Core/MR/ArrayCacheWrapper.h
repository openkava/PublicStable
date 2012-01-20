/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_ARRAY_CACHE_WRAPPER_H
#define _FABRIC_MR_ARRAY_CACHE_WRAPPER_H

#include <Fabric/Core/MR/ArrayProducerWrapper.h>

namespace Fabric
{
  namespace Util
  {
    class JSONObjectGenerator;
  }
  
  namespace MR
  {
    class ArrayProducerWrapper;
    class ArrayCache;
    
    class ArrayCacheWrapper : public ArrayProducerWrapper
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
    public:
    
      static RC::Handle<ArrayCacheWrapper> Create(
        RC::ConstHandle<ArrayProducerWrapper> const &inputArrayProducer
        );
      
      virtual RC::ConstHandle<ArrayProducer> getUnwrapped() const;
      
      // Virtual functions: ArrayProducer
    
    protected:
        
      ArrayCacheWrapper(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<ArrayProducerWrapper> const &inputArrayProducer
        );
    
      virtual char const *getKind() const;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const;
    
    private:
    
      RC::ConstHandle<ArrayProducerWrapper> m_inputArrayProducer;
      RC::ConstHandle<ArrayCache> m_unwrapped;
    };
  }
}

#endif //_FABRIC_MR_ARRAY_CACHE_WRAPPER_H
