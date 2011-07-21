/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/MethodOpImpl.h>
#include <Fabric/Core/AST/Param.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( MethodOpImpl );
    
    RC::Handle<MethodOpImpl> MethodOpImpl::Create(
      CG::Location const &location,
      std::string const &returnTypeName,
      std::string const &selfTypeName,
      std::string const &methodName,
      RC::ConstHandle<ParamVector> const &params,
      RC::ConstHandle<CompoundStatement> const &body
      )
    {
      return new MethodOpImpl( location, returnTypeName, selfTypeName, methodName, params, body );
    }
    
    MethodOpImpl::MethodOpImpl(
      CG::Location const &location,
      std::string const &returnTypeName,
      std::string const &selfTypeName,
      std::string const &methodName,
      RC::ConstHandle<ParamVector> const &params,
      RC::ConstHandle<CompoundStatement> const &body
      )
      : FunctionBase(
        location,
        returnTypeName,
        body
        )
      , m_selfTypeName( selfTypeName )
      , m_methodName( methodName )
      , m_params( m_params )
    {
    }
          
    std::string MethodOpImpl::getEntryName( RC::Handle<CG::Manager> const &cgManager ) const
    {
      return CG::methodOverloadName( m_methodName, cgManager->getAdapter( m_selfTypeName ), m_params->getAdapters( cgManager ) );
    }
    
    RC::ConstHandle<ParamVector> MethodOpImpl::getParams( RC::Handle<CG::Manager> const &cgManager ) const
    {
      return ParamVector::Create(
        Param::Create( getLocation(), "self", m_selfTypeName, !getReturnType().empty() ? CG::USAGE_RVALUE : CG::USAGE_LVALUE ),
        m_params
        );
    }
  };
};
