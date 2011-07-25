/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_VAR_DECL_STATEMENT_H
#define _FABRIC_AST_VAR_DECL_STATEMENT_H

#include <Fabric/Core/AST/Statement.h>

namespace Fabric
{
  namespace Util
  {
    class SimpleString;
  };
  
  namespace AST
  {
    class VarDeclVector;
    
    class VarDeclStatement : public Statement
    {
      FABRIC_AST_NODE_DECL( VarDecl );
      
    public:

      static RC::ConstHandle<VarDeclStatement> Create(
        CG::Location const &location,
        std::string const &baseType,
        RC::ConstHandle<VarDeclVector> const &varDecls
        );

      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      VarDeclStatement(
        CG::Location const &location,
        std::string const &baseType,
        RC::ConstHandle<VarDeclVector> const &varDecls
        );
      
      virtual void appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const;

    private:
    
      std::string m_baseType;
      RC::ConstHandle<VarDeclVector> m_varDecls;
    };
  };
};

#endif //_FABRIC_AST_VAR_DECL_STATEMENT_H
