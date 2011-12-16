/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KLC/MapOperator.h>
#include <Fabric/Core/KLC/Executable.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/SizeAdapter.h>
#include <Fabric/Core/AST/Operator.h>
#include <Fabric/Core/AST/Param.h>
#include <Fabric/Core/AST/ParamVector.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace KLC
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( MapOperator, Operator )
      
    RC::Handle<MapOperator> MapOperator::Create(
      RC::ConstHandle<Executable> const &executable,
      RC::ConstHandle<AST::Operator> const &astOperator,
      GenericFunctionPtr functionPtr
      )
    {
      return new MapOperator(
        FABRIC_GC_OBJECT_MY_CLASS,
        executable,
        astOperator,
        functionPtr
        );
    }

    MapOperator::MapOperator(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<Executable> const &executable,
      RC::ConstHandle<AST::Operator> const &astOperator,
      GenericFunctionPtr functionPtr
      )
      : Operator(
        FABRIC_GC_OBJECT_CLASS_ARG,
        executable,
        astOperator,
        functionPtr
        )
    {
      static const Exception exception("operator must take 3 parameters: in Size index, in InputType inputData, io OutputType outputData");
      
      if ( astOperator )
      {
        RC::Handle<CG::Manager> cgManager = executable->getCGManager();
        
        RC::ConstHandle<AST::ParamVector> params = astOperator->getParams( cgManager );
        if ( params->size() != 3 )
          throw exception;
        
        RC::ConstHandle<AST::Param> sizeParam = params->get(0);
        if ( sizeParam->getUsage() != CG::USAGE_RVALUE
          || sizeParam->getAdapter( cgManager ) != cgManager->getSizeAdapter()
          )
          throw exception;
        
        RC::ConstHandle<AST::Param> inputParam = params->get(1);
        if ( inputParam->getUsage() != CG::USAGE_RVALUE )
          throw exception;
        m_inputDesc = inputParam->getAdapter( cgManager )->getDesc();
        
        RC::ConstHandle<AST::Param> outputParam = params->get(2);
        if ( outputParam->getUsage() != CG::USAGE_LVALUE )
          throw exception;
        m_outputDesc = outputParam->getAdapter( cgManager )->getDesc();
      }
    }
    
    char const *MapOperator::getKind() const
    {
      return "MapOperator";
    }
        
    RC::ConstHandle<RT::Desc> MapOperator::getInputDesc() const
    {
      return m_inputDesc;
    }
    
    RC::ConstHandle<RT::Desc> MapOperator::getOutputDesc() const
    {
      return m_outputDesc;
    }
  }
}
