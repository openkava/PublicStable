/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_ARRAY_TRANSFORM_H
#define _FABRIC_MR_ARRAY_TRANSFORM_H

#include <Fabric/Core/MR/ArrayProducer.h>
#include <Fabric/Core/MR/ValueProducer.h>

namespace Fabric
{
  namespace KLC
  {
    class ArrayTransformOperator;
  };
  
  namespace MR
  {
    class ArrayTransform : public ArrayProducer
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
    
    public:
    
      static RC::Handle<ArrayTransform> Create(
        RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
        RC::ConstHandle<KLC::ArrayTransformOperator> const &operator_,
        RC::ConstHandle<ValueProducer> const &sharedValueProducer
        );
      
      // Virtual functions: ArrayProducer
    
    public:
    
      virtual size_t count() const;
      virtual void produce( size_t index, void *data ) const;
            
    protected:
    
      ArrayTransform(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
        RC::ConstHandle<KLC::ArrayTransformOperator> const &operator_,
        RC::ConstHandle<ValueProducer> const &sharedValueProducer
        );
      ~ArrayTransform();
    
      virtual char const *getKind() const;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const;
    
    private:
    
      RC::ConstHandle<ArrayProducer> m_inputArrayProducer;
      RC::ConstHandle<KLC::ArrayTransformOperator> m_operator;
      RC::ConstHandle<ValueProducer> m_sharedValueProducer;
    };
  };
};

#endif //_FABRIC_MR_ARRAY_TRANSFORM_H
