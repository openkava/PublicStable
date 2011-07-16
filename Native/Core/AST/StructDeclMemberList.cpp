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
      : Node( location )
    {
    }
    
    void StructDeclMemberList::append( RC::ConstHandle<StructDeclMember> const &structDeclMember )
    {
      m_items.push_back( structDeclMember );
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
    
    size_t StructDeclMemberList::numItems() const
    {
      return m_items.size();
    }
    
    RC::ConstHandle<StructDeclMember> StructDeclMemberList::item( size_t index ) const
    {
      return m_items[index];
    }

    void StructDeclMemberList::appenedToStructMemberInfoVector( RT::StructMemberInfoVector &structMemberInfoVector, RC::ConstHandle<RT::Manager> const &rtManager )
    {
      size_t count = numItems();
      for ( size_t i=0; i<count; ++i )
        structMemberInfoVector.push_back( item(i)->getStructMemberInfo( rtManager ) );
    }
  };
};
