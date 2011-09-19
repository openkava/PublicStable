/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "UniOp.h"
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( UniOp );
    
    UniOp::UniOp( CG::Location const &location, CG::UniOpType uniOpType, RC::ConstHandle<Expr> const &child )
      : Expr( location )
      , m_uniOpType( uniOpType )
      , m_child( child )
    {
    }
    
    void UniOp::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const
    {
      Expr::appendJSONMembers( jsonObjectGenerator, includeLocation );
      jsonObjectGenerator.makeMember( "op" ).makeString( uniOpUserName( m_uniOpType ) );
      m_child->appendJSON( jsonObjectGenerator.makeMember( "child" ), includeLocation );
    }
    
    RC::ConstHandle<CG::FunctionSymbol> UniOp::getFunctionSymbol( CG::BasicBlockBuilder &basicBlockBuilder ) const
    {
      RC::ConstHandle<CG::Adapter> childType = m_child->getType( basicBlockBuilder );
      std::string functionName = CG::uniOpOverloadName( m_uniOpType, childType );
      RC::ConstHandle<CG::FunctionSymbol> functionSymbol = basicBlockBuilder.maybeGetFunction( functionName );
      if ( !functionSymbol )
        throw Exception( "unary operator " + _(CG::uniOpUserName( m_uniOpType )) + " not supported for expressions of type " + _(childType->getUserName()) );
      return functionSymbol;
    }
    
    void UniOp::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      m_child->registerTypes( cgManager, diagnostics );
    }
    
    RC::ConstHandle<CG::Adapter> UniOp::getType( CG::BasicBlockBuilder &basicBlockBuilder ) const
    {
      RC::ConstHandle<CG::Adapter> adapter = getFunctionSymbol( basicBlockBuilder )->getReturnInfo().getAdapter();
      adapter->llvmCompileToModule( basicBlockBuilder.getModuleBuilder() );
      return adapter;
    }
    
    CG::ExprValue UniOp::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      RC::ConstHandle< CG::FunctionSymbol > functionSymbol = getFunctionSymbol( basicBlockBuilder );
      std::vector<CG::FunctionParam> const &functionParams = functionSymbol->getParams();
      
      try
      {
        CG::ExprValue childExprValue = m_child->buildExprValue( basicBlockBuilder, functionParams[0].getUsage(), lValueErrorDesc );
        return functionSymbol->llvmCreateCall( basicBlockBuilder, childExprValue );
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
