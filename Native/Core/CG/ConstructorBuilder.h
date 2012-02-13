/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_CG_CONSTRUCTOR_BUILDER_H
#define _FABRIC_CG_CONSTRUCTOR_BUILDER_H

#include <Fabric/Core/CG/FunctionBuilder.h>
#include <Fabric/Core/CG/OverloadNames.h>

namespace Fabric
{
  namespace CG
  {
    class ConstructorBuilder : public FunctionBuilder
    {
    public:
    
      ConstructorBuilder( 
        ModuleBuilder &moduleBuilder,
        RC::ConstHandle<Adapter> const &thisAdapter,
        RC::ConstHandle<Adapter> const &thatAdapter
        )
        : FunctionBuilder(
          moduleBuilder,
          ConstructorPencilName( thisAdapter ),
          ConstructorDefaultSymbolName(
            thisAdapter,
            thatAdapter
            ),
          0,
          ParamVector(
            FunctionParam( "thisLValue", thisAdapter, USAGE_LVALUE ),
            FunctionParam( "thatRValue", thatAdapter, USAGE_RVALUE )
            ),
          0
          )
      {
      }
    };
  }
}

#endif //_FABRIC_CG_CONSTRUCTOR_BUILDER_H
