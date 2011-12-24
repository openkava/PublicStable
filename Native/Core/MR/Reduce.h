/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_REDUCE_H
#define _FABRIC_MR_REDUCE_H

#include <Fabric/Core/MR/ValueProducer.h>
#include <Fabric/Core/MR/ArrayProducer.h>
#include <Fabric/Core/Util/Mutex.h>

namespace Fabric
{
  namespace KLC
  {
    class ReduceOperator;
  };
  
  namespace MR
  {
    class Reduce : public ValueProducer
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
      class Execution;

    public:
    
      static RC::Handle<Reduce> Create(
        RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
        RC::ConstHandle<KLC::ReduceOperator> const &reduceOperator,
        RC::ConstHandle<ValueProducer> const &sharedValueProducer
        );
      
      // Virtual functions: ValueProducer
    
    public:
      
      virtual const RC::Handle<ValueProducer::ComputeState> createComputeState() const;
            
    protected:
    
      class ComputeState : public ValueProducer::ComputeState
      {
      public:
      
        static RC::Handle<ComputeState> Create( RC::ConstHandle<Reduce> const &reduce );
      
        virtual void produce( void *data ) const;
      
      protected:
      
        ComputeState( RC::ConstHandle<Reduce> const &reduce );
        ~ComputeState();
        
      private:
      
        RC::ConstHandle<ArrayProducer::ComputeState> m_inputComputeState;
        RC::ConstHandle<RT::Desc> m_inputDesc;
        RC::ConstHandle<KLC::ReduceOperator> m_operator;
        RC::ConstHandle<ValueProducer> m_shared;
        std::vector<uint8_t> m_sharedData;
        Util::Mutex &m_mutex;
      };
      
      Reduce(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
        RC::ConstHandle<KLC::ReduceOperator> const &reduceOperator,
        RC::ConstHandle<ValueProducer> const &sharedValueProducer
        );
      ~Reduce();
    
      virtual char const *getKind() const;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const;
    
    private:
    
      RC::ConstHandle<ArrayProducer> m_inputArrayProducer;
      RC::ConstHandle<KLC::ReduceOperator> m_reduceOperator;
      RC::ConstHandle<ValueProducer> m_sharedValueProducer;
      
      mutable Util::Mutex m_mutex;
    };
  };
};

#endif //_FABRIC_MR_REDUCE_H
