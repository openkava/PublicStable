/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_AST_CONTINUE_STATEMENT_H
#define _FABRIC_AST_CONTINUE_STATEMENT_H

#include <Fabric/Core/AST/Statement.h>
#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace AST
  {
    class ContinueStatement: public Statement
    {
    public:
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;

      static RC::Handle<ContinueStatement> Create( CG::Location const &location )
      {
        return new ContinueStatement( location );
      }
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      ContinueStatement( CG::Location const &location );
    };
  };
};

#endif //_FABRIC_AST_CONTINUE_STATEMENT_H
