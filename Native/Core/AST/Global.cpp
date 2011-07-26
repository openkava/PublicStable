/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "Global.h"

namespace Fabric
{
  namespace AST
  {
    Global::Global( CG::Location const &location )
      : Node( location )
    {
    }
    
    void Global::registerTypes( RC::Handle<RT::Manager> const &rtManager, CG::Diagnostics &diagnostics ) const
    {
    }
    
    void Global::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const
    {
    }
  };
};
