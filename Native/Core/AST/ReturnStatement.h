/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_RETURN_STATEMENT_H
#define _FABRIC_AST_RETURN_STATEMENT_H

#include <Fabric/Core/AST/Statement.h>
#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace Util
  {
    class SimpleString;
  };
  
  namespace AST
  {
    class ReturnStatement: public Statement
    {
      FABRIC_AST_NODE_DECL( ReturnStatement );

    public:

      static RC::ConstHandle<ReturnStatement> Create( CG::Location const &location, RC::ConstHandle<Expr> const &expr = RC::ConstHandle<Expr>() )
      {
        return new ReturnStatement( location, expr );
      }

      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      ReturnStatement( CG::Location const &location, RC::ConstHandle<Expr> const &expr);
      
      virtual void appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const;
    
    private:
    
      RC::ConstHandle<Expr> m_expr;
    };
  };
};

#endif //_FABRIC_AST_RETURN_STATEMENT_H
