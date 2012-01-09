/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KLC_VALUE_MAP_OPERATOR_H
#define _FABRIC_KLC_VALUE_MAP_OPERATOR_H

#include <Fabric/Core/KLC/ValueIOOperator.h>

namespace Fabric
{
  namespace AST
  {
    class Operator;
  }
  
  namespace KLC
  {
    class Executable;

    class ValueMapOperator : public ValueIOOperator
    {
    public:
    
      static RC::Handle<ValueMapOperator> Create(
        RC::ConstHandle<Executable> const &executable,
        RC::ConstHandle<AST::Operator> const &astOperator,
        GenericFunctionPtr functionPtr
        );
      
    protected:
    
      ValueMapOperator(
        RC::ConstHandle<Executable> const &executable,
        RC::ConstHandle<AST::Operator> const &astOperator,
        GenericFunctionPtr functionPtr
        );
    };
  }
}

#endif //_FABRIC_KLC_VALUE_MAP_OPERATOR_H
