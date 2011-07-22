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
    class MemberDeclVector;
    
    class StructDecl : public Global
    {
      FABRIC_AST_NODE_DECL( StructDecl );

    public:

      static RC::ConstHandle<StructDecl> Create(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<MemberDeclVector> const &members
        );

      RC::Handle<JSON::Object> toJSON() const;

      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
      
      virtual void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const;
      
    protected:
    
      StructDecl(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<MemberDeclVector> const &members
        );
    
    private:
    
      std::string m_name;
      RC::ConstHandle<MemberDeclVector> m_members;
    };
  };
};

#endif //_FABRIC_AST_STRUCT_DECL_H
