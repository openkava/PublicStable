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
      FABRIC_AST_NODE_DECL( ConditionalStatement );

    public:

      static RC::ConstHandle<ConditionalStatement> Create(
        CG::Location const &location,
        RC::ConstHandle<Expr> const &expr,
        RC::ConstHandle<Statement> const &trueStatement,
        RC::ConstHandle<Statement> const &falseStatement = RC::ConstHandle<Statement>()
        );

      RC::Handle<JSON::Object> toJSONImpl() const;
      
      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
      
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
