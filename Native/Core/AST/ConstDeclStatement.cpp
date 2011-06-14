/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "ConstDeclStatement.h"
#include "ConstDecl.h"

#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Manager.h>

namespace Fabric
{
  namespace AST
  {
    ConstDeclStatement::ConstDeclStatement(
      CG::Location const &location,
      RC::ConstHandle<ConstDecl> const &constDecl
      )
      : Statement( location )
      , m_constDecl( constDecl )
    {
    }
    
    std::string ConstDeclStatement::localDesc() const
    {
      return "ConstDeclStatement";
    }

    std::string ConstDeclStatement::deepDesc( std::string const &indent ) const
    {
      return indent + localDesc() + "\n"
        + m_constDecl->deepDesc( indent + "  " );
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
