/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_VALUE_MAP_OPERATOR_H
#define _FABRIC_KL_MAP_OPERATOR_H

#include <Fabric/Core/KLC/ValueIOOperator.h>

namespace Fabric
{
  namespace KLC
  {
    class ValueMapOperator : public ValueIOOperator
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
    public:
    
      static RC::Handle<ValueMapOperator> Create(
        RC::ConstHandle<Executable> const &executable,
        RC::ConstHandle<AST::Operator> const &astOperator,
        GenericFunctionPtr functionPtr
        );
      
      virtual char const *getKind() const;
      
    protected:
    
      ValueMapOperator(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<Executable> const &executable,
        RC::ConstHandle<AST::Operator> const &astOperator,
        GenericFunctionPtr functionPtr
        );
    };
  }
}

#endif //_FABRIC_KL_VALUE_MAP_OPERATOR_H
