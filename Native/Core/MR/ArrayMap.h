/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_ARRAY_MAP_H
#define _FABRIC_MR_ARRAY_MAP_H

#include <Fabric/Core/MR/ArrayProducer.h>

#include <stdint.h>

namespace Fabric
{
  namespace KLC
  {
    class ArrayMapOperator;
  };
  
  namespace MR
  {
    class ValueProducer;
    
    class ArrayMap : public ArrayProducer
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
    
    public:
    
      static RC::Handle<ArrayMap> Create(
        RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
        RC::ConstHandle<KLC::ArrayMapOperator> const &mapOperator,
        RC::ConstHandle<ValueProducer> const &sharedValueProducer
        );
      
      // Virtual functions: ArrayProducer
    
    public:
      
      virtual size_t getCount() const;
      virtual const RC::Handle<ArrayProducer::ComputeState> createComputeState() const;
            
    protected:
    
      class ComputeState : public ArrayProducer::ComputeState
      {
      public:
      
        static RC::Handle<ComputeState> Create( RC::ConstHandle<ArrayMap> const &arrayMap );
      
        virtual void produce( size_t index, void *data ) const;
      
      protected:
      
        ComputeState( RC::ConstHandle<ArrayMap> const &arrayMap );
        ~ComputeState();
        
      private:
      
        RC::ConstHandle<ArrayMap> m_arrayMap;
        RC::ConstHandle<ArrayProducer::ComputeState> m_inputArrayProducerComputeState;
        std::vector<uint8_t> m_sharedData;
      };
    
      ArrayMap(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
        RC::ConstHandle<KLC::ArrayMapOperator> const &mapOperator,
        RC::ConstHandle<ValueProducer> const &sharedValueProducer
        );
      ~ArrayMap();
    
      virtual char const *getKind() const;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const;
    
    private:
    
      RC::ConstHandle<ArrayProducer> m_inputArrayProducer;
      RC::ConstHandle<KLC::ArrayMapOperator> m_mapOperator;
      RC::ConstHandle<ValueProducer> m_sharedValueProducer;
    };
  };
};

#endif //_FABRIC_MR_ARRAY_MAP_H
