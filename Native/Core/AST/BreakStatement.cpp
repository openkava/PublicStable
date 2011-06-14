/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/BreakStatement.h>
#include <Fabric/Core/CG/Scope.h>

namespace Fabric
{
  namespace AST
  {
    BreakStatement::BreakStatement( CG::Location const &location )
      : Statement( location )
    {
    }
    
    std::string BreakStatement::localDesc() const
    {
      return "BreakStatement";
    }
    
    std::string BreakStatement::deepDesc( std::string const &indent ) const
    {
      return indent + localDesc() + "\n";
    }

    void BreakStatement::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      CG::Scope &scope = basicBlockBuilder.getScope();
      scope.llvmBreak( basicBlockBuilder, getLocation() );
    }
  };
};
