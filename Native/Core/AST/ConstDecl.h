/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_AST_CONST_DECL_H
#define _FABRIC_AST_CONST_DECL_H

#include <Fabric/Core/AST/Node.h>
#include <Fabric/Base/RC/ConstHandle.h>

#include <string>

namespace Fabric
{
  namespace CG
  {
    class Location;
    class Scope;
    class Manager;
    class Adapter;
  };
  
  namespace AST
  {
    class ConstDecl : public Node
    {
    public:

      RC::Handle<JSON::Object> toJSON() const;
      
      std::string const &getName() const
      {
        return m_name;
      }
      RC::ConstHandle<CG::Adapter> getAdapter() const;

      virtual void llvmCompileToScope( CG::Scope &scope, RC::ConstHandle<CG::Manager> const &manager ) const = 0;
     
    protected:
    
      ConstDecl(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<CG::Adapter> const &adapter
        );

    private:
    
      std::string m_name;
      RC::ConstHandle<CG::Adapter> m_adapter;
    };
  };
};

#endif //_FABRIC_AST_CONST_DECL_H
