/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "NotOp.h"
#include <Fabric/Core/CG/BooleanAdapter.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( NotOp );
    
    NotOp::NotOp( CG::Location const &location, RC::ConstHandle<Expr> const &child )
      : Expr( location )
      , m_child( child )
    {
    }
    
    void NotOp::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Expr::appendJSONMembers( jsonObjectGenerator );
      m_child->appendJSON( jsonObjectGenerator.makeMember( "child" ) );
    }
    
    void NotOp::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      m_child->llvmPrepareModule( moduleBuilder, diagnostics );
    }
    
    RC::ConstHandle<CG::Adapter> NotOp::getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      return basicBlockBuilder.getManager()->getBooleanAdapter();
    }
    
    CG::ExprValue NotOp::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      try
      {
        if ( usage == CG::USAGE_LVALUE )
          throw CG::Error( getLocation(), "result of logical not "+lValueErrorDesc );
          
        CG::ExprValue childExprValue = m_child->buildExprValue( basicBlockBuilder, CG::USAGE_RVALUE, lValueErrorDesc );
        RC::ConstHandle< CG::Adapter > booleanAdapter = basicBlockBuilder.getManager()->getBooleanAdapter();
        llvm::Value *booleanRValue = booleanAdapter->llvmCast( basicBlockBuilder, childExprValue );
        llvm::Value *notBooleanRValue = basicBlockBuilder->CreateNot( booleanRValue );
        return CG::ExprValue( booleanAdapter, CG::USAGE_RVALUE, basicBlockBuilder.getContext(), notBooleanRValue );
      }
      catch ( CG::Error e )
      {
        throw e;
      }
      catch ( Exception e )
      {
        throw CG::Error( getLocation(), e );
      }
    }
  };
};
