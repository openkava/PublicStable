/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "ConstDecl.h"
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Location.h>

namespace Fabric
{
  namespace AST
  {
    ConstDecl::ConstDecl(
      CG::Location const &location,
      std::string const &name,
      RC::ConstHandle<CG::Adapter> const &adapter
      )
      : Node( location )
      , m_name( name )
      , m_adapter( adapter )
    {
    }

    RC::ConstHandle<CG::Adapter> ConstDecl::getAdapter() const
    {
      return m_adapter;
    }
  };
};
