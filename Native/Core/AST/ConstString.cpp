/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "ConstString.h"
#include <Fabric/Core/CG/ConstStringAdapter.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Core/Util/Parse.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( ConstString );
    
    RC::Handle<ConstString> ConstString::Create( CG::Location const &location, std::string const &value, bool quoted )
    {
      if ( quoted )
        return new ConstString( location, Util::parseQuotedString( value ) );
      else return new ConstString( location, value );
    }

    ConstString::ConstString( CG::Location const &location, std::string const &value )
      : Expr( location )
      , m_value( value )
    {
    }
    
    RC::Handle<JSON::Object> ConstString::toJSON() const
    {
      RC::Handle<JSON::Object> result = Expr::toJSON();
      result->set( "value", JSON::String::Create( m_value ) );
      return result;
    }
    
    RC::ConstHandle<CG::ConstStringAdapter> ConstString::getAdapter( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      return basicBlockBuilder.getManager()->getConstStringAdapter( m_value.length() );
    }
    
    RC::ConstHandle<CG::Adapter> ConstString::getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      return getAdapter( basicBlockBuilder );
    }
    
    CG::ExprValue ConstString::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      if ( usage == CG::USAGE_LVALUE )
        throw Exception( "constants cannot be used as l-values" );
      RC::ConstHandle<CG::ConstStringAdapter> constStringAdapter = getAdapter( basicBlockBuilder );
      return CG::ExprValue( constStringAdapter, CG::USAGE_RVALUE, constStringAdapter->llvmConst( basicBlockBuilder, m_value.data(), m_value.length() ) );
    }
  };
};
