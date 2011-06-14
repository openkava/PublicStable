/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_STRUCT_DECL_MEMBER_H
#define _FABRIC_AST_STRUCT_DECL_MEMBER_H

#include <Fabric/Core/AST/Node.h>
#include <Fabric/Core/RT/StructMemberInfo.h>

namespace Fabric
{
  namespace CG
  {
    class Adapter;
  };
  
  namespace AST
  {
    class StructDeclMember: public Node
    {
    public:
    
      static RC::Handle<StructDeclMember> Create( CG::Location const &location, std::string const &name, RC::ConstHandle<CG::Adapter> const &adapter )
      {
        return new StructDeclMember( location, name, adapter );
      }
    
      virtual std::string localDesc() const;
      
      RT::StructMemberInfo getStructMemberInfo() const;

    protected:
    
      StructDeclMember( CG::Location const &location, std::string const &name, RC::ConstHandle<CG::Adapter> const &adapter );
      
    private:
    
      std::string m_name;
      RC::ConstHandle<CG::Adapter> m_adapter;
    };
  };
};

#endif //_FABRIC_AST_STRUCT_DECL_MEMBER_H
