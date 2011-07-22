/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_VAR_DECL_VECTOR_H
#define _FABRIC_AST_VAR_DECL_VECTOR_H

#include <Fabric/Base/RC/Vector.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace JSON
  {
    class Array;
  };
  
  namespace CG
  {
    class BasicBlockBuilder;
    class Diagnostics;
    class ModuleBuilder;
  };
  
  namespace AST
  {
    class VarDecl;
    
    class VarDeclVector : public RC::Vector< RC::ConstHandle<VarDecl> >
    {
    public:
      
      static RC::Handle<VarDeclVector> Create();
      static RC::Handle<VarDeclVector> Create( RC::ConstHandle<VarDecl> const &first );
      static RC::Handle<VarDeclVector> Create( RC::ConstHandle<VarDecl> const &first, RC::ConstHandle<VarDeclVector> const &remaining );

      RC::Handle<JSON::Array> toJSON() const;
      
      void llvmPrepareModule( std::string const &baseType, CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
    
      void llvmCompileToBuilder( std::string const &baseType, CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;

    protected:
    
      VarDeclVector();
    };
  };
};

#endif //_FABRIC_AST_VAR_DECL_VECTOR_H
