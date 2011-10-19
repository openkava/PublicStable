/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_AST_CONTAINER_LOOP_H
#define _FABRIC_AST_CONTAINER_LOOP_H

#include <Fabric/Core/AST/Statement.h>

namespace Fabric
{
  namespace CG
  {
    class Diagnostics;
    class Manager;
  };
  
  namespace AST
  {
    class Expr;
    
    class ContainerLoop: public Statement
    {
      FABRIC_AST_NODE_DECL( ContainerLoop );

    public:

      static RC::ConstHandle<ContainerLoop> Create(
        CG::Location const &location,
        RC::ConstHandle<Expr> const &dictExpr,
        std::string const &keyName,
        RC::ConstHandle<Statement> const &body
        );
      
      virtual void registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const;
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      ContainerLoop(
        CG::Location const &location,
        RC::ConstHandle<Expr> const &dictExpr,
        std::string const &keyName,
        RC::ConstHandle<Statement> const &body
        );
      
      virtual void appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const;
    
    private:
    
      RC::ConstHandle<Expr> m_dictExpr;
      std::string m_keyName;
      RC::ConstHandle<Statement> m_body;
    };
  };
};

#endif //_FABRIC_AST_CONTAINER_LOOP_H
