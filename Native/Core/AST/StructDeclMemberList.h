/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_STRUCT_DECL_MEMBER_LIST_NODE_H
#define _FABRIC_AST_STRUCT_DECL_MEMBER_LIST_NODE_H

#include <Fabric/Core/AST/ListNode.h>
#include <Fabric/Core/RT/StructMemberInfo.h>

namespace Fabric
{
  namespace AST
  {
    class StructDeclMember;
    
    class StructDeclMemberList: public ListNode
    {
    public:
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;

      static RC::Handle<StructDeclMemberList> Create( CG::Location const &location )
      {
        return new StructDeclMemberList( location );
      }
      
      static RC::Handle<StructDeclMemberList> Create( CG::Location const &location, RC::ConstHandle<StructDeclMember> const &structDeclMember )
      {
        return new StructDeclMemberList( location, structDeclMember );
      }
      
      static RC::Handle<StructDeclMemberList> Create( CG::Location const &location, RC::ConstHandle<StructDeclMember> const &structDeclMember, RC::ConstHandle<StructDeclMemberList> const &remaining )
      {
        return new StructDeclMemberList( location, structDeclMember, remaining );
      }
      
      RC::ConstHandle<StructDeclMember> item( size_t index ) const;
      
      void appenedToStructMemberInfoVector( RT::StructMemberInfoVector &structMemberInfoVector );
      
    protected:
    
      StructDeclMemberList( CG::Location const &location );
      StructDeclMemberList( CG::Location const &location, RC::ConstHandle<StructDeclMember> const &structDeclMember );
      StructDeclMemberList( CG::Location const &location, RC::ConstHandle<StructDeclMember> const &structDeclMember, RC::ConstHandle<StructDeclMemberList> const &next );
    };
  };
};

#endif //_FABRIC_AST_STRUCT_DECL_MEMBER_LIST_NODE_H
