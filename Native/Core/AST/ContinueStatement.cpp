/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "ContinueStatement.h"
#include <Fabric/Core/CG/Scope.h>

namespace Fabric
{
  namespace AST
  {
    ContinueStatement::ContinueStatement( CG::Location const &location )
      : Statement( location )
    {
    }
    
    std::string ContinueStatement::localDesc() const
    {
      return "ContinueStatement";
    }
    
    std::string ContinueStatement::deepDesc( std::string const &indent ) const
    {
      return indent + localDesc() + "\n";
    }

    void ContinueStatement::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      CG::Scope &scope = basicBlockBuilder.getScope();
      scope.llvmContinue( basicBlockBuilder, getLocation() );
    }
  };
};
