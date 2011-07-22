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
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Array.h>

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
    
    RC::Handle<JSON::Object> Case::toJSON() const
    {
      RC::Handle<JSON::Object> result = Node::toJSON();
      if ( m_expr )
        result->set( "expr", m_expr->toJSON() );
      result->set( "statements", m_statements->toJSON() );
      return result;
    }
    
    void Case::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      if ( m_expr )
        m_expr->llvmPrepareModule( moduleBuilder, diagnostics );
      m_statements->llvmPrepareModule( moduleBuilder, diagnostics );
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
