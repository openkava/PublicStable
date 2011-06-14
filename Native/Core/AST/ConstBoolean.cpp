/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "ConstBoolean.h"
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/BooleanAdapter.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/BasicBlockBuilder.h>

namespace Fabric
{
  namespace AST
  {
    ConstBoolean::ConstBoolean( CG::Location const &location, bool value )
      : Expr( location )
      , m_value( value )
    {
    }
    
    std::string ConstBoolean::localDesc() const
    {
      return "ConstBoolean( " + _(m_value) + " )";
    }
    
    RC::ConstHandle<CG::Adapter> ConstBoolean::getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      return basicBlockBuilder.getManager()->getBooleanAdapter();
    }
    
    CG::ExprValue ConstBoolean::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      if ( usage == CG::USAGE_LVALUE )
        throw Exception( "constants cannot be used as l-values" );
      RC::ConstHandle<CG::BooleanAdapter> booleanAdapter = basicBlockBuilder.getManager()->getBooleanAdapter();
      return CG::ExprValue( booleanAdapter, CG::USAGE_RVALUE, booleanAdapter->llvmConst( m_value ) );
    }
  };
};
