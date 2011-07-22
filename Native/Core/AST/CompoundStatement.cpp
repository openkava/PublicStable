/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "CompoundStatement.h"
#include <Fabric/Core/AST/StatementVector.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( CompoundStatement );
    
    RC::Handle<CompoundStatement> CompoundStatement::Create(
      CG::Location const &location,
      RC::ConstHandle<StatementVector> const &statements
      )
    {
      return new CompoundStatement( location, statements );
    }
    
    CompoundStatement::CompoundStatement(
      CG::Location const &location,
      RC::ConstHandle<StatementVector> const &statements
      )
      : Statement( location )
      , m_statements( statements )
    {
    }
    
    RC::Handle<JSON::Object> CompoundStatement::toJSON() const
    {
      RC::Handle<JSON::Object> result = Node::toJSON();
      result->set( "statements", m_statements->toJSON() );
      return result;
    }

    void CompoundStatement::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      CG::Scope subScope( basicBlockBuilder.getScope() );
      {
        CG::BasicBlockBuilder subBasicBlockBuilder( basicBlockBuilder, subScope );
        for ( size_t i=0; i<m_statements->size(); ++i )
          m_statements->get(i)->llvmCompileToBuilder( subBasicBlockBuilder, diagnostics );
      }
      if ( !basicBlockBuilder->GetInsertBlock()->getTerminator() )
        subScope.llvmUnwind( basicBlockBuilder );
    }
  };
};
