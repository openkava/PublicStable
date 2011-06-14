/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "CompoundStatement.h"
#include <Fabric/Core/CG/Scope.h>

namespace Fabric
{
  namespace AST
  {
    CompoundStatement::CompoundStatement( CG::Location const &location, RC::ConstHandle<StatementList> const &statementList )
      : Statement( location )
      , m_statementList( statementList )
    {
    }
    
    std::string CompoundStatement::localDesc() const
    {
      return "CompoundStatement";
    }
    
    std::string CompoundStatement::deepDesc( std::string const &indent ) const
    {
      return indent + localDesc() + "\n"
        + m_statementList->deepDesc( indent + "  " );
    }

    void CompoundStatement::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      CG::Scope subScope( basicBlockBuilder.getScope() );
      {
        CG::BasicBlockBuilder subBasicBlockBuilder( basicBlockBuilder, subScope );
        m_statementList->llvmCompileToBuilder( subBasicBlockBuilder, diagnostics );
      }
      if ( !basicBlockBuilder->GetInsertBlock()->getTerminator() )
        subScope.llvmUnwind( basicBlockBuilder );
    }
  };
};
