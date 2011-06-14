/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_STRUCT_DECL_H
#define _FABRIC_AST_STRUCT_DECL_H

#include <Fabric/Core/AST/Global.h>

namespace Fabric
{
  namespace CG
  {
    class Adapter;
  };
  
  namespace AST
  {
    class StructDeclMemberList;
    
    class StructDecl : public Global
    {
    public:

      static RC::Handle<StructDecl> Create(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<StructDeclMemberList> const &structDeclMemberList
        )
      {
        return new StructDecl( location, name, structDeclMemberList );
      }
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;
      
      virtual void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const;
      
    protected:
    
      StructDecl(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<StructDeclMemberList> const &structDeclMemberList
        );
    
    private:
    
      std::string m_name;
      RC::ConstHandle<StructDeclMemberList> m_structDeclMemberList;
    };
  };
};

#endif //_FABRIC_AST_STRUCT_DECL_H
