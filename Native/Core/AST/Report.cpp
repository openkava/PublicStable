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
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Report );
    
    Report::Report( CG::Location const &location, RC::ConstHandle<Expr> const &expr )
      : Statement( location )
      , m_expr( expr )
    {
    }
    
    void Report::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Statement::appendJSONMembers( jsonObjectGenerator );
      m_expr->appendJSON( jsonObjectGenerator.makeMember( "expr" ) );
    }
    
    void Report::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      m_expr->registerTypes( cgManager, diagnostics );
    }

    void Report::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      RC::ConstHandle< CG::StringAdapter > stringAdapter = basicBlockBuilder.getManager()->getStringAdapter();
      stringAdapter->llvmCompileToModule( basicBlockBuilder.getModuleBuilder() );

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
