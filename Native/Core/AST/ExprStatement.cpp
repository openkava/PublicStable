/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/ExprStatement.h>
#include <Fabric/Core/AST/Expr.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( ExprStatement );
    
    RC::ConstHandle<ExprStatement> ExprStatement::Create( CG::Location const &location, RC::ConstHandle<Expr> const &expr )
    {
      return new ExprStatement( location, expr );
    }
    
    ExprStatement::ExprStatement( CG::Location const &location, RC::ConstHandle<Expr> const &expr )
      : Statement( location )
      , m_expr( expr )
    {
    }
    
    void ExprStatement::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Statement::appendJSONMembers( jsonObjectGenerator );
      m_expr->appendJSON( jsonObjectGenerator.makeMember( "expr" ) );
    }
    
    void ExprStatement::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      m_expr->llvmPrepareModule( moduleBuilder, diagnostics );
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
