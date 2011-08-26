/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "GlobalConstDecl.h"
#include "ConstDecl.h"
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( GlobalConstDecl );
    
    GlobalConstDecl::GlobalConstDecl(
      CG::Location const &location,
      RC::ConstHandle<ConstDecl> const &constDecl
      )
      : Global( location )
      , m_constDecl( constDecl )
    {
    }
    
    void GlobalConstDecl::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Global::appendJSONMembers( jsonObjectGenerator );
      m_constDecl->appendJSON( jsonObjectGenerator.makeMember( "constDecl" ) );
    }
    
    void GlobalConstDecl::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctions ) const
    {
      m_constDecl->llvmPrepareModule( moduleBuilder, diagnostics, buildFunctions );
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
