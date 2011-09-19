/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/AST/VarDeclStatement.h>
#include <Fabric/Core/AST/VarDecl.h>
#include <Fabric/Core/AST/VarDeclVector.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( VarDeclStatement );
    
    RC::ConstHandle<VarDeclStatement> VarDeclStatement::Create(
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
    
    void VarDeclStatement::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const
    {
      Statement::appendJSONMembers( jsonObjectGenerator, includeLocation );
      jsonObjectGenerator.makeMember( "baseType" ).makeString( m_baseType );
      m_varDecls->appendJSON( jsonObjectGenerator.makeMember( "varDecls" ), includeLocation );
    }
    
    void VarDeclStatement::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      m_varDecls->registerTypes( m_baseType, cgManager, diagnostics );
    }

    void VarDeclStatement::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      m_varDecls->llvmCompileToBuilder( m_baseType, basicBlockBuilder, diagnostics );
    }
  };
};
