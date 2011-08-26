/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_STRUCT_DECL_MEMBER_VECTOR_H
#define _FABRIC_AST_STRUCT_DECL_MEMBER_VECTOR_H

#include <Fabric/Base/RC/Vector.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>
#include <Fabric/Core/RT/StructMemberInfo.h>

namespace Fabric
{
  namespace Util
  {
    class JSONGenerator;
  };
  
  namespace JSON
  {
    class Array;
  };
  
  namespace RT
  {
    class Manager;
  };
  
  namespace CG
  {
    class Manager;
    class ModuleBuilder;
    class Diagnostics;
  };
  
  namespace AST
  {
    class MemberDecl;
    
    class MemberDeclVector : public RC::Vector< RC::ConstHandle<MemberDecl> >
    {
    public:
      
      static RC::ConstHandle<MemberDeclVector> Create( RC::ConstHandle<MemberDecl> const &first = 0, RC::ConstHandle<MemberDeclVector> const &remaining = 0 );

      void appendJSON( Util::JSONGenerator const &jsonGenerator ) const;
      
      void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctions ) const;
          
      void buildStructMemberInfoVector( RC::ConstHandle<RT::Manager> const &rtManager, RT::StructMemberInfoVector &structMemberInfoVector ) const;
    
    protected:
    
      MemberDeclVector();
      
    private:
    
      mutable RC::ConstHandle<JSON::Value> m_jsonValue;
    };
  };
};

#endif //_FABRIC_AST_STRUCT_DECL_MEMBER_VECTOR_H
