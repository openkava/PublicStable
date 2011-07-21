/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "ExprStatement.h"
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( ExprStatement );
    
    ExprStatement::ExprStatement( CG::Location const &location, RC::ConstHandle<Expr> const &expr )
      : Statement( location )
      , m_expr( expr )
    {
    }
    
    RC::Handle<JSON::Object> ExprStatement::toJSON() const
    {
      RC::Handle<JSON::Object> result = Statement::toJSON();
      result->set( "expr", m_expr->toJSON() );
      return result;
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
