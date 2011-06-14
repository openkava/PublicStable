/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_COMPOUND_STATEMENT_H
#define _FABRIC_AST_COMPOUND_STATEMENT_H

#include <Fabric/Core/AST/Statement.h>
#include <Fabric/Core/AST/StatementList.h>

namespace Fabric
{
  namespace AST
  {
    class CompoundStatement: public Statement
    {
    public:
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;

      static RC::Handle<CompoundStatement> Create( CG::Location const &location, RC::ConstHandle<StatementList> const &statementList )
      {
        return new CompoundStatement( location, statementList );
      }
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      CompoundStatement( CG::Location const &location, RC::ConstHandle<StatementList> const &statementList );
    
    private:
    
      RC::ConstHandle<StatementList> m_statementList;
    };
  };
  
  inline std::string _( RC::ConstHandle<AST::CompoundStatement> const &compoundStatement )
  {
    return compoundStatement->localDesc();
  }
};

#endif //_FABRIC_AST_COMPOUND_STATEMENT_H
