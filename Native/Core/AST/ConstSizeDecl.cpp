/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "ConstSizeDecl.h"
#include <Fabric/Core/CG/ByteAdapter.h>
#include <Fabric/Core/CG/IntegerAdapter.h>
#include <Fabric/Core/CG/SizeAdapter.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/Util/Parse.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( ConstSizeDecl );
    
    ConstSizeDecl::ConstSizeDecl(
      CG::Location const &location,
      std::string const &name,
      RC::ConstHandle<CG::Adapter> const &adapter,
      std::string const &sizeString
      )
      : ConstDecl( location, name, adapter )
      , m_size( Util::parseSize( sizeString ) )
    {
    }
    
    RC::Handle<JSON::Object> ConstSizeDecl::toJSON() const
    {
      RC::Handle<JSON::Object> result = ConstDecl::toJSON();
      result->set( "value", JSON::Integer::Create( m_size ) );
      return result;
    }

    void ConstSizeDecl::llvmCompileToScope( CG::Scope &scope, RC::ConstHandle<CG::Manager> const &manager ) const
    {
      RC::ConstHandle<CG::ByteAdapter> byteAdapter = manager->getByteAdapter();
      RC::ConstHandle<CG::IntegerAdapter> integerAdapter = manager->getIntegerAdapter();
      RC::ConstHandle<CG::SizeAdapter> sizeAdapter = manager->getSizeAdapter();
      
      CG::ExprValue exprValue;
      if ( getAdapter() == byteAdapter )
        exprValue = CG::ExprValue( byteAdapter, CG::USAGE_RVALUE, byteAdapter->llvmConst( m_size ) );
      else if ( getAdapter() == integerAdapter )
        exprValue = CG::ExprValue( integerAdapter, CG::USAGE_RVALUE, integerAdapter->llvmConst( m_size ) );
      else if ( getAdapter() == sizeAdapter )
        exprValue = CG::ExprValue( sizeAdapter, CG::USAGE_RVALUE, sizeAdapter->llvmConst( m_size ) );
      else throw CG::Error( getLocation(), "constant declaration type must be Byte, Integer or Scalar" );
        
      std::string const &name = getName();
      if ( scope.has( name ) )
        throw CG::Error( getLocation(), "symbol " + _(name) + " already exists" );
      else scope.put( name, CG::VariableSymbol::Create( exprValue ) );
    }
  };
};
