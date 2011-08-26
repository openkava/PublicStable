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
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( ConstScalar );
    
    RC::ConstHandle<ConstScalar> ConstScalar::Create( CG::Location const &location, std::string const &valueString )
    {
      return new ConstScalar( location, valueString );
    }
    
    ConstScalar::ConstScalar( CG::Location const &location, std::string const &valueString )
      : Expr( location )
      , m_valueString( valueString )
    {
    }
    
    void ConstScalar::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Expr::appendJSONMembers( jsonObjectGenerator );
      jsonObjectGenerator.makeMember( "value" ).makeString( m_valueString );
    }
    
    void ConstScalar::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctions ) const
    {
      RC::ConstHandle<CG::FloatAdapter> floatAdapter = moduleBuilder.getManager()->getFP32Adapter();
      floatAdapter->llvmPrepareModule( moduleBuilder, buildFunctions );
    }
    
    RC::ConstHandle<CG::Adapter> ConstScalar::getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      return basicBlockBuilder.getManager()->getFP32Adapter();
    }
    
    CG::ExprValue ConstScalar::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      if ( usage == CG::USAGE_LVALUE )
        throw Exception( "constants cannot be used as l-values" );
      RC::ConstHandle<CG::FloatAdapter> floatAdapter = basicBlockBuilder.getManager()->getFP32Adapter();
      return CG::ExprValue( floatAdapter, CG::USAGE_RVALUE, floatAdapter->llvmConst( m_valueString ) );
    }
  };
};
