/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "ConstDeclStatement.h"
#include "ConstDecl.h"
#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( ConstDeclStatement );
    
    RC::ConstHandle<ConstDeclStatement> ConstDeclStatement::Create(
      CG::Location const &location,
      RC::ConstHandle<ConstDecl> const &constDecl
      )
    {
      return new ConstDeclStatement( location, constDecl );
    }

    ConstDeclStatement::ConstDeclStatement(
      CG::Location const &location,
      RC::ConstHandle<ConstDecl> const &constDecl
      )
      : Statement( location )
      , m_constDecl( constDecl )
    {
    }
    
    void ConstDeclStatement::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Node::appendJSONMembers( jsonObjectGenerator );
      m_constDecl->appendJSON( jsonObjectGenerator.makeMember( "constDecl" ) );
    }
    
    void ConstDeclStatement::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      m_constDecl->registerTypes( cgManager, diagnostics );
    }
    
    void ConstDeclStatement::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      try
      {
        m_constDecl->llvmCompileToScope( basicBlockBuilder.getScope(), basicBlockBuilder.getManager() );
      }
      catch ( CG::Error e )
      {
        addError( diagnostics, e );
      }
    }
  };
};
