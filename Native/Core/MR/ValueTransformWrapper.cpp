/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ValueTransformWrapper.h>
#include <Fabric/Core/MR/ValueTransform.h>
#include <Fabric/Core/KLC/ValueTransformOperatorWrapper.h>
#include <Fabric/Core/KLC/ValueTransformOperator.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ValueTransformWrapper, ValueProducerWrapper );
    
    RC::Handle<ValueTransformWrapper> ValueTransformWrapper::Create(
      RC::ConstHandle<ValueProducerWrapper> const &input,
      RC::ConstHandle<KLC::ValueTransformOperatorWrapper> const &operator_,
      RC::ConstHandle<ValueProducerWrapper> const &shared
      )
    {
      return new ValueTransformWrapper( FABRIC_GC_OBJECT_MY_CLASS, input, operator_, shared );
    }
    
    ValueTransformWrapper::ValueTransformWrapper(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<ValueProducerWrapper> const &input,
      RC::ConstHandle<KLC::ValueTransformOperatorWrapper> const &operator_,
      RC::ConstHandle<ValueProducerWrapper> const &shared
      )
      : ValueProducerWrapper( FABRIC_GC_OBJECT_CLASS_ARG )
      , m_input( input )
      , m_operator( operator_ )
      , m_shared( shared )
      , m_unwrapped(
        ValueTransform::Create(
          input->getUnwrapped(),
          operator_->getUnwrapped(),
          shared? shared->getUnwrapped(): RC::ConstHandle<ValueProducer>()
        )
      )
    {
    }
      
    RC::ConstHandle<ValueProducer> ValueTransformWrapper::getUnwrapped() const
    {
      return m_unwrapped;
    }

    char const *ValueTransformWrapper::getKind() const
    {
      return "ValueTransform";
    }
    
    void ValueTransformWrapper::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      {
        Util::JSONGenerator jg = jog.makeMember( "input" );
        m_input->toJSON( jg );
      }

      {
        Util::JSONGenerator jg = jog.makeMember( "operator" );
        m_operator->toJSON( jg );
      }
      
      if ( m_shared )
      {
        Util::JSONGenerator jg = jog.makeMember( "shared" );
        m_shared->toJSON( jg );
      }
    }
  }
}
