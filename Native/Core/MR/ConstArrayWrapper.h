/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_CONST_ARRAY_WRAPPER_H
#define _FABRIC_MR_CONST_ARRAY_WRAPPER_H

#include <Fabric/Core/MR/ArrayProducerWrapper.h>

namespace Fabric
{
  namespace Util
  {
    class JSONEntityInfo;
  }
  
  namespace RT
  {
    class Desc;
    class Manager;
  }
  
  namespace JSON
  {
    class Array;
  }
  
  namespace MR
  {
    class ArrayProducer;
    class ConstArray;
    
    class ConstArrayWrapper : public ArrayProducerWrapper
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
    public:
    
      static RC::Handle<ConstArrayWrapper> Create(
        RC::ConstHandle<RT::Manager> const &rtManager,
        RC::ConstHandle<RT::Desc> const &elementDesc,
        RC::ConstHandle<JSON::Array> const &jsonArray
        );
      
      static RC::Handle<ConstArrayWrapper> Create(
        RC::ConstHandle<RT::Manager> const &rtManager,
        RC::ConstHandle<RT::Desc> const &elementDesc,
        Util::JSONEntityInfo const &entityInfo
        );
      
      virtual RC::ConstHandle<ArrayProducer> getUnwrapped() const;
      
      // Virtual functions: ArrayProducer
    
    protected:
        
      ConstArrayWrapper(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<RT::Manager> const &rtManager,
        RC::ConstHandle<RT::Desc> const &elementDesc,
        RC::ConstHandle<JSON::Array> const &jsonArray
        );
    
      ConstArrayWrapper(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<RT::Manager> const &rtManager,
        RC::ConstHandle<RT::Desc> const &elementDesc,
        Util::JSONEntityInfo const &entityInfo
        );
    
      virtual char const *getKind() const;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const;
    
    private:
    
      RC::ConstHandle<ConstArray> m_unwrapped;
    };
  }
}

#endif //_FABRIC_MR_CONST_ARRAY_WRAPPER_H
