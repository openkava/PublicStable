/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "StructDeclMember.h"
#include <Fabric/Core/CG/Location.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/RT/Manager.h>

namespace Fabric
{
  namespace AST
  {
    StructDeclMember::StructDeclMember( CG::Location const &location, std::string const &name, std::string const &typeName )
      : Node( location )
      , m_name( name )
      , m_typeName( typeName )
    {
    }

    std::string StructDeclMember::localDesc() const
    {
      return "StructDeclMember( " + _(m_name) + ", " + _(m_typeName) + " )";
    }

    RT::StructMemberInfo StructDeclMember::getStructMemberInfo( RC::ConstHandle<RT::Manager> const &rtManager ) const
    {
      RT::StructMemberInfo result;
      result.name = m_name;
      result.desc = rtManager->getDesc( m_typeName );
      return result;
    }
  };
};
