/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_REDUCE_H
#define _FABRIC_MR_REDUCE_H

#include <Fabric/Core/MR/ValueProducer.h>

namespace Fabric
{
  namespace KLC
  {
    class ReduceOperator;
  };
  
  namespace MR
  {
    class ArrayProducer;
    
    class Reduce : public ValueProducer
    {
      FABRIC_GC_OBJECT_CLASS_DECL()

    public:
    
      static RC::Handle<Reduce> Create(
        RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
        RC::ConstHandle<KLC::ReduceOperator> const &reduceOperator
        );
      
      // Virtual functions: ValueProducer
    
    public:
    
      virtual void produce( void *data ) const;
      
    protected:
    
      Reduce(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
         RC::ConstHandle<KLC::ReduceOperator> const &reduceOperator
        );
      ~Reduce();
    
      virtual char const *getKind() const;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const;
    
    private:
    
      RC::ConstHandle<ArrayProducer> m_inputArrayProducer;
      RC::ConstHandle<KLC::ReduceOperator> m_reduceOperator;
    };
  };
};

#endif //_FABRIC_MR_REDUCE_H
