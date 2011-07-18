/*
 *
 *  Created by Peter Zion on 10-12-09.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "Param.h"
#include <Fabric/Core/CG/Location.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Param );
    
    Param::Param( CG::Location const &location, std::string const &name, RC::ConstHandle< CG::Adapter > const &adapter, CG::Usage usage )
      : Node( location )
      , m_functionParam( name, adapter, usage )
    {
    }
    
    RC::Handle<JSON::Object> Param::toJSON() const
    {
      RC::Handle<JSON::Object> result = Node::toJSON();
      result->set( "name", JSON::String::Create( getName() ) );
      result->set( "type", JSON::String::Create( getAdapter()->getUserName() ) );
      result->set( "usage", JSON::String::Create( CG::usageDesc( getUsage() ) ) );
      return result;
    }
  };
};
