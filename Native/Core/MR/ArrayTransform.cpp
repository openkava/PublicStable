/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ArrayTransform.h>
#include <Fabric/Core/KLC/ArrayTransformOperator.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ArrayTransform, ArrayProducer );
    
    RC::Handle<ArrayTransform> ArrayTransform::Create(
      RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
      RC::ConstHandle<KLC::ArrayTransformOperator> const &operator_,
      RC::ConstHandle<ValueProducer> const &sharedValueProducer
      )
    {
      return new ArrayTransform( FABRIC_GC_OBJECT_MY_CLASS, inputArrayProducer, operator_, sharedValueProducer );
    }
    
    ArrayTransform::ArrayTransform(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
      RC::ConstHandle<KLC::ArrayTransformOperator> const &operator_,
      RC::ConstHandle<ValueProducer> const &sharedValueProducer
      )
      : ArrayProducer( FABRIC_GC_OBJECT_CLASS_ARG, operator_->getValueDesc() )
      , m_inputArrayProducer( inputArrayProducer )
      , m_operator( operator_ )
      , m_sharedValueProducer( sharedValueProducer )
    {
      RC::ConstHandle<RT::Desc> inputElementDesc = inputArrayProducer->getElementDesc();
      if ( !inputElementDesc )
        throw Exception("input is invalid");
      RC::ConstHandle<RT::Desc> operatorValueDesc = m_operator->getValueDesc();
      if ( !operatorValueDesc )
        throw Exception("operator is invalid");
      if ( !operatorValueDesc->isEquivalentTo( inputElementDesc ) )
        throw Exception(
          "input type ("
          + _(inputElementDesc->getUserName())
          + ") is not equivalent to operator value type ("
          + _(operatorValueDesc->getUserName()) + ")"
          );
      RC::ConstHandle<RT::Desc> operatorSharedDesc = m_operator->getSharedDesc();
      if ( operatorSharedDesc )
      {
        RC::ConstHandle<RT::Desc> sharedValueDesc = m_sharedValueProducer->getValueDesc();
        if ( !sharedValueDesc )
          throw Exception( "operator requires a shared value but no shared value producer is provided" );
        if ( !sharedValueDesc->isEquivalentTo( operatorSharedDesc ) )
          throw Exception(
            "shared value type ("
            + _(operatorSharedDesc->getUserName())
            + ") is not equivalent to operator shared type ("
            + _(sharedValueDesc->getUserName()) + ")"
            );
      }
    }
    
    ArrayTransform::~ArrayTransform()
    {
    }

    char const *ArrayTransform::getKind() const
    {
      return "ArrayTransform";
    }
    
    void ArrayTransform::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      {
        Util::JSONGenerator jg = jog.makeMember( "inputArrayProducer" );
        m_inputArrayProducer->toJSON( jg );
      }

      {
        Util::JSONGenerator jg = jog.makeMember( "operator" );
        m_operator->toJSON( jg );
      }
      
      if ( m_sharedValueProducer )
      {
        Util::JSONGenerator jg = jog.makeMember( "sharedValueProducer" );
        m_sharedValueProducer->toJSON( jg );
      }
    }

    size_t ArrayTransform::count() const
    {
      return m_inputArrayProducer->count();
    }
    
    void ArrayTransform::produce( size_t index, void *data ) const
    {
      m_inputArrayProducer->produce( index, data );
      
      if ( m_operator->takesIndex() )
      {
        if ( m_operator->takesCount() )
        {
          size_t count = m_inputArrayProducer->count();
              
          if ( m_operator->takesSharedValue() )
          {
            RC::ConstHandle<RT::Desc> sharedDesc = m_sharedValueProducer->getValueDesc();
            size_t sharedDataSize = sharedDesc->getAllocSize();
            void *sharedData = alloca( sharedDataSize );
            memset( sharedData, 0, sharedDataSize );
            
            m_sharedValueProducer->produce( sharedData );

            m_operator->call( data, index, count, sharedData );
            
            sharedDesc->disposeData( sharedData );
          }
          else m_operator->call( data, index, count );
        }
        else m_operator->call( data, index );
      }
      else m_operator->call( data );
    }
  };
};
