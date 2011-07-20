/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "StructDecl.h"
#include "MemberDeclVector.h"
#include <Fabric/Core/CG/Diagnostics.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/RT/StructDesc.h>
#include <Fabric/Core/RT/StructMemberInfo.h>
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
    
    void StructDecl::registerTypes( RC::Handle<RT::Manager> const &rtManager, CG::Diagnostics &diagnostics ) const
    {
      RT::StructMemberInfoVector structMemberInfoVector;
      m_members->buildStructMemberInfoVector( rtManager, structMemberInfoVector );
      try
      {
        rtManager->registerStruct( m_name, structMemberInfoVector );
      }
      catch ( Exception e )
      {
        addError( diagnostics, e.getDesc() );
      }
    }
  };
};
