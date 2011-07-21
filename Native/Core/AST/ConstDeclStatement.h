/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_CONST_DECL_STATEMENT_H
#define _FABRIC_AST_CONST_DECL_STATEMENT_H

#include <Fabric/Core/AST/Statement.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace CG
  {
    class Adapter;
  };
  
  namespace AST
  {
    class ConstDecl;
    
    class ConstDeclStatement : public Statement
    {
      FABRIC_AST_NODE_DECL( ConstDeclStatement );

    public:

      static RC::Handle<ConstDeclStatement> Create(
        CG::Location const &location,
        RC::ConstHandle<ConstDecl> const &constDecl
        )
      {
        return new ConstDeclStatement( location, constDecl );
      }

      RC::Handle<JSON::Object> toJSON() const;
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
      
    protected:
    
      ConstDeclStatement(
        CG::Location const &location,
        RC::ConstHandle<ConstDecl> const &constDecl
        );
    
    private:
    
      RC::ConstHandle<ConstDecl> m_constDecl;
    };
  };
};

#endif //_FABRIC_AST_CONST_DECL_STATEMENT_H
