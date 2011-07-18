/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_AST_CONST_SCALAR_DECL_H
#define _FABRIC_AST_CONST_SCALAR_DECL_H

#include <Fabric/Core/AST/ConstDecl.h>

namespace Fabric
{
  namespace AST
  {
    class ConstScalarDecl: public ConstDecl
    {
      FABRIC_AST_NODE_DECL( ConstScalarDecl );

    public:

      static RC::Handle<ConstScalarDecl> Create(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<CG::Adapter> const &adapter,
        std::string const &scalarString
        )
      {
        return new ConstScalarDecl( location, name, adapter, scalarString );
      }

      RC::Handle<JSON::Object> toJSON() const;
      
      virtual void llvmCompileToScope( CG::Scope &scope, RC::ConstHandle<CG::Manager> const &manager ) const;
     
    protected:
    
      ConstScalarDecl(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<CG::Adapter> const &adapter,
        std::string const &scalarString
        );

    private:
    
      float m_scalar;
    };
  };
};

#endif //_FABRIC_AST_CONST_SCALAR_DECL_H
