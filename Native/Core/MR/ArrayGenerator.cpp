/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ArrayGenerator.h>
#include <Fabric/Core/MR/ValueProducer.h>
#include <Fabric/Core/KLC/ArrayGeneratorOperator.h>
#include <Fabric/Core/RT/IntegerDesc.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ArrayGenerator, ArrayProducer );
    
    RC::Handle<ArrayGenerator> ArrayGenerator::Create(
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<ValueProducer> const &countValueProducer,
      RC::ConstHandle<KLC::ArrayGeneratorOperator> const &arrayGeneratorOperator
      )
    {
      return new ArrayGenerator( FABRIC_GC_OBJECT_MY_CLASS, rtManager, countValueProducer, arrayGeneratorOperator );
    }
    
    ArrayGenerator::ArrayGenerator(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<RT::Manager> const &rtManager,
      RC::ConstHandle<ValueProducer> const &countValueProducer,
      RC::ConstHandle<KLC::ArrayGeneratorOperator> const &arrayGeneratorOperator
      )
      : ArrayProducer( FABRIC_GC_OBJECT_CLASS_ARG, arrayGeneratorOperator->getOutputDesc() )
      , m_countValueProducer( countValueProducer )
      , m_arrayGeneratorOperator( arrayGeneratorOperator )
    {
      if ( !m_countValueProducer->getValueDesc()->isEquivalentTo( rtManager->getSizeDesc() ) )
        throw Exception("count value producer must produce a value of type Size");
    }
    
    ArrayGenerator::~ArrayGenerator()
    {
    }

    char const *ArrayGenerator::getKind() const
    {
      return "ArrayGenerator";
    }

    size_t ArrayGenerator::getCount() const
    {
      size_t result;
      m_countValueProducer->createComputeState()->produce( &result );
      return result;
    }
      
    const RC::Handle<ArrayProducer::ComputeState> ArrayGenerator::createComputeState() const
    {
      return ComputeState::Create( this );
    }
    
    void ArrayGenerator::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      {
        Util::JSONGenerator jg = jog.makeMember( "countValueProducer" );
        m_countValueProducer->toJSON( jg );
      }
      
      {
        Util::JSONGenerator jg = jog.makeMember( "arrayGeneratorOperator" );
        m_arrayGeneratorOperator->toJSON( jg );
      }
    }
    
    RC::Handle<ArrayGenerator::ComputeState> ArrayGenerator::ComputeState::Create( RC::ConstHandle<ArrayGenerator> const &arrayGenerator )
    {
      return new ComputeState( arrayGenerator );
    }
    
    ArrayGenerator::ComputeState::ComputeState( RC::ConstHandle<ArrayGenerator> const &arrayGenerator )
      : ArrayProducer::ComputeState( arrayGenerator )
      , m_arrayGenerator( arrayGenerator )
    {
    }
    
    void ArrayGenerator::ComputeState::produce( size_t index, void *data ) const
    {
      m_arrayGenerator->m_arrayGeneratorOperator->call( index, data );
    }
  };
};
