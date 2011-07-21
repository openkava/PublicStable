/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/VarDeclStatement.h>
#include <Fabric/Core/AST/VarDecl.h>
#include <Fabric/Core/AST/VarDeclVector.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( VarDeclStatement );
    
    RC::Handle<VarDeclStatement> VarDeclStatement::Create(
      CG::Location const &location,
      std::string const &baseType,
      RC::ConstHandle<VarDeclVector> const &varDecls
      )
    {
      return new VarDeclStatement( location, baseType, varDecls );
    }
    
    VarDeclStatement::VarDeclStatement(
      CG::Location const &location,
      std::string const &baseType,
      RC::ConstHandle<VarDeclVector> const &varDecls
      )
      : Statement( location )
      , m_baseType( baseType )
      , m_varDecls( varDecls )
    {
    }
    
    RC::Handle<JSON::Object> VarDeclStatement::toJSON() const
    {
      RC::Handle<JSON::Object> result = Statement::toJSON();
      result->set( "baseType", JSON::String::Create( m_baseType ) );
      result->set( "varDecls", m_varDecls->toJSON() );
      return result;
    }

    void VarDeclStatement::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      for ( VarDeclVector::const_iterator it=m_varDecls->begin(); it!=m_varDecls->end(); ++it )
        (*it)->llvmCompileToBuilder( m_baseType, basicBlockBuilder, diagnostics );
    }
  };
};
