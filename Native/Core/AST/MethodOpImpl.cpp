/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "MethodOpImpl.h"
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/OverloadNames.h>

namespace Fabric
{
  namespace AST
  {
    MethodOpImpl::MethodOpImpl(
      CG::Location const &location,
      CG::ExprType const &returnExprType,
      RC::ConstHandle<CG::Adapter> const &selfAdapter,
      std::string const &methodName,
      RC::ConstHandle<ParamList> const &params,
      RC::ConstHandle<CompoundStatement> const &body
      )
      : Function(
        location,
        "",
        CG::methodOverloadName( methodName, selfAdapter, params->getTypes() ),
        returnExprType,
        ParamList::Create(
          location,
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
