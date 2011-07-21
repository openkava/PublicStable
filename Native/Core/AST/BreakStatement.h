/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_BREAK_STATEMENT_H
#define _FABRIC_AST_BREAK_STATEMENT_H

#include <Fabric/Core/AST/Statement.h>
#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace AST
  {
    class BreakStatement: public Statement
    {
      FABRIC_AST_NODE_DECL( BreakStatement );
      
    public:

      static RC::Handle<BreakStatement> Create( CG::Location const &location )
      {
        return new BreakStatement( location );
      }
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      BreakStatement( CG::Location const &location );
    };
  };
};

#endif //_FABRIC_AST_BREAK_STATEMENT_H
