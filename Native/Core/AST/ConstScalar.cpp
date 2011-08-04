/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "ConstScalar.h"
#include <Fabric/Core/CG/FloatAdapter.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( ConstScalar );
    
    RC::ConstHandle<ConstScalar> ConstScalar::Create( CG::Location const &location, std::string const &valueString )
    {
      float value;
      if ( sscanf( valueString.c_str(), "%f", &value ) != 1 )
        throw Exception( "invalid floating-point constant '" + valueString + "'" );
      return new ConstScalar( location, value );
    }
    
    ConstScalar::ConstScalar( CG::Location const &location, float value )
      : Expr( location )
      , m_value( value )
    {
    }
    
    void ConstScalar::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Expr::appendJSONMembers( jsonObjectGenerator );
      jsonObjectGenerator.makeMember( "value" ).makeScalar( m_value );
    }
    
    void ConstScalar::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      RC::ConstHandle<CG::FloatAdapter> scalarAdapter = moduleBuilder.getManager()->getScalarAdapter();
      scalarAdapter->llvmPrepareModule( moduleBuilder, true );
    }
    
    RC::ConstHandle<CG::Adapter> ConstScalar::getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      return basicBlockBuilder.getManager()->getScalarAdapter();
    }
    
    CG::ExprValue ConstScalar::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      if ( usage == CG::USAGE_LVALUE )
        throw Exception( "constants cannot be used as l-values" );
      RC::ConstHandle<CG::FloatAdapter> scalarAdapter = basicBlockBuilder.getManager()->getScalarAdapter();
      return CG::ExprValue( scalarAdapter, CG::USAGE_RVALUE, scalarAdapter->llvmConst( m_value ) );
    }
  };
};
