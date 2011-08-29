/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/AssignOpImpl.h>
#include <Fabric/Core/AST/Param.h>
#include <Fabric/Core/AST/ParamVector.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/OpTypes.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( AssignOpImpl );
    
    RC::ConstHandle<AssignOpImpl> AssignOpImpl::Create(
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
      , m_assignOpType( assignOpType )
    {
    }
    
    void AssignOpImpl::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      MethodOpImpl::appendJSONMembers( jsonObjectGenerator );
      jsonObjectGenerator.makeMember( "op" ).makeString( CG::assignOpTypeDesc( m_assignOpType ) );
    }
  };
};
