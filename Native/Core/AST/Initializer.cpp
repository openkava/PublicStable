/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "Initializer.h"
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/OverloadNames.h>

namespace Fabric
{
  namespace AST
  {
    Initializer::Initializer(
        CG::Location const &location,
        RC::ConstHandle<CG::Adapter> const &selfAdapter,
        RC::ConstHandle<ParamList> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        )
      : Function(
        location,
        "",
        CG::constructOverloadName( selfAdapter, params->getTypes() ),
        CG::ExprType(),
        ParamList::Create(
          location,
          Param::Create(
            location,
            "self",
            selfAdapter,
            CG::USAGE_LVALUE
            ),
          params
          ),
        body
        )
    {
    }
  };
};
