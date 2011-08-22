/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "ContinueStatement.h"
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( ContinueStatement );
    
    RC::ConstHandle<ContinueStatement> ContinueStatement::Create( CG::Location const &location )
    {
      return new ContinueStatement( location );
    }
    
    ContinueStatement::ContinueStatement( CG::Location const &location )
      : Statement( location )
    {
    }
    
    void ContinueStatement::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Statement::appendJSONMembers( jsonObjectGenerator );
    }
    
    void ContinueStatement::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
    }

    void ContinueStatement::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      CG::Scope &scope = basicBlockBuilder.getScope();
      scope.llvmContinue( basicBlockBuilder, getLocation() );
    }
  };
};
