/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/Case.h>
#include <Fabric/Core/AST/Expr.h>
#include <Fabric/Core/AST/StatementVector.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Case );
    
    RC::ConstHandle<Case> Case::Create(
        CG::Location const &location,
        RC::ConstHandle<Expr> const &expr,
        RC::ConstHandle<StatementVector> const &statements
        )
    {
      return new Case( location, expr, statements );
    }
    
    Case::Case(
        CG::Location const &location,
        RC::ConstHandle<Expr> const &expr,
        RC::ConstHandle<StatementVector> const &statements
        )
      : Node( location )
      , m_expr( expr )
      , m_statements( statements )
    {
    }
    
    void Case::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Node::appendJSONMembers( jsonObjectGenerator );
      if ( m_expr )
        m_expr->appendJSON( jsonObjectGenerator.makeMember( "expr" ) );
      m_statements->appendJSON( jsonObjectGenerator.makeMember( "statements" ) );
    }
    
    void Case::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      if ( m_expr )
        m_expr->registerTypes( cgManager, diagnostics );
      m_statements->registerTypes( cgManager, diagnostics );
    }

    RC::ConstHandle<Expr> Case::getExpr() const
    {
      return m_expr;
    }
    
    RC::ConstHandle<StatementVector> Case::getStatements() const
    {
      return m_statements;
    }
  };
};
