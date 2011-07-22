/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_EXPR_STATEMENT_H
#define _FABRIC_AST_EXPR_STATEMENT_H

#include <Fabric/Core/AST/Statement.h>
#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace AST
  {
    class ExprStatement: public Statement
    {
      FABRIC_AST_NODE_DECL( ExprStatement );

    public:

      static RC::Handle<ExprStatement> Create( CG::Location const &location, RC::ConstHandle<Expr> const &expr )
      {
        return new ExprStatement( location, expr );
      }

      RC::Handle<JSON::Object> toJSON() const;

      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      ExprStatement( CG::Location const &location, RC::ConstHandle<Expr> const &expr );
    
    private:
    
      RC::ConstHandle<Expr> m_expr;
    };
  };
};

#endif //_FABRIC_AST_EXPR_STATEMENT_H
