/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_VALUE_TRANSFORM_H
#define _FABRIC_MR_VALUE_TRANSFORM_H

#include <Fabric/Core/MR/ValueProducer.h>

#include <stdint.h>

namespace Fabric
{
  namespace KLC
  {
    class ValueTransformOperator;
  };
  
  namespace MR
  {
    class ValueTransform : public ValueProducer
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
    
    public:
    
      static RC::Handle<ValueTransform> Create(
        RC::ConstHandle<ValueProducer> const &inputValueProducer,
        RC::ConstHandle<KLC::ValueTransformOperator> const &valueTransformOperator,
        RC::ConstHandle<ValueProducer> const &sharedValueProducer
        );
      
      // Virtual functions: ValueProducer
    
    public:
      
      virtual const RC::Handle<ValueProducer::ComputeState> createComputeState() const;
            
    protected:
    
      class ComputeState : public ValueProducer::ComputeState
      {
      public:
      
        static RC::Handle<ComputeState> Create( RC::ConstHandle<ValueTransform> const &valueTransform );
      
        virtual void produce( void *data ) const;
      
      protected:
      
        ComputeState( RC::ConstHandle<ValueTransform> const &valueTransform );
        ~ComputeState();
        
      private:
      
        RC::ConstHandle<ValueTransform> m_valueTransform;
        RC::ConstHandle<ValueProducer::ComputeState> m_inputValueProducerComputeState;
        std::vector<uint8_t> m_sharedData;
      };
    
      ValueTransform(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<ValueProducer> const &inputValueProducer,
        RC::ConstHandle<KLC::ValueTransformOperator> const &valueTransformOperator,
        RC::ConstHandle<ValueProducer> const &sharedValueProducer
        );
      ~ValueTransform();
    
      virtual char const *getKind() const;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const;
    
    private:
    
      RC::ConstHandle<ValueProducer> m_inputValueProducer;
      RC::ConstHandle<KLC::ValueTransformOperator> m_valueTransformOperator;
      RC::ConstHandle<ValueProducer> m_sharedValueProducer;
    };
  };
};

#endif //_FABRIC_MR_VALUE_TRANSFORM_H
