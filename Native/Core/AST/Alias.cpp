/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "Alias.h"

namespace Fabric
{
  namespace AST
  {
    Alias::Alias(
      CG::Location const &location,
      std::string const &name,
      RC::ConstHandle<CG::Adapter> const &adapter
      )
      : Global( location )
      , m_name( name )
      , m_adapter( adapter )
    {
    }
    
    std::string Alias::localDesc() const
    {
      return "Alias( '" + m_name + "', " + m_adapter->getUserName() + " )";
    }

    void Alias::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const
    {
    }
  };
};
