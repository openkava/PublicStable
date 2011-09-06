/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/AST/Use.h>
#include <Fabric/Core/AST/UseInfo.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Use );
    
    RC::ConstHandle<Use> Use::Create(
      CG::Location const &location,
      std::string const &name
      )
    {
      return new Use( location, name );
    }
    
    Use::Use(
      CG::Location const &location,
      std::string const &name
      )
      : Node( location )
      , m_name( name )
    {
    }
    
    void Use::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Node::appendJSONMembers( jsonObjectGenerator );
      jsonObjectGenerator.makeMember( "name" ).makeString( m_name );
    }
    
    void Use::collectUses( UseNameToLocationMap &uses ) const
    {
      uses.insert( UseNameToLocationMap::value_type( m_name, getLocation() ) );
    }
  };
};
