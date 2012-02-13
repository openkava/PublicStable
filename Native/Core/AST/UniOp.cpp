/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "UniOp.h"
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Function.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Core/CG/PencilSymbol.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( UniOp );
    
    RC::ConstHandle<UniOp> UniOp::Create(
      CG::Location const &location,
      CG::UniOpType uniOpType,
      RC::ConstHandle<Expr> const &child
      )
    {
      return new UniOp( location, uniOpType, child );
    }

    UniOp::UniOp( CG::Location const &location, CG::UniOpType uniOpType, RC::ConstHandle<Expr> const &child )
      : Expr( location )
      , m_uniOpType( uniOpType )
      , m_child( child )
    {
    }
    
    void UniOp::appendJSONMembers( JSON::ObjectEncoder const &jsonObjectEncoder, bool includeLocation ) const
    {
      Expr::appendJSONMembers( jsonObjectEncoder, includeLocation );
      jsonObjectEncoder.makeMember( "op" ).makeString( uniOpUserName( m_uniOpType ) );
      m_child->appendJSON( jsonObjectEncoder.makeMember( "child" ), includeLocation );
    }
    
    CG::Function const &UniOp::getFunction( CG::BasicBlockBuilder &basicBlockBuilder ) const
    {
      CG::ExprType childExprType = m_child->getExprType( basicBlockBuilder );
      std::string pencilName = CG::UniOpPencilName( m_uniOpType );
      RC::ConstHandle<CG::PencilSymbol> pencilSymbol = basicBlockBuilder.maybeGetPencil( pencilName );
      if ( !pencilSymbol )
        throw Exception( "unary operator " + _(CG::uniOpUserName( m_uniOpType )) + " not supported for expressions of type " + _(childExprType.getUserName()) );
      return pencilSymbol->getFunction( getLocation(), childExprType );
    }
    
    void UniOp::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      m_child->registerTypes( cgManager, diagnostics );
    }
    
    CG::ExprType UniOp::getExprType( CG::BasicBlockBuilder &basicBlockBuilder ) const
    {
      RC::ConstHandle<CG::Adapter> adapter = getFunction( basicBlockBuilder ).getReturnInfo().getAdapter();
      adapter->llvmCompileToModule( basicBlockBuilder.getModuleBuilder() );
      return CG::ExprType( adapter, CG::USAGE_RVALUE );
    }
    
    CG::ExprValue UniOp::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      CG::Function const &function = getFunction( basicBlockBuilder );
      CG::ParamVector const &functionParams = function.getParams();
      
      try
      {
        CG::ExprValue childExprValue = m_child->buildExprValue( basicBlockBuilder, functionParams[0].getUsage(), lValueErrorDesc );
        return function.llvmCreateCall( basicBlockBuilder, childExprValue );
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
