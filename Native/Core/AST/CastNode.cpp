/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "CastNode.h"
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Adapter.h>

namespace Fabric
{
  namespace AST
  {
    CastNode::CastNode( CG::Location const &location, RC::ConstHandle< CG::Adapter > const &adapter, RC::ConstHandle<Expr> const &child )
      : Expr( location )
      , m_adapter( adapter )
      , m_child( child )
    {
    }
    
    std::string CastNode::localDesc() const
    {
      return "CastNode( " + _(m_adapter) + " )";
    }
    
    std::string CastNode::deepDesc( std::string const &indent ) const
    {
      return indent + localDesc() + "\n"
        + m_child->deepDesc(indent+"  ");
    }
    
    RC::ConstHandle<CG::Adapter> CastNode::getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      return m_adapter;
    }
    
    CG::ExprValue CastNode::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      if ( usage == CG::USAGE_LVALUE )
        throw CG::Error( getLocation(), "explicit casts "+lValueErrorDesc );
      else usage = CG::USAGE_RVALUE;
      try
      {
        return m_child->buildExprValue( basicBlockBuilder, usage, lValueErrorDesc ).castTo( basicBlockBuilder, m_adapter );
      }
      catch ( Exception e )
      {
        throw CG::Error( getLocation(), e );
      }
    }
  };
};
