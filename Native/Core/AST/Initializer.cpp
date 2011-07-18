/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/Initializer.h>
#include <Fabric/Core/AST/Param.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Initializer );
    
    RC::Handle<Function> Initializer::Create(
      CG::Location const &location,
      RC::ConstHandle<CG::Adapter> const &selfAdapter,
      RC::ConstHandle<ParamVector> const &params,
      RC::ConstHandle<CompoundStatement> const &body
      )
    {
      return new Initializer( location, selfAdapter, params, body );
    }
    
    Initializer::Initializer(
        CG::Location const &location,
        RC::ConstHandle<CG::Adapter> const &selfAdapter,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        )
      : Function(
        location,
        "",
        CG::constructOverloadName( selfAdapter, params->getTypes() ),
        CG::ExprType(),
        ParamVector::Create(
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
