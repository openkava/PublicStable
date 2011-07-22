/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "GlobalConstDecl.h"
#include "ConstDecl.h"
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Base/JSON/String.h>

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
    
    RC::Handle<JSON::Object> GlobalConstDecl::toJSON() const
    {
      RC::Handle<JSON::Object> result = Global::toJSON();
      result->set( "constDecl", m_constDecl->toJSON() );
      return result;
    }
    
    void GlobalConstDecl::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      m_constDecl->llvmPrepareModule( moduleBuilder, diagnostics );
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
