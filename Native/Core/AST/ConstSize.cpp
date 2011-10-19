/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/AST/ConstSize.h>
#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Core/CG/Context.h>
#include <Fabric/Core/CG/IntegerAdapter.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/SizeAdapter.h>
#include <Fabric/Core/Util/Parse.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( ConstSize );
    
    RC::ConstHandle<ConstSize> ConstSize::Create( CG::Location const &location, std::string const &valueString )
    {
      return new ConstSize( location, Util::parseSize( valueString ) );
    }
    
    ConstSize::ConstSize( CG::Location const &location, size_t value )
      : Expr( location )
      , m_value( value )
    {
    }
    
    void ConstSize::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const
    {
      Expr::appendJSONMembers( jsonObjectGenerator, includeLocation );
      jsonObjectGenerator.makeMember( "value" ).makeInteger( m_value );
    }
    
    void ConstSize::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      cgManager->getIntegerAdapter();
    }
    
    RC::ConstHandle<CG::Adapter> ConstSize::getType( CG::BasicBlockBuilder &basicBlockBuilder ) const
    {
      RC::ConstHandle<CG::Adapter> adapter = basicBlockBuilder.getManager()->getIntegerAdapter();
      adapter->llvmCompileToModule( basicBlockBuilder.getModuleBuilder() );
      return adapter;
    }
    
    CG::ExprValue ConstSize::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      if ( usage == CG::USAGE_LVALUE )
        throw Exception( "constants cannot be used as l-values" );
      /*
      // [pzion 20110205] Once we have "desired types" we should actually infer
      // the correct type here
      RC::ConstHandle< RT::SizeAdapter > sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();
      return CG::ExprValue( sizeAdapter, CG::USAGE_RVALUE, llvm::ConstantInt::get( sizeAdapter->llvmRType(), m_value, false ) );
      */
      RC::ConstHandle< CG::IntegerAdapter > integerAdapter = basicBlockBuilder.getManager()->getIntegerAdapter();
      integerAdapter->llvmCompileToModule( basicBlockBuilder.getModuleBuilder() );
      return CG::ExprValue( integerAdapter, CG::USAGE_RVALUE, basicBlockBuilder.getContext(), integerAdapter->llvmConst( basicBlockBuilder.getContext(), (int32_t)m_value ) );
    }
  };
};
