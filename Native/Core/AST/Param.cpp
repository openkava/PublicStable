/*
 *
 *  Created by Peter Zion on 10-12-09.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "Param.h"
#include <Fabric/Core/CG/Location.h>
#include <Fabric/Core/CG/Adapter.h>

namespace Fabric
{
  namespace AST
  {
    Param::Param( CG::Location const &location, std::string const &name, RC::ConstHandle< CG::Adapter > const &adapter, CG::Usage usage )
      : Node( location )
      , m_functionParam( name, adapter, usage )
    {
    }

    std::string Param::localDesc() const
    {
      return "Param( " + _(getName()) + ", " + _(getAdapter()) + ", " + std::string( CG::usageDesc( getUsage() ) ) + " )";
    }
  };
};
