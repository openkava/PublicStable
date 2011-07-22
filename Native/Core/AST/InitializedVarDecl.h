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

namespace Fabric
{
  namespace AST
  {
    class ExprVector;
    
    class InitializedVarDecl: public VarDecl
    {
      FABRIC_AST_NODE_DECL( InitializedVarDecl );

    public:

      static RC::Handle<InitializedVarDecl> Create(
        CG::Location const &location,
        std::string const &name,
        std::string const &arrayModifier,
        RC::ConstHandle<ExprVector> const &args
        );

      RC::Handle<JSON::Object> toJSON() const;

      virtual void llvmPrepareModule( std::string const &baseType, CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;

      virtual void llvmCompileToBuilder( std::string const &baseType, CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      InitializedVarDecl(
        CG::Location const &location,
        std::string const &name,
        std::string const &arrayModifier,
        RC::ConstHandle<ExprVector> const &args
        );
    
    private:
    
      RC::ConstHandle<ExprVector> m_args;
    };
  };
};

#endif //_FABRIC_AST_INITIALIZED_VAR_DECL_H
