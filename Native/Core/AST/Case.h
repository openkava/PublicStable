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

#include <vector>

namespace Fabric
{
  namespace CG
  {
    class ModuleBuilder;
  };
  
  namespace AST
  {
    class Expr;
    class StatementVector;
    
    class Case : public Node
    {
      FABRIC_AST_NODE_DECL( Case );

    public:
    
      static RC::ConstHandle<Case> Create(
        CG::Location const &location,
        RC::ConstHandle<Expr> const &expr,
        RC::ConstHandle<StatementVector> const &statements
        );

      RC::Handle<JSON::Object> toJSONImpl() const;
      
      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;

      RC::ConstHandle<Expr> getExpr() const;
      RC::ConstHandle<StatementVector> getStatements() const;
           
    protected:
    
      Case(
        CG::Location const &location,
        RC::ConstHandle<Expr> const &expr,
        RC::ConstHandle<StatementVector> const &statements
        );
    
    private:
    
      RC::ConstHandle<Expr> m_expr;
      RC::ConstHandle<StatementVector> m_statements;
    };
  };
};

#endif //_FABRIC_AST_CASE_H
