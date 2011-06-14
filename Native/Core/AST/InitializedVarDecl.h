/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_INITIALIZED_VAR_DECL_H
#define _FABRIC_AST_INITIALIZED_VAR_DECL_H

#include <Fabric/Core/AST/VarDecl.h>
#include <Fabric/Core/AST/Expr.h>
#include <Fabric/Core/AST/ArgList.h>

namespace Fabric
{
  namespace AST
  {
    class InitializedVarDecl: public VarDecl
    {
    public:
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;

      static RC::Handle<InitializedVarDecl> Create(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<CG::Adapter> const &adapter,
        RC::ConstHandle<ArgList> const &args
        )
      {
        return new InitializedVarDecl( location, name, adapter, args );
      }

      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      InitializedVarDecl(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<CG::Adapter> const &adapter,
        RC::ConstHandle<ArgList> const &args
        );
    
    private:
    
      RC::ConstHandle<ArgList> m_argList;
    };
  };
};

#endif //_FABRIC_AST_INITIALIZED_VAR_DECL_H
