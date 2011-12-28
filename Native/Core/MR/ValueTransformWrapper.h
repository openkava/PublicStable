/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_VALUE_TRANSFORM_WRAPPER_H
#define _FABRIC_MR_VALUE_TRANSFORM_WRAPPER_H

#include <Fabric/Core/MR/ValueProducerWrapper.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace KLC
  {
    class ValueTransformOperatorWrapper;
  }
  
  namespace MR
  {
    class ValueTransform;
    
    class ValueTransformWrapper : public ValueProducerWrapper
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
    
    public:
    
      static RC::Handle<ValueTransformWrapper> Create(
        RC::ConstHandle<ValueProducerWrapper> const &input,
        RC::ConstHandle<KLC::ValueTransformOperatorWrapper> const &operator_,
        RC::ConstHandle<ValueProducerWrapper> const &shared
        );
      
      virtual RC::ConstHandle<ValueProducer> getUnwrapped() const;
      
    protected:
    
      ValueTransformWrapper(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<ValueProducerWrapper> const &input,
        RC::ConstHandle<KLC::ValueTransformOperatorWrapper> const &operator_,
        RC::ConstHandle<ValueProducerWrapper> const &shared
        );
    
      virtual char const *getKind() const;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const;
    
    private:
    
      RC::ConstHandle<ValueProducerWrapper> m_input;
      RC::ConstHandle<KLC::ValueTransformOperatorWrapper> m_operator;
      RC::ConstHandle<ValueProducerWrapper> m_shared;
      
      RC::ConstHandle<ValueProducer> m_unwrapped;
    };
  }
}

#endif //_FABRIC_MR_VALUE_TRANSFORM_WRAPPER_H
