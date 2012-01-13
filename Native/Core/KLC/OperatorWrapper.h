/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KLC_OPERATOR_WRAPPER_H
#define _FABRIC_KLC_OPERATOR_WRAPPER_H

#include <Fabric/Core/KLC/FunctionWrapper.h>

namespace Fabric
{
  namespace AST
  {
    class Operator;
  }
  
  namespace KLC
  {
    class OperatorWrapper : public FunctionWrapper
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
    protected:
    
      OperatorWrapper(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<Executable> const &executable,
        RC::ConstHandle<AST::Operator> const &astOperator,
        GenericFunctionPtr functionPtr
        );
    };
  }
}

#endif //_FABRIC_KLC_OPERATOR_WRAPPER_H
