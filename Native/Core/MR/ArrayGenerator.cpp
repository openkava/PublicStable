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
    
    void ArrayGenerator::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      {
        Util::JSONGenerator jg = jog.makeMember( "arrayGeneratorOperator" );
        m_arrayGeneratorOperator->toJSON( jg );
      }
    }

    size_t ArrayGenerator::count() const
    {
      size_t result;
      m_countValueProducer->produce( &result );
      return result;
    }
    
    void ArrayGenerator::produce( size_t index, void *data ) const
    {
      m_arrayGeneratorOperator->call( index, data );
    }
  };
};
