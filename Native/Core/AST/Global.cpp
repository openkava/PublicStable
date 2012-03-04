/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "Global.h"

namespace Fabric
{
  namespace AST
  {
    Global::Global( CG::Location const &location )
      : Node( location )
    {
    }

    void Global::appendJSON( JSON::Encoder const &encoder, bool includeLocation ) const
    {
      if ( m_json.getLength() == 0 )
        Node::appendJSON( JSON::Encoder( &m_json ), includeLocation );
      encoder.appendJSON( m_json );
    }
    
    void Global::collectUses( UseNameToLocationMap &uses ) const
    {
    }
  };
};
