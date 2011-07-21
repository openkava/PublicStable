/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "ContinueStatement.h"
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( ContinueStatement );
    
    ContinueStatement::ContinueStatement( CG::Location const &location )
      : Statement( location )
    {
    }

    void ContinueStatement::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      CG::Scope &scope = basicBlockBuilder.getScope();
      scope.llvmContinue( basicBlockBuilder, getLocation() );
    }
  };
};
