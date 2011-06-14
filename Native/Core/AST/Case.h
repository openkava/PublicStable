/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_CASE_H
#define _FABRIC_AST_CASE_H

#include <Fabric/Core/AST/Node.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace AST
  {
    class Expr;
    class StatementList;
    
    class Case: public Node
    {
    public:
    
      static RC::Handle<Case> Create( CG::Location const &location, RC::ConstHandle<Expr> const &expr, RC::ConstHandle<StatementList> const &statementList );
      
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;

      RC::ConstHandle<Expr> getExpr() const;
      RC::ConstHandle<StatementList> getStatementList() const;
     
    protected:
    
      Case( CG::Location const &location, RC::ConstHandle<Expr> const &expr, RC::ConstHandle<StatementList> const &statementList );
    
    private:
    
      RC::ConstHandle<Expr> m_expr;
      RC::ConstHandle<StatementList> m_statementList;
    };
  };
};

#endif //_FABRIC_AST_CASE_H
