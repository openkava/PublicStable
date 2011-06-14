/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_LIST_NODE_H
#define _FABRIC_AST_LIST_NODE_H

#include <Fabric/Core/AST/Node.h>
#include <Fabric/Base/RC/ConstHandle.h>

#include <vector>

namespace Fabric
{
  namespace AST
  {
    class ListNode: public Node
    {
    public:
    
      size_t numItems() const
      {
        return m_items.size();
      }
      
      virtual std::string deepDesc( std::string const &indent ) const;

    protected:
    
      ListNode( CG::Location const &location );
      ListNode( CG::Location const &location, RC::ConstHandle<Node> const &item );
      ListNode( CG::Location const &location, RC::ConstHandle<Node> const &item, RC::ConstHandle<ListNode> const &remaining );
      
      RC::ConstHandle<Node> item( size_t index ) const
      {
        if ( index >= m_items.size() )
          throw Exception("index out of range");
        return m_items[index];
      }
    
    private:
    
      std::vector< RC::ConstHandle<Node> > m_items;
    };
  };
};

#endif //_FABRIC_AST_LIST_NODE_H
