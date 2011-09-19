/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "Node.h"
#include <Fabric/Core/CG/Diagnostics.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    Node::Node( CG::Location const &location )
      : m_location( location )
    {
    }
    
    void Node::appendJSON( Util::JSONGenerator const &jsonGenerator, bool includeLocation ) const
    {
      appendJSONMembers( jsonGenerator.makeObject(), includeLocation );
    }
    
    void Node::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const
    {
      jsonObjectGenerator.makeMember( "+" ).makeString( nodeTypeName() );
      if ( includeLocation )
        m_location.appendJSON( jsonObjectGenerator.makeMember( "-" ), includeLocation );
    }

    void Node::addWarning( CG::Diagnostics &diagnostics, Util::SimpleString const &desc ) const
    {
      diagnostics.addWarning( m_location, desc );
    }
    
    void Node::addError( CG::Diagnostics &diagnostics, Util::SimpleString const &desc ) const
    {
	 		diagnostics.addError( m_location, desc );
    }
    
    void Node::addError( CG::Diagnostics &diagnostics, CG::Error const &error ) const
    {
      diagnostics.addError( error.getLocation(), error.getDesc() );
    }
  };
};
