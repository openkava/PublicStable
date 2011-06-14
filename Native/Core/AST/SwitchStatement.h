/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_SWITCH_STATEMENT_H
#define _FABRIC_AST_SWITCH_STATEMENT_H

#include <Fabric/Core/AST/Statement.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace AST
  {
    class Expr;
    class CaseList;
    class StatementList;
    
    class SwitchStatement: public Statement
    {
    public:

      static RC::Handle<SwitchStatement> Create(
        CG::Location const &location,
        RC::ConstHandle<Expr> const &expr,
        RC::ConstHandle<CaseList> const &caseList
        );
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      SwitchStatement(
        CG::Location const &location,
        RC::ConstHandle<Expr> const &expr,
        RC::ConstHandle<CaseList> const &caseList
        );
    
    private:
    
      RC::ConstHandle<Expr> m_expr;
      RC::ConstHandle<CaseList> m_caseList;
    };
  };
};

#endif //_FABRIC_AST_SWITCH_STATEMENT_H
