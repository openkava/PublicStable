/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_STATEMENT_LIST_NODE_H
#define _FABRIC_AST_STATEMENT_LIST_NODE_H

#include <Fabric/Core/AST/ListNode.h>
#include <Fabric/Core/AST/Statement.h>

namespace Fabric
{
  namespace AST
  {
    class StatementList : public Statement
    {
    public:
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;

      static RC::Handle<StatementList> Create( CG::Location const &location )
      {
        std::vector< RC::ConstHandle<Statement> > statements;
        return new StatementList( location, statements );
      }
      
      static RC::Handle<StatementList> Create( CG::Location const &location, RC::ConstHandle<Statement> const &statement )
      {
        std::vector< RC::ConstHandle<Statement> > statements;
        statements.push_back( statement );
        return new StatementList( location, statements );
      }
      
      static RC::Handle<StatementList> Create( CG::Location const &location, RC::ConstHandle<Statement> const &statement, RC::ConstHandle<StatementList> const &statementList )
      {
        std::vector< RC::ConstHandle<Statement> > statements;
        statements.push_back( statement );
        statements.insert( statements.end(), statementList->m_statements.begin(), statementList->m_statements.end() );
        return new StatementList( location, statements );
      }
      
      void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
      
    protected:
    
      StatementList( CG::Location const &location, std::vector< RC::ConstHandle<Statement> > const &statements );
    
    private:
    
      std::vector< RC::ConstHandle<Statement> > m_statements;
    };
  };
};

#endif //_FABRIC_AST_STATEMENT_LIST_NODE_H
