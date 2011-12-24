/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KLC/ArrayMapOperator.h>
#include <Fabric/Core/KLC/Executable.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/AST/Operator.h>

namespace Fabric
{
  namespace KLC
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ArrayMapOperator, ArrayIOOperator )
      
    RC::Handle<ArrayMapOperator> ArrayMapOperator::Create(
      RC::ConstHandle<Executable> const &executable,
      RC::ConstHandle<AST::Operator> const &astOperator,
      GenericFunctionPtr functionPtr
      )
    {
      return new ArrayMapOperator(
        FABRIC_GC_OBJECT_MY_CLASS,
        executable,
        astOperator,
        functionPtr
        );
    }

    ArrayMapOperator::ArrayMapOperator(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<Executable> const &executable,
      RC::ConstHandle<AST::Operator> const &astOperator,
      GenericFunctionPtr functionPtr
      )
      : ArrayIOOperator(
        FABRIC_GC_OBJECT_CLASS_ARG,
        executable,
        astOperator,
        functionPtr
        )
    {
    }
    
    char const *ArrayMapOperator::getKind() const
    {
      return "ArrayMapOperator";
    }
  }
}
