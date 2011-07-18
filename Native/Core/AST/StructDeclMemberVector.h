/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_STRUCT_DECL_MEMBER_VECTOR_H
#define _FABRIC_AST_STRUCT_DECL_MEMBER_VECTOR_H

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
    class ModuleBuilder;
    class Diagnostics;
  };
  
  namespace AST
  {
    class StructDeclMember;
    
    class StructDeclMemberVector : public RC::Vector< RC::ConstHandle<StructDeclMember> >
    {
    public:
      
      static RC::Handle<StructDeclMemberVector> Create();

      RC::Handle<JSON::Array> toJSON() const;
          
      void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
    
    protected:
    
      StructDeclMemberVector();
    };
  };
};

#endif //_FABRIC_AST_STRUCT_DECL_MEMBER_VECTOR_H
