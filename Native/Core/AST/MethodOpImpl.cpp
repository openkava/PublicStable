/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/MethodOpImpl.h>
#include <Fabric/Core/AST/Param.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( MethodOpImpl );
    
    RC::Handle<Function> MethodOpImpl::Create(
      CG::Location const &location,
      CG::ExprType const &returnExprType,
      RC::ConstHandle<CG::Adapter> const &selfAdapter,
      std::string const &methodName,
      RC::ConstHandle<ParamVector> const &params,
      RC::ConstHandle<CompoundStatement> const &body
      )
    {
      return new MethodOpImpl( location, returnExprType, selfAdapter, methodName, params, body );
    }
    
    MethodOpImpl::MethodOpImpl(
      CG::Location const &location,
      CG::ExprType const &returnExprType,
      RC::ConstHandle<CG::Adapter> const &selfAdapter,
      std::string const &methodName,
      RC::ConstHandle<ParamVector> const &params,
      RC::ConstHandle<CompoundStatement> const &body
      )
      : Function(
        location,
        "",
        CG::methodOverloadName( methodName, selfAdapter, params->getTypes() ),
        returnExprType,
        ParamVector::Create(
          Param::Create(
            location,
            "self",
            selfAdapter,
            returnExprType ? CG::USAGE_RVALUE : CG::USAGE_LVALUE
            ),
          params
          ),
        body
        )
    {
    }
  };
};
