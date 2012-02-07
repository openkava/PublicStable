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
      std::string const &thisType,
      CG::AssignOpType assignOpType,
      RC::ConstHandle<AST::Param> rhs,
      RC::ConstHandle<CompoundStatement> const &body
      )
    {
      return new AssignOpImpl( location, thisType, assignOpType, rhs, body );
    }
    
    AssignOpImpl::AssignOpImpl(
      CG::Location const &location,
      std::string const &thisType,
      CG::AssignOpType assignOpType,
      RC::ConstHandle<AST::Param> rhs,
      RC::ConstHandle<CompoundStatement> const &body
      )
      : FunctionBase(
        location,
        "",
        body
        )
      , m_assignOpType( assignOpType )
      , m_params(
        ParamVector::Create(
          Param::Create(
            location,
            "this",
            thisType,
            CG::USAGE_LVALUE
            ),
          rhs
          )
        )
    {
    }
    
    void AssignOpImpl::appendJSONMembers( JSON::ObjectEncoder const &jsonObjectEncoder, bool includeLocation ) const
    {
      FunctionBase::appendJSONMembers( jsonObjectEncoder, includeLocation );
      jsonObjectEncoder.makeMember( "op" ).makeString( CG::assignOpUserName( m_assignOpType ) );
      m_params->appendJSON( jsonObjectEncoder.makeMember( "params" ), includeLocation );
    }
    
    std::string AssignOpImpl::getEntryName( RC::Handle<CG::Manager> const &cgManager ) const
    {
      RC::ConstHandle<ParamVector> params = getParams( cgManager );
      return CG::assignOpOverloadName( m_assignOpType, params->get(0)->getAdapter( cgManager ), params->get(1)->getAdapter( cgManager ) );
    }

    RC::ConstHandle<ParamVector> AssignOpImpl::getParams( RC::Handle<CG::Manager> const &cgManager ) const
    {
      return m_params;
    }
  }
}
