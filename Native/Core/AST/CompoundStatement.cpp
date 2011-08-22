/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "CompoundStatement.h"
#include <Fabric/Core/AST/StatementVector.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( CompoundStatement );
    
    RC::ConstHandle<CompoundStatement> CompoundStatement::Create(
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
    
    void CompoundStatement::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Statement::appendJSONMembers( jsonObjectGenerator );
      m_statements->appendJSON( jsonObjectGenerator.makeMember( "statements" ) );
    }
    
    void CompoundStatement::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      m_statements->llvmPrepareModule( moduleBuilder, diagnostics );
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
