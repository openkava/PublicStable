/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_CONDITIONAL_STATEMENT_H
#define _FABRIC_AST_CONDITIONAL_STATEMENT_H

#include <Fabric/Core/AST/Statement.h>
#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace AST
  {
    class ConditionalStatement: public Statement
    {
    public:
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;

      static RC::Handle<ConditionalStatement> Create(
        CG::Location const &location,
        RC::ConstHandle<Expr> const &expr,
        RC::ConstHandle<Statement> const &trueStatement,
        RC::ConstHandle<Statement> const &falseStatement = RC::ConstHandle<Statement>()
        )
      {
        return new ConditionalStatement( location, expr, trueStatement, falseStatement );
      }
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      ConditionalStatement(
        CG::Location const &location,
        RC::ConstHandle<Expr> const &expr,
        RC::ConstHandle<Statement> const &trueStatement,
        RC::ConstHandle<Statement> const &falseStatement = RC::ConstHandle<Statement>()
        );
    
    private:
    
      RC::ConstHandle<Expr> m_expr;
      RC::ConstHandle<Statement> m_trueStatement;
      RC::ConstHandle<Statement> m_falseStatement;
    };
  };
};

#endif //_FABRIC_AST_CONDITIONAL_STATEMENT_H
