/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_AST_CONST_SIZE_DECL_H
#define _FABRIC_AST_CONST_SIZE_DECL_H

#include <Fabric/Core/AST/ConstDecl.h>

namespace Fabric
{
  namespace AST
  {
    class ConstSizeDecl: public ConstDecl
    {
      FABRIC_AST_NODE_DECL( ConstSizeDecl );

    public:

      static RC::Handle<ConstSizeDecl> Create(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<CG::Adapter> const &adapter,
        std::string const &sizeString
        )
      {
        return new ConstSizeDecl( location, name, adapter, sizeString );
      }

      RC::Handle<JSON::Object> toJSON() const;
      
      virtual void llvmCompileToScope( CG::Scope &scope, RC::ConstHandle<CG::Manager> const &manager ) const;
     
    protected:
    
      ConstSizeDecl(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<CG::Adapter> const &adapter,
        std::string const &sizeString
        );

    private:
    
      size_t m_size;
    };
  };
};

#endif //_FABRIC_AST_CONST_SIZE_DECL_H
