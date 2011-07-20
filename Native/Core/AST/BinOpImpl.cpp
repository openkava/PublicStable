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
    
    RC::Handle<BinOpImpl> BinOpImpl::Create(
      CG::Location const &location,
      std::string const &returnType,
      CG::BinOpType binOpType,
      RC::ConstHandle<AST::Param> const &lhs,
      RC::ConstHandle<AST::Param> const &rhs,
      RC::ConstHandle<CompoundStatement> const &body
      )
    {
      return new BinOpImpl( location, returnType, binOpType, lhs, rhs, body );
    }
        
    BinOpImpl::BinOpImpl(
      CG::Location const &location,
      std::string const &returnType,
      CG::BinOpType binOpType,
      RC::ConstHandle<AST::Param> const &lhs,
      RC::ConstHandle<AST::Param> const &rhs,
      RC::ConstHandle<CompoundStatement> const &body
      )
      : FunctionBase(
        location,
        returnType,
        ParamVector::Create( lhs, rhs ),
        body
        )
      , m_binOpType( binOpType )
    {
    }
    
    RC::Handle<JSON::Object> BinOpImpl::toJSON() const
    {
      RC::Handle<JSON::Object> result = FunctionBase::toJSON();
      result->set( "op", JSON::String::Create( CG::binOpUserName( m_binOpType ) ) );
      return result;
    }
    
    std::string BinOpImpl::getEntryName( RC::Handle<CG::Manager> const &cgManager ) const
    {
      RC::ConstHandle<ParamVector> params = getParams();
      return CG::binOpOverloadName( m_binOpType, cgManager, params->get(0)->getAdapterName(), params->get(1)->getAdapterName() );
    }
  };
};
