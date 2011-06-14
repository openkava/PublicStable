/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/StatementList.h>
#include <Fabric/Core/CG/BasicBlockBuilder.h>

namespace Fabric
{
  namespace AST
  {
    StatementList::StatementList( CG::Location const &location, std::vector< RC::ConstHandle<Statement> > const &statements )
      : Statement( location )
      , m_statements( statements )
    {
    }
    
    std::string StatementList::localDesc() const
    {
      return "StatementList";
    }
    
    std::string StatementList::deepDesc( std::string const &indent ) const
    {
      std::string result = indent + localDesc() + "\n";
      std::string subIndent = indent + "  ";
      for ( size_t i=0; i<m_statements.size(); ++i )
        result += m_statements[i]->deepDesc( subIndent );
      return result;
    }
    
    void StatementList::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      for ( size_t i=0; i<m_statements.size(); ++i )
      {
        if ( basicBlockBuilder->GetInsertBlock()->getTerminator() )
        {
          m_statements[i]->addError( diagnostics, "code is unreachable" );
          break;
        }
        m_statements[i]->llvmCompileToBuilder( basicBlockBuilder, diagnostics );
      }
    }
  };
};
