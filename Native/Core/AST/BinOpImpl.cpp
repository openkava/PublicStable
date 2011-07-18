/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/BinOpImpl.h>
#include <Fabric/Core/AST/Param.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( BinOpImpl );
    
    BinOpImpl::BinOpImpl(
        CG::Location const &location,
        CG::ExprType const &returnExprType,
        CG::BinOpType binOpType,
        RC::ConstHandle<AST::Param> lhs,
        RC::ConstHandle<AST::Param> rhs,
        RC::ConstHandle<CompoundStatement> const &body
        )
      : Function(
        location,
        "",
        CG::binOpOverloadName( binOpType, lhs->getAdapter(), rhs->getAdapter() ),
        returnExprType,
        ParamVector::Create( lhs, rhs ),
        body
        )
    {
    }
  };
};
