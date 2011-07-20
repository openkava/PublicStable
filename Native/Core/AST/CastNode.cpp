/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/CastNode.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( CastNode );
    
    RC::Handle<CastNode> CastNode::Create(
      CG::Location const &location,
      std::string const &type,
      RC::ConstHandle<Expr> const &child
      )
    {
      return new CastNode( location, type, child );
    }
    
    CastNode::CastNode(
      CG::Location const &location,
      std::string const &type,
      RC::ConstHandle<Expr> const &child
      )
      : Expr( location )
      , m_type( type )
      , m_child( child )
    {
    }
    
    RC::Handle<JSON::Object> CastNode::toJSON() const
    {
      RC::Handle<JSON::Object> result = Node::toJSON();
      result->set( "type", JSON::String::Create( m_type ) );
      result->set( "child", m_child->toJSON() );
      return result;
    }
    
    RC::ConstHandle<CG::Adapter> CastNode::getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      return basicBlockBuilder.getAdapter( m_type );
    }
    
    CG::ExprValue CastNode::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      if ( usage == CG::USAGE_LVALUE )
        throw CG::Error( getLocation(), "explicit casts "+lValueErrorDesc );
      else usage = CG::USAGE_RVALUE;
      
      try
      {
        return m_child->buildExprValue( basicBlockBuilder, usage, lValueErrorDesc ).castTo( basicBlockBuilder, getType( basicBlockBuilder ) );
      }
      catch ( Exception e )
      {
        throw CG::Error( getLocation(), e );
      }
    }
  };
};
