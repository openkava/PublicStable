/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "StructDeclMemberList.h"
#include "StructDeclMember.h"

namespace Fabric
{
  namespace AST
  {
    StructDeclMemberList::StructDeclMemberList( CG::Location const &location )
      : ListNode( location )
    {
    }
    
    StructDeclMemberList::StructDeclMemberList( CG::Location const &location, RC::ConstHandle<StructDeclMember> const &structDeclMember )
      : ListNode( location, structDeclMember )
    {
    }
    
    StructDeclMemberList::StructDeclMemberList( CG::Location const &location, RC::ConstHandle<StructDeclMember> const &structDeclMember, RC::ConstHandle<StructDeclMemberList> const &remaining )
      : ListNode( location, structDeclMember, remaining )
    {
    }
    
    std::string StructDeclMemberList::localDesc() const
    {
      return "StructDeclMemberList";
    }
    
    std::string StructDeclMemberList::deepDesc( std::string const &indent ) const
    {
      size_t count = numItems();
      std::string result = "";
      for ( size_t i=0; i<count; ++i )
        result += item(i)->deepDesc( indent );
      return result;
    }
          
    RC::ConstHandle<StructDeclMember> StructDeclMemberList::item( size_t index ) const
    {
      return RC::ConstHandle<StructDeclMember>::StaticCast( ListNode::item(index) );
    }

    void StructDeclMemberList::appenedToStructMemberInfoVector( RT::StructMemberInfoVector &structMemberInfoVector )
    {
      size_t count = numItems();
      for ( size_t i=0; i<count; ++i )
        structMemberInfoVector.push_back( item(i)->getStructMemberInfo() );
    }
  };
};
