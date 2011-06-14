/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "Node.h"
#include <Fabric/Core/CG/Diagnostics.h>
#include <Fabric/Core/CG/Error.h>

namespace Fabric
{
  namespace AST
  {
    Node::Node( CG::Location const &location )
      : m_location( location )
    {
    }
    
    std::string Node::deepDesc( std::string const &indent ) const
    {
      return indent + localDesc() + "\n";
    }

    void Node::addWarning( CG::Diagnostics &diagnostics, std::string const &desc ) const
    {
      diagnostics.addWarning( m_location, desc );
    }
    
    void Node::addError( CG::Diagnostics &diagnostics, std::string const &desc ) const
    {
	 		diagnostics.addError( m_location, desc );
    }
    
    void Node::addError( CG::Diagnostics &diagnostics, CG::Error const &error ) const
    {
      diagnostics.addError( error.getLocation(), error.getDesc() );
    }
  };
};
