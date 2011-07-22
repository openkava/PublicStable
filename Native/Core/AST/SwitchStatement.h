/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_SWITCH_STATEMENT_H
#define _FABRIC_AST_SWITCH_STATEMENT_H

#include <Fabric/Core/AST/Statement.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace AST
  {
    class Expr;
    class CaseVector;
    class StatementVector;
    
    class SwitchStatement: public Statement
    {
      FABRIC_AST_NODE_DECL( SwitchStatement );

    public:

      static RC::ConstHandle<SwitchStatement> Create(
        CG::Location const &location,
        RC::ConstHandle<Expr> const &expr,
        RC::ConstHandle<CaseVector> const &cases
        );

      RC::Handle<JSON::Object> toJSON() const;

      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      SwitchStatement(
        CG::Location const &location,
        RC::ConstHandle<Expr> const &expr,
        RC::ConstHandle<CaseVector> const &cases
        );
    
    private:
    
      RC::ConstHandle<Expr> m_expr;
      RC::ConstHandle<CaseVector> m_cases;
    };
  };
};

#endif //_FABRIC_AST_SWITCH_STATEMENT_H
