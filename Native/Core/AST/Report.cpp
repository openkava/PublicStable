/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "Report.h"
#include <Fabric/Core/CG/StringAdapter.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/BasicBlockBuilder.h>

namespace Fabric
{
  namespace AST
  {
    Report::Report( CG::Location const &location, RC::ConstHandle<Expr> const &expr )
      : Statement( location )
      , m_expr( expr )
    {
    }
    
    std::string Report::localDesc() const
    {
      return "Report";
    }
    
    std::string Report::deepDesc( std::string const &indent ) const
    {
      return indent + localDesc() + "\n"
        + m_expr->deepDesc(indent+"  ");
    }

    void Report::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      RC::ConstHandle< CG::StringAdapter > stringAdapter = basicBlockBuilder.getManager()->getStringAdapter();

      try
      {
        CG::ExprValue exprExprValue = m_expr->buildExprValue( basicBlockBuilder, CG::USAGE_RVALUE, "cannot be an l-value" );

        llvm::Value *stringRValue = stringAdapter->llvmCast( basicBlockBuilder, exprExprValue );
        
        stringAdapter->llvmReport( basicBlockBuilder, stringRValue );
        stringAdapter->llvmRelease( basicBlockBuilder, stringRValue );
      }
      catch ( CG::Error e )
      {
        addError( diagnostics, e );
      }
      catch ( Exception e )
      {
        addError( diagnostics, e );
      }
    }
  };
};
