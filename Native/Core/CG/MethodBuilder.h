/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_CG_METHOD_BUILDER_H
#define _FABRIC_CG_METHOD_BUILDER_H

#include <Fabric/Core/CG/FunctionBuilder.h>
#include <Fabric/Core/CG/OverloadNames.h>

namespace Fabric
{
  namespace CG
  {
    class MethodBuilder : public FunctionBuilder
    {
      static inline FunctionParam FriendlyFunctionParam(
        std::string const &paramName,
        RC::ConstHandle<Adapter> const &paramAdapter,
        Usage paramUsage
        )
      {
        std::string friendlyParamName;
        switch ( paramUsage )
        {
          case USAGE_LVALUE:
            friendlyParamName = paramName + "LValue";
            break;
          case USAGE_RVALUE:
            friendlyParamName = paramName + "RValue";
            break;
          default:
            FABRIC_ASSERT( false );
            friendlyParamName = paramName;
            break;
        }
        return FunctionParam( friendlyParamName, paramAdapter, paramUsage );
      }
        
    public:
    
      MethodBuilder( 
        ModuleBuilder &moduleBuilder,
        RC::ConstHandle<Adapter> const &returnAdapter,
        RC::ConstHandle<Adapter> const &thisAdapter,
        Usage thisUsage,
        std::string const &methodName
        )
        : FunctionBuilder(
          moduleBuilder,
          MethodPencilName(
            thisAdapter,
            methodName
            ),
          MethodDefaultSymbolName(
            ExprType( thisAdapter, thisUsage ),
            methodName
            ),
          returnAdapter,
          ParamVector(
            FriendlyFunctionParam( "this", thisAdapter, thisUsage )
            ),
          0
          )
      {
      }
    
      MethodBuilder( 
        ModuleBuilder &moduleBuilder,
        RC::ConstHandle<Adapter> const &returnAdapter,
        RC::ConstHandle<Adapter> const &thisAdapter,
        Usage thisUsage,
        std::string const &methodName,
        std::string const &paramName,
        RC::ConstHandle<Adapter> const &paramAdapter,
        Usage paramUsage
        )
        : FunctionBuilder(
          moduleBuilder,
          MethodPencilName(
            thisAdapter,
            methodName
            ),
          MethodDefaultSymbolName(
            ExprType( thisAdapter, thisUsage ),
            methodName
            ),
          returnAdapter,
          ParamVector(
            FriendlyFunctionParam( "this", thisAdapter, thisUsage ),
            FriendlyFunctionParam( paramName, paramAdapter, paramUsage )
            ),
          0
          )
      {
      }
    
      MethodBuilder( 
        ModuleBuilder &moduleBuilder,
        RC::ConstHandle<Adapter> const &returnAdapter,
        RC::ConstHandle<Adapter> const &thisAdapter,
        Usage thisUsage,
        std::string const &methodName,
        std::string const &param1Name,
        RC::ConstHandle<Adapter> const &param1Adapter,
        Usage param1Usage,
        std::string const &param2Name,
        RC::ConstHandle<Adapter> const &param2Adapter,
        Usage param2Usage
        )
        : FunctionBuilder(
          moduleBuilder,
          MethodPencilName(
            thisAdapter,
            methodName
            ),
          MethodDefaultSymbolName(
            ExprType( thisAdapter, thisUsage ),
            methodName
            ),
          returnAdapter,
          ParamVector(
            FriendlyFunctionParam( "this", thisAdapter, thisUsage ),
            FriendlyFunctionParam( param1Name, param1Adapter, param1Usage ),
            FriendlyFunctionParam( param2Name, param2Adapter, param2Usage )
            ),
          0
          )
      {
      }
    };
  }
}

#endif //_FABRIC_CG_METHOD_BUILDER_H
