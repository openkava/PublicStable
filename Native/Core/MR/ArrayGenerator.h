/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_ARRAY_GENERATOR_H
#define _FABRIC_MR_ARRAY_GENERATOR_H

#include <Fabric/Core/MR/ArrayProducer.h>

namespace Fabric
{
  namespace RT
  {
    class Manager;
  };
  
  namespace KLC
  {
    class ArrayGeneratorOperator;
  };
  
  namespace MR
  {
    class ValueProducer;
    
    class ArrayGenerator : public ArrayProducer
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
    
    public:
    
      static RC::Handle<ArrayGenerator> Create(
        RC::ConstHandle<RT::Manager> const &rtManager,
        RC::ConstHandle<ValueProducer> const &countValueProducer,
        RC::ConstHandle<KLC::ArrayGeneratorOperator> const &mapOperator
        );
      
      // Virtual functions: ArrayProducer
    
    public:
      
      virtual size_t getCount() const;
      virtual const RC::Handle<ArrayProducer::ComputeState> createComputeState() const;
            
    protected:
    
      class ComputeState : public ArrayProducer::ComputeState
      {
      public:
      
        static RC::Handle<ComputeState> Create( RC::ConstHandle<ArrayGenerator> const &arrayGenerator );
      
        virtual void produce( size_t index, void *data ) const;
      
      protected:
      
        ComputeState( RC::ConstHandle<ArrayGenerator> const &arrayGenerator );
        
      private:
      
        RC::ConstHandle<ArrayGenerator> m_arrayGenerator;
      };
    
      ArrayGenerator(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<RT::Manager> const &rtManager,
        RC::ConstHandle<ValueProducer> const &countValueProducer,
        RC::ConstHandle<KLC::ArrayGeneratorOperator> const &mapOperator
        );
      ~ArrayGenerator();
    
      virtual char const *getKind() const;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const;
    
    private:
    
      RC::ConstHandle<ValueProducer> m_countValueProducer;
      RC::ConstHandle<KLC::ArrayGeneratorOperator> m_arrayGeneratorOperator;
    };
  };
};

#endif //_FABRIC_MR_ARRAY_GENERATOR_H
