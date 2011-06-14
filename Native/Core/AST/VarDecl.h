/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_VAR_DECL_H
#define _FABRIC_AST_VAR_DECL_H

#include <Fabric/Core/AST/Statement.h>
#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace AST
  {
    class VarDecl: public Statement
    {
    public:
    
      virtual std::string localDesc() const;

      static RC::Handle<VarDecl> Create( CG::Location const &location, std::string const &name, RC::ConstHandle< CG::Adapter > const &adapter )
      {
        return new VarDecl( location, name, adapter );
      }
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      VarDecl( CG::Location const &location, std::string const &name, RC::ConstHandle< CG::Adapter > const &adapter );
    
      CG::ExprValue llvmAllocateVariable( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;

    private:
    
      std::string m_name;
      RC::ConstHandle< CG::Adapter > m_adapter;
    };
  };
};

#endif //_FABRIC_AST_VAR_DECL_H
