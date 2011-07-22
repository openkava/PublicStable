/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_GLOBAL_CONST_DECL_H
#define _FABRIC_AST_GLOBAL_CONST_DECL_H

#include <Fabric/Core/AST/Global.h>

namespace Fabric
{
  namespace AST
  {
    class ConstDecl;
    
    class GlobalConstDecl : public Global
    {
      FABRIC_AST_NODE_DECL( GlobalConstDecl );

    public:

      static RC::Handle<GlobalConstDecl> Create(
        CG::Location const &location,
        RC::ConstHandle<ConstDecl> const &constDecl
        )
      {
        return new GlobalConstDecl( location, constDecl );
      }

      RC::Handle<JSON::Object> toJSON() const;
      
      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
      
      virtual void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const;
      
    protected:
    
      GlobalConstDecl(
        CG::Location const &location,
        RC::ConstHandle<ConstDecl> const &constDecl
        );
    
    private:
    
      RC::ConstHandle<ConstDecl> m_constDecl;
    };
  };
};

#endif //_FABRIC_AST_GLOBAL_CONST_DECL_H
