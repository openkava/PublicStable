/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/AssignOpImpl.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/OverloadNames.h>

namespace Fabric
{
  namespace AST
  {
    AssignOpImpl::AssignOpImpl(
      CG::Location const &location,
      RC::ConstHandle<CG::Adapter> const &selfType,
      CG::AssignOpType assignOpType,
      RC::ConstHandle< AST::Param > rhs,
      RC::ConstHandle<CompoundStatement> const &body
      )
      : MethodOpImpl(
        location,
        RC::ConstHandle<CG::Adapter>(),
        selfType,
        CG::assignOpMethodName( assignOpType ),
        ParamList::Create( location, rhs ),
        body
        )
    {
    }
  };
};
