/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "GlobalConstDecl.h"
#include "ConstDecl.h"

#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Manager.h>

namespace Fabric
{
  namespace AST
  {
    GlobalConstDecl::GlobalConstDecl(
      CG::Location const &location,
      RC::ConstHandle<ConstDecl> const &constDecl
      )
      : Global( location )
      , m_constDecl( constDecl )
    {
    }
    
    std::string GlobalConstDecl::localDesc() const
    {
      return "GlobalConstDecl";
    }

    std::string GlobalConstDecl::deepDesc( std::string const &indent ) const
    {
      return indent + localDesc() + "\n"
        + m_constDecl->deepDesc( indent + "  " );
    }
    
    void GlobalConstDecl::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const
    {
      if ( !buildFunctionBodies )
      {
        try
        {
          m_constDecl->llvmCompileToScope( moduleBuilder.getScope(), moduleBuilder.getManager() );
        }
        catch ( CG::Error e )
        {
          addError( diagnostics, e );
        }
      }
    }
  };
};
