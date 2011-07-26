/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "Node.h"
#include <Fabric/Core/CG/Diagnostics.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    Node::Node( CG::Location const &location )
      : m_location( location )
    {
    }
    
    RC::Handle<JSON::Value> Node::toJSON() const
    {
      if ( !m_jsonValue )
        m_jsonValue = toJSONImpl();
      return m_jsonValue;
    }
    
    RC::Handle<JSON::Object> Node::toJSONImpl() const
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( "nodeType", nodeTypeNameJSONString() );
      result->set( "location", m_location.toJSON() );
      return result;
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
