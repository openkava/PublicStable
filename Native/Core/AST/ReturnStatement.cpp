/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "ReturnStatement.h"
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/FunctionBuilder.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( ReturnStatement );
    
    ReturnStatement::ReturnStatement( CG::Location const &location, RC::ConstHandle<Expr> const &expr )
      : Statement( location )
      , m_expr( expr )
    {
    }
    
    void ReturnStatement::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Statement::appendJSONMembers( jsonObjectGenerator );
      if ( m_expr )
        m_expr->appendJSON( jsonObjectGenerator.makeMember( "expr" ) );
    }
    
    void ReturnStatement::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctions ) const
    {
      if ( m_expr )
        m_expr->llvmPrepareModule( moduleBuilder, diagnostics, buildFunctions );
    }

    void ReturnStatement::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      try
      {
        CG::ReturnInfo const &returnInfo = basicBlockBuilder.getFunctionBuilder().getScope().getReturnInfo();
        if ( m_expr )
        {
          if ( !returnInfo )
            throw CG::Error( getLocation(), "functions with no return types do not return values" );
          CG::ExprValue returnExprValue = m_expr->buildExprValue( basicBlockBuilder, returnInfo.getUsage(), "cannot be assigned to" );
          basicBlockBuilder.getScope().llvmReturn( basicBlockBuilder, returnExprValue );
        }
        else
        {
          if ( returnInfo )
            throw CG::Error( getLocation(), "function must return a value" );
          basicBlockBuilder.getScope().llvmReturn( basicBlockBuilder, 0 );
        }
      }
      catch ( Exception e )
      {
        addError( diagnostics, e );
      }
    }
  };
};
