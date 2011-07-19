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
    
    RC::Handle<Initializer> Initializer::Create(
      CG::Location const &location,
      std::string const &selfType,
      RC::ConstHandle<ParamVector> const &params,
      RC::ConstHandle<CompoundStatement> const &body
      )
    {
      return new Initializer( location, selfType, params, body );
    }
    
    Initializer::Initializer(
      CG::Location const &location,
      std::string const &selfType,
      RC::ConstHandle<ParamVector> const &params,
      RC::ConstHandle<CompoundStatement> const &body
      )
      : FunctionBase(
        location,
        "",
        ParamVector::Create(
          Param::Create(
            location,
            "self",
            selfType,
            CG::USAGE_LVALUE
            ),
          params
          ),
        body
        )
      , m_selfType( selfType )
      , m_params( params )
    {
    }
    
    std::string Initializer::getEntryName( RC::Handle<CG::Manager> const &cgManager ) const
    {
      return CG::constructOverloadName( cgManager, m_selfType, m_params->getTypes( cgManager ) );
    }
  };
};
