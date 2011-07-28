/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_C_STYLE_LOOP_H
#define _FABRIC_AST_C_STYLE_LOOP_H

#include <Fabric/Core/AST/Statement.h>

namespace Fabric
{
  namespace AST
  {
    class Expr;
    
    class CStyleLoop: public Statement
    {
      FABRIC_AST_NODE_DECL( CStyleLoop );

    public:

      static RC::ConstHandle<CStyleLoop> Create(
        CG::Location const &location,
        RC::ConstHandle<Statement> const &startStatement,
        RC::ConstHandle<Expr> const &preCondExpr,
        RC::ConstHandle<Expr> const &nextExpr,
        RC::ConstHandle<Expr> const &postCondExpr,
        RC::ConstHandle<Statement> const &body
        );
      
      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      CStyleLoop(
        CG::Location const &location,
        RC::ConstHandle<Statement> const &startStatement,
        RC::ConstHandle<Expr> const &preCondExpr,
        RC::ConstHandle<Expr> const &nextExpr,
        RC::ConstHandle<Expr> const &postCondExpr,
        RC::ConstHandle<Statement> const &body
        );
      
      virtual void appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const;
    
    private:
    
      RC::ConstHandle<Statement> m_startStatement;
      RC::ConstHandle<Expr> m_preCondExpr;
      RC::ConstHandle<Expr> m_nextExpr;
      RC::ConstHandle<Expr> m_postCondExpr;
      RC::ConstHandle<Statement> m_body;
    };
  };
};

#endif //_FABRIC_AST_C_STYLE_LOOP_H
