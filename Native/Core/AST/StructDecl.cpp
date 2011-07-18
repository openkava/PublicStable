/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "StructDecl.h"
#include "MemberDeclVector.h"
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( StructDecl );
    
    RC::Handle<StructDecl> StructDecl::Create(
      CG::Location const &location,
      std::string const &name,
      RC::ConstHandle<MemberDeclVector> const &members
      )
    {
      return new StructDecl( location, name, members );
    }
    
    StructDecl::StructDecl(
      CG::Location const &location,
      std::string const &name,
      RC::ConstHandle<MemberDeclVector> const &members
      )
      : Global( location )
      , m_name( name )
      , m_members( members )
    {
    }
    
    RC::Handle<JSON::Object> StructDecl::toJSON() const
    {
      RC::Handle<JSON::Object> result = Global::toJSON();
      result->set( "name", JSON::String::Create( m_name ) );
      result->set( "members", m_members->toJSON() );
      return result;
    }
    
    void StructDecl::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const
    {
    }
  };
};
