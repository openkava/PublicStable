/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/Case.h>
#include <Fabric/Core/AST/Expr.h>
#include <Fabric/Core/AST/StatementList.h>
#include <Fabric/Core/CG/Scope.h>

namespace Fabric
{
  namespace AST
  {
    RC::Handle<Case> Case::Create( CG::Location const &location, RC::ConstHandle<Expr> const &expr, RC::ConstHandle<StatementList> const &statementList )
    {
      return new Case( location, expr, statementList );
    }
    
    Case::Case( CG::Location const &location, RC::ConstHandle<Expr> const &expr, RC::ConstHandle<StatementList> const &statementList )
      : Node( location )
      , m_expr( expr )
      , m_statementList( statementList )
    {
    }
    
    std::string Case::localDesc() const
    {
      return "Case( " + m_expr->localDesc() + " )";
    }
    
    std::string Case::deepDesc( std::string const &indent ) const
    {
      std::string subIndent = "  " + indent;
      std::string result = indent + "Case\n";
      if ( m_expr )
        result += m_expr->deepDesc( subIndent );
      if ( m_statementList )
        result += m_statementList->deepDesc( subIndent );
      return result;
    }

    RC::ConstHandle<Expr> Case::getExpr() const
    {
      return m_expr;
    }
    
    RC::ConstHandle<StatementList> Case::getStatementList() const
    {
      return m_statementList;
    }
  };
};
