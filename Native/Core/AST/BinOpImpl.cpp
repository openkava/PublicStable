/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/BinOpImpl.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/OverloadNames.h>

namespace Fabric
{
  namespace AST
  {
    BinOpImpl::BinOpImpl(
        CG::Location const &location,
        CG::ExprType const &returnExprType,
        CG::BinOpType binOpType,
        RC::ConstHandle< AST::Param > lhs,
        RC::ConstHandle< AST::Param > rhs,
        RC::ConstHandle<CompoundStatement> const &body
        )
      : Function(
        location,
        CG::binOpOverloadName( binOpType, lhs->getAdapter(), rhs->getAdapter() ),
        returnExprType,
        ParamList::Create( location, lhs, ParamList::Create( location, rhs ) ),
        body
        )
    {
    }
  };
};
