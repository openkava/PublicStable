/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_VAR_DECL_H
#define _FABRIC_AST_VAR_DECL_H

#include <Fabric/Core/AST/Node.h>
#include <Fabric/Core/CG/ExprValue.h>

namespace Fabric
{
  namespace Util
  {
    class SimpleString;
  };
  
  namespace CG
  {
    class BasicBlockBuilder;
    class Diagnostics;
    class Manager;
    class ModuleBuilder;
  };
  
  namespace AST
  {
    class VarDecl : public Node
    {
      FABRIC_AST_NODE_DECL( VarDecl );
      
    public:

      static RC::ConstHandle<VarDecl> Create(
        CG::Location const &location,
        std::string const &name,
        std::string const &arrayModifier
        );
      
      virtual void registerTypes( std::string const &baseType, RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const;
      
      virtual void llvmCompileToBuilder( std::string const &baseType, CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;
     
    protected:
    
      VarDecl(
        CG::Location const &location,
        std::string const &name,
        std::string const &arrayModifier
        );
      
      virtual void appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const;
    
      CG::ExprValue llvmAllocateVariable( std::string const &baseType, CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;

      RC::ConstHandle<CG::Adapter> getAdapter( std::string const &baseType, RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const;
      
    private:
    
      std::string m_name;
      std::string m_arrayModifier;
    };
  };
};

#endif //_FABRIC_AST_VAR_DECL_H
