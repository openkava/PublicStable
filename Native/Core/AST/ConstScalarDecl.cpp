/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "ConstScalarDecl.h"
#include <Fabric/Core/CG/ScalarAdapter.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/Error.h>

namespace Fabric
{
  namespace AST
  {
    ConstScalarDecl::ConstScalarDecl(
      CG::Location const &location,
      std::string const &name,
      RC::ConstHandle<CG::Adapter> const &adapter,
      std::string const &scalarString
      )
      : ConstDecl( location, name, adapter )
    {
      if ( sscanf( scalarString.c_str(), "%f", &m_scalar ) != 1 )
        throw Exception( "invalid floating-point constant "+_(scalarString) );
    }
    
    std::string ConstScalarDecl::localDesc() const
    {
      return "ConstScalarDecl( "+_(getName())+", "+getAdapter()->getUserName()+", "+_(m_scalar)+" )";
    }

    void ConstScalarDecl::llvmCompileToScope( CG::Scope &scope, RC::ConstHandle<CG::Manager> const &manager ) const
    {
      RC::ConstHandle<CG::ScalarAdapter> scalarAdapter = manager->getScalarAdapter();
      if ( getAdapter() == scalarAdapter )
      {
        CG::ExprValue exprValue( scalarAdapter, CG::USAGE_RVALUE, scalarAdapter->llvmConst( m_scalar ) );
        
        std::string const &name = getName();
        if ( scope.has( name ) )
          throw CG::Error( getLocation(), "symbol " + _(name) + " already exists" );
        else scope.put( name, CG::VariableSymbol::Create( exprValue ) );
      }
      else throw CG::Error( getLocation(), "constant declaration type must be Scalar" );
    }
  };
};
