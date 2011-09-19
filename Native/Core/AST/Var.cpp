/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "Var.h"
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Var );
    
    Var::Var( CG::Location const &location, std::string const &name )
      : Expr( location )
      , m_name( name )
    {
    }
    
    void Var::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const
    {
      Expr::appendJSONMembers( jsonObjectGenerator, includeLocation );
      jsonObjectGenerator.makeMember( "name" ).makeString( m_name );
    }
    
    void Var::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
    }
    
    RC::ConstHandle<CG::ValueSymbol> Var::getValueSymbol( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      try
      {
        RC::ConstHandle<CG::Symbol> symbol = basicBlockBuilder.getScope().get( m_name );
        if ( !symbol )
          throw CG::Error( getLocation(), "symbol not found" );
          
        if ( !symbol->isValue() )
          throw CG::Error( getLocation(), "not a value" );
        return RC::ConstHandle<CG::ValueSymbol>::StaticCast( symbol );
      }
      catch ( CG::Error e )
      {
        throw _(m_name) + ": " + e;
      }
    }
    
    RC::ConstHandle<CG::Adapter> Var::getType( CG::BasicBlockBuilder &basicBlockBuilder ) const
    {
      RC::ConstHandle<CG::Adapter> adapter = getValueSymbol( basicBlockBuilder )->getAdapter();
      adapter->llvmCompileToModule( basicBlockBuilder.getModuleBuilder() );
      return adapter;
    }
    
    CG::ExprValue Var::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      RC::ConstHandle<CG::ValueSymbol> valueSymbol = getValueSymbol( basicBlockBuilder );
      CG::ExprValue resultExprValue = valueSymbol->getExprValue();
      if ( resultExprValue.getUsage() == CG::USAGE_RVALUE )
        resultExprValue.getAdapter()->llvmRetain( basicBlockBuilder, resultExprValue.getValue() );
      return resultExprValue.castTo( basicBlockBuilder, usage );
    }
  };
};
