/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "ConstSize.h"
#include <Fabric/Core/CG/IntegerAdapter.h>
#include <Fabric/Core/CG/SizeAdapter.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/BasicBlockBuilder.h>

namespace Fabric
{
  namespace AST
  {
    RC::Handle<ConstSize> ConstSize::Create( CG::Location const &location, std::string const &valueString )
    {
      unsigned long value;
      if ( sscanf( valueString.c_str(), "%lu", &value ) != 1 )
        throw Exception( "invalid integer constant '" + valueString + "'" );
      return new ConstSize( location, value );
    }
    
    ConstSize::ConstSize( CG::Location const &location, size_t value )
      : Expr( location )
      , m_value( value )
    {
    }
    
    std::string ConstSize::localDesc() const
    {
      char buf[1024];
      snprintf( buf, 1024, "ConstSize( %u )", (unsigned)m_value );
      return std::string(buf);
    }
    
    RC::ConstHandle<CG::Adapter> ConstSize::getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      return basicBlockBuilder.getManager()->getIntegerAdapter();
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
      return CG::ExprValue( integerAdapter, CG::USAGE_RVALUE, integerAdapter->llvmConst( (int32_t)m_value ) );
    }
  };
};
