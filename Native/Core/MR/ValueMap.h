/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_VALUE_MAP_H
#define _FABRIC_MR_VALUE_MAP_H

#include <Fabric/Core/MR/ValueProducer.h>

#include <stdint.h>

namespace Fabric
{
  namespace KLC
  {
    class ValueMapOperator;
  };
  
  namespace MR
  {
    class ValueMap : public ValueProducer
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
    
    public:
    
      static RC::Handle<ValueMap> Create(
        RC::ConstHandle<ValueProducer> const &inputValueProducer,
        RC::ConstHandle<KLC::ValueMapOperator> const &valueMapOperator,
        RC::ConstHandle<ValueProducer> const &sharedValueProducer
        );
      
      // Virtual functions: ValueProducer
    
    public:
      
      virtual const RC::Handle<ValueProducer::ComputeState> createComputeState() const;
            
    protected:
    
      class ComputeState : public ValueProducer::ComputeState
      {
      public:
      
        static RC::Handle<ComputeState> Create( RC::ConstHandle<ValueMap> const &valueMap );
      
        virtual void produce( void *data ) const;
      
      protected:
      
        ComputeState( RC::ConstHandle<ValueMap> const &valueMap );
        ~ComputeState();
        
      private:
      
        RC::ConstHandle<ValueProducer::ComputeState> m_inputComputeState;
        RC::ConstHandle<RT::Desc> m_inputDesc;
        RC::ConstHandle<KLC::ValueMapOperator> m_operator;
        RC::ConstHandle<ValueProducer> m_shared;
        std::vector<uint8_t> m_sharedData;
      };
      
      ValueMap(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<ValueProducer> const &inputValueProducer,
        RC::ConstHandle<KLC::ValueMapOperator> const &valueMapOperator,
        RC::ConstHandle<ValueProducer> const &sharedValueProducer
        );
      ~ValueMap();
    
      virtual char const *getKind() const;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const;
    
    private:
    
      RC::ConstHandle<ValueProducer> m_inputValueProducer;
      RC::ConstHandle<KLC::ValueMapOperator> m_valueMapOperator;
      RC::ConstHandle<ValueProducer> m_sharedValueProducer;
    };
  };
};

#endif //_FABRIC_MR_VALUE_MAP_H
