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
    public:
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;

      static RC::Handle<AssignedVarDecl> Create(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle< CG::Adapter > const &adapter,
        RC::ConstHandle<Expr> initialExpr
        )
      {
        return new AssignedVarDecl( location, name, adapter, initialExpr );
      }
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      AssignedVarDecl(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle< CG::Adapter > const &adapter,
        RC::ConstHandle<Expr> const &initialExpr
        );
    
    private:
    
      RC::ConstHandle<Expr> m_initialExpr;
    };
  };
};

#endif //_FABRIC_AST_VAR_DECL_H
