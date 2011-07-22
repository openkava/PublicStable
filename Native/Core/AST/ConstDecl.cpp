/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "ConstDecl.h"
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/ByteAdapter.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/ExprValue.h>
#include <Fabric/Core/CG/IntegerAdapter.h>
#include <Fabric/Core/CG/Location.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/ScalarAdapter.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/SizeAdapter.h>
#include <Fabric/Core/Util/Parse.h>
#include <Fabric/Base/JSON/String.h>

#include <llvm/Constant.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( ConstDecl );

    RC::Handle<ConstDecl> ConstDecl::Create(
      CG::Location const &location,
      std::string const &name,
      std::string const &type,
      std::string const &value
      )
    {
      return new ConstDecl( location, name, type, value );
    }
    
    ConstDecl::ConstDecl(
      CG::Location const &location,
      std::string const &name,
      std::string const &type,
      std::string const &value
      )
      : Node( location )
      , m_name( name )
      , m_type( type )
      , m_value( value )
    {
    }
    
    RC::Handle<JSON::Object> ConstDecl::toJSON() const
    {
      RC::Handle<JSON::Object> result = Node::toJSON();
      result->set( "name", JSON::String::Create( m_name ) );
      result->set( "type", JSON::String::Create( m_type ) );
      result->set( "value", JSON::String::Create( m_value ) );
      return result;
    }
    
    void ConstDecl::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      RC::ConstHandle<CG::Adapter> adapter = moduleBuilder.getAdapter( m_type, getLocation() );
      adapter->llvmPrepareModule( moduleBuilder, true );
    }

    void ConstDecl::llvmCompileToScope( CG::Scope &scope, RC::ConstHandle<CG::Manager> const &manager ) const
    {
      RC::ConstHandle<CG::Adapter> adapter = manager->getAdapter( m_type );
      
      RC::ConstHandle<CG::ByteAdapter> byteAdapter = manager->getByteAdapter();
      RC::ConstHandle<CG::IntegerAdapter> integerAdapter = manager->getIntegerAdapter();
      RC::ConstHandle<CG::SizeAdapter> sizeAdapter = manager->getSizeAdapter();
      RC::ConstHandle<CG::ScalarAdapter> scalarAdapter = manager->getScalarAdapter();
      
      CG::ExprValue exprValue;
      if ( adapter == byteAdapter )
        exprValue = CG::ExprValue( byteAdapter, CG::USAGE_RVALUE, byteAdapter->llvmConst( Util::parseSize( m_value ) ) );
      else if ( adapter == integerAdapter )
        exprValue = CG::ExprValue( integerAdapter, CG::USAGE_RVALUE, integerAdapter->llvmConst( Util::parseSize( m_value ) ) );
      else if ( adapter == sizeAdapter )
        exprValue = CG::ExprValue( sizeAdapter, CG::USAGE_RVALUE, sizeAdapter->llvmConst( Util::parseSize( m_value ) ) );
      else if ( adapter == scalarAdapter )
        exprValue = CG::ExprValue( scalarAdapter, CG::USAGE_RVALUE, scalarAdapter->llvmConst( Util::parseFloat( m_value ) ) );
      else throw CG::Error( getLocation(), "constant declaration type must be Byte, Integer, Size or Scalar" );
        
      if ( scope.has( m_name ) )
        throw CG::Error( getLocation(), "symbol " + _(m_name) + " already exists" );
      else scope.put( m_name, CG::VariableSymbol::Create( exprValue ) );
    }
  };
};
