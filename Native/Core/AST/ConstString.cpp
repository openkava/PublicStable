/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "ConstString.h"
#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Core/CG/ConstStringAdapter.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/StringAdapter.h>
#include <Fabric/Core/Util/Parse.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( ConstString );
    
    RC::ConstHandle<ConstString> ConstString::Create( CG::Location const &location, std::string const &value )
    {
      return new ConstString( location, value );
    }

    ConstString::ConstString( CG::Location const &location, std::string const &value )
      : Expr( location )
      , m_value( value )
    {
    }
    
    void ConstString::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Expr::appendJSONMembers( jsonObjectGenerator );
      jsonObjectGenerator.makeMember( "value" ).makeString( m_value );
    }
    
    void ConstString::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      std::string unquotedValue;
      try
      {
        unquotedValue = Util::parseQuotedString( m_value );
      }
      catch ( Exception e )
      {
        throw CG::Error( getLocation(), e.getDesc() + "(" + m_value + ")" );
      }

      RC::ConstHandle<CG::ConstStringAdapter> constStringAdapter = moduleBuilder.getManager()->getConstStringAdapter( unquotedValue.length() );
      constStringAdapter->llvmPrepareModule( moduleBuilder, true );

      RC::ConstHandle<CG::StringAdapter> stringAdapter = moduleBuilder.getManager()->getStringAdapter();
      stringAdapter->llvmPrepareModule( moduleBuilder, true );
    }
    
    RC::ConstHandle<CG::Adapter> ConstString::getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      return basicBlockBuilder.getManager()->getStringAdapter();
    }
    
    CG::ExprValue ConstString::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      if ( usage == CG::USAGE_LVALUE )
        throw Exception( "constants cannot be used as l-values" );
      std::string unquotedValue;
      try
      {
        unquotedValue = Util::parseQuotedString( m_value );
      }
      catch ( Exception e )
      {
        throw CG::Error( getLocation(), e.getDesc() + "(" + m_value + ")" );
      }
      RC::ConstHandle<CG::ConstStringAdapter> constStringAdapter = basicBlockBuilder.getManager()->getConstStringAdapter( unquotedValue.length() );
      RC::ConstHandle<CG::StringAdapter> stringAdapter = basicBlockBuilder.getManager()->getStringAdapter();
      return CG::ExprValue( constStringAdapter, CG::USAGE_RVALUE, basicBlockBuilder.getContext(), constStringAdapter->llvmConst( basicBlockBuilder, unquotedValue.data(), unquotedValue.length() ) ).castTo( basicBlockBuilder, stringAdapter );
    }
  };
};
