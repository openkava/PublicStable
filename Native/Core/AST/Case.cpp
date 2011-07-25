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
#include <Fabric/Core/Util/SimpleString.h>

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
    
    void Case::appendJSONMembers( Util::SimpleString &ss ) const
    {
      Node::appendJSONMembers(ss);
      if ( m_expr )
      {
        ss.append( ",\"expr\":" );
        m_expr->appendJSON( ss );
      }
      ss.append( ",\"statements\":" );
      m_statements->appendJSON( ss );
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
