/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/AssignOpImpl.h>
#include <Fabric/Core/AST/Param.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( AssignOpImpl );
    
    RC::Handle<AssignOpImpl> AssignOpImpl::Create(
      CG::Location const &location,
      std::string const &selfType,
      CG::AssignOpType assignOpType,
      RC::ConstHandle<AST::Param> rhs,
      RC::ConstHandle<CompoundStatement> const &body
      )
    {
      return new AssignOpImpl( location, selfType, assignOpType, rhs, body );
    }
    
    AssignOpImpl::AssignOpImpl(
      CG::Location const &location,
      std::string const &selfType,
      CG::AssignOpType assignOpType,
      RC::ConstHandle<AST::Param> rhs,
      RC::ConstHandle<CompoundStatement> const &body
      )
      : MethodOpImpl(
        location,
        "",
        selfType,
        CG::assignOpMethodName( assignOpType ),
        ParamVector::Create( rhs ),
        body
        )
    {
    }
  };
};
