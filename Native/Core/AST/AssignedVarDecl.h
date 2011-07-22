/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_ASSIGNED_VAR_DECL_H
#define _FABRIC_AST_ASSIGNED_VAR_DECL_H

#include <Fabric/Core/AST/VarDecl.h>
#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace AST
  {
    class AssignedVarDecl : public VarDecl
    {
      FABRIC_AST_NODE_DECL( AssignedVarDecl );
      
    public:
    
      static RC::ConstHandle<AssignedVarDecl> Create(
        CG::Location const &location,
        std::string const &name,
        std::string const &arrayModifier,
        RC::ConstHandle<Expr> initialExpr
        );

      RC::Handle<JSON::Object> toJSON() const;

      virtual void llvmPrepareModule( std::string const &baseType, CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
      
      virtual void llvmCompileToBuilder( std::string const &baseType, CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      AssignedVarDecl(
        CG::Location const &location,
        std::string const &name,
        std::string const &arrayModifier,
        RC::ConstHandle<Expr> const &initialExpr
        );
    
    private:
    
      RC::ConstHandle<Expr> m_initialExpr;
    };
  };
};

#endif //_FABRIC_AST_VAR_DECL_H
