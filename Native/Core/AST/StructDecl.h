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
    class StructDeclMemberVector;
    
    class StructDecl : public Global
    {
      FABRIC_AST_NODE_DECL( StructDecl );

    public:

      static RC::Handle<StructDecl> Create(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<StructDeclMemberVector> const &members
        );

      RC::Handle<JSON::Object> toJSON() const;
      
      virtual void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const;
      
    protected:
    
      StructDecl(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<StructDeclMemberVector> const &members
        );
    
    private:
    
      std::string m_name;
      RC::ConstHandle<StructDeclMemberVector> m_members;
    };
  };
};

#endif //_FABRIC_AST_STRUCT_DECL_H
