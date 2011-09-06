/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_AST_CONTINUE_STATEMENT_H
#define _FABRIC_AST_CONTINUE_STATEMENT_H

#include <Fabric/Core/AST/Statement.h>

namespace Fabric
{
  namespace AST
  {
    class ContinueStatement: public Statement
    {
      FABRIC_AST_NODE_DECL( ContinueStatement );

    public:

      static RC::ConstHandle<ContinueStatement> Create( CG::Location const &location );
      
      virtual void registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const;
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      ContinueStatement( CG::Location const &location );
      
      virtual void appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const;
    };
  };
};

#endif //_FABRIC_AST_CONTINUE_STATEMENT_H
