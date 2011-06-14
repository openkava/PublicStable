/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "ExprStatement.h"
#include <Fabric/Core/CG/Error.h>

namespace Fabric
{
  namespace AST
  {
    ExprStatement::ExprStatement( CG::Location const &location, RC::ConstHandle<Expr> const &expr )
      : Statement( location )
      , m_expr( expr )
    {
    }
    
    std::string ExprStatement::localDesc() const
    {
      return "ExprStatement";
    }
    
    std::string ExprStatement::deepDesc( std::string const &indent ) const
    {
      return indent + localDesc() + "\n"
        + m_expr->deepDesc( indent + "  " );
    }

    void ExprStatement::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      try
      {
        m_expr->buildExprValue( basicBlockBuilder, CG::USAGE_UNSPECIFIED, "cannot be an l-value" ).llvmDispose( basicBlockBuilder );
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
