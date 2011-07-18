/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "MemberDecl.h"
#include <Fabric/Core/CG/Location.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( MemberDecl );
    
    MemberDecl::MemberDecl( CG::Location const &location, std::string const &name, std::string const &typeName )
      : Node( location )
      , m_name( name )
      , m_typeName( typeName )
    {
    }
    
    RC::Handle<JSON::Object> MemberDecl::toJSON() const
    {
      RC::Handle<JSON::Object> result = Node::toJSON();
      result->set( "name", JSON::String::Create( m_name ) );
      result->set( "type", JSON::String::Create( m_typeName ) );
      return result;
    }

    RT::StructMemberInfo MemberDecl::getStructMemberInfo( RC::ConstHandle<RT::Manager> const &rtManager ) const
    {
      RT::StructMemberInfo result;
      result.name = m_name;
      result.desc = rtManager->getDesc( m_typeName );
      return result;
    }
  };
};
