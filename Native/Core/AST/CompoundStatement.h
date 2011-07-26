/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_COMPOUND_STATEMENT_H
#define _FABRIC_AST_COMPOUND_STATEMENT_H

#include <Fabric/Core/AST/Statement.h>

#include <vector>

namespace Fabric
{
  namespace AST
  {
    class StatementVector;
    
    class CompoundStatement : public Statement
    {
      FABRIC_AST_NODE_DECL( CompoundStatement );

    public:

      static RC::ConstHandle<CompoundStatement> Create(
        CG::Location const &location,
        RC::ConstHandle<StatementVector> const &statements
        );

      RC::Handle<JSON::Object> toJSONImpl() const;
      
      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      CompoundStatement(
        CG::Location const &location,
        RC::ConstHandle<StatementVector> const &statements
        );
    
    private:
    
      RC::ConstHandle<StatementVector> m_statements;
    };
  };
};

#endif //_FABRIC_AST_COMPOUND_STATEMENT_H
