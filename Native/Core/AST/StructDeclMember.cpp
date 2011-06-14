/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "StructDeclMember.h"
#include <Fabric/Core/CG/Location.h>
#include <Fabric/Core/CG/Adapter.h>

namespace Fabric
{
  namespace AST
  {
    StructDeclMember::StructDeclMember( CG::Location const &location, std::string const &name, RC::ConstHandle<CG::Adapter> const &adapter )
      : Node( location )
      , m_name( name )
      , m_adapter( adapter )
    {
    }

    std::string StructDeclMember::localDesc() const
    {
      return "StructDeclMember( " + _(m_name) + ", " + _(m_adapter) + " )";
    }

    RT::StructMemberInfo StructDeclMember::getStructMemberInfo() const
    {
      RT::StructMemberInfo result;
      result.name = m_name;
      result.desc = m_adapter->getDesc();
      return result;
    }
  };
};
