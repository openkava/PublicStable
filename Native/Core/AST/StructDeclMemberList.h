/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_STRUCT_DECL_MEMBER_LIST_NODE_H
#define _FABRIC_AST_STRUCT_DECL_MEMBER_LIST_NODE_H

#include <Fabric/Core/AST/Node.h>
#include <Fabric/Core/RT/StructMemberInfo.h>

namespace Fabric
{
  namespace RT
  {
    class Manager;
  };
  
  namespace AST
  {
    class StructDeclMember;
    
    class StructDeclMemberList: public Node
    {
    public:
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;

      static RC::Handle<StructDeclMemberList> Create( CG::Location const &location )
      {
        return new StructDeclMemberList( location );
      }
      
      void append( RC::ConstHandle<StructDeclMember> const &structDeclMember );
      
      size_t numItems() const;
      RC::ConstHandle<StructDeclMember> item( size_t index ) const;
      
      void appenedToStructMemberInfoVector( RT::StructMemberInfoVector &structMemberInfoVector, RC::ConstHandle<RT::Manager> const &rtManager );
      
    protected:
    
      StructDeclMemberList( CG::Location const &location );
      
    private:
    
      std::vector< RC::ConstHandle<StructDeclMember> > m_items;
    };
  };
};

#endif //_FABRIC_AST_STRUCT_DECL_MEMBER_LIST_NODE_H
