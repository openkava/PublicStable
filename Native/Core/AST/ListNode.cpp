/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "ListNode.h"

namespace Fabric
{
  namespace AST
  {
    ListNode::ListNode( CG::Location const &location )
      : Node( location )
    {
    }
    
    ListNode::ListNode( CG::Location const &location, RC::ConstHandle<Node> const &item )
      : Node( location )
    {
      m_items.push_back( item );
    }
    
    ListNode::ListNode( CG::Location const &location, RC::ConstHandle<Node> const &item, RC::ConstHandle<ListNode> const &remaining )
      : Node( location )
    {
      m_items.push_back( item );
      for ( size_t i=0; i<remaining->numItems(); ++i )
        m_items.push_back( remaining->item(i) );
    }
    
    std::string ListNode::deepDesc( std::string const &indent ) const
    {
      std::string subIndent = indent + "  ";
      std::string result = indent + localDesc() + "\n";
      for ( size_t i=0; i<numItems(); ++i )
        result += item(i)->deepDesc( subIndent );
      return result;
    }
  };
};
