/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "Destructor.h"
#include <Fabric/Core/AST/Param.h>
#include <Fabric/Core/AST/ParamVector.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Destructor );
    
    RC::ConstHandle<Destructor> Destructor::Create(
      CG::Location const &location,
      std::string const &thisTypeName,
      std::string const &entryName,
      RC::ConstHandle<CompoundStatement> const &body
      )
    {
      return new Destructor( location, thisTypeName, entryName, body );
    }
    
    Destructor::Destructor(
      CG::Location const &location,
      std::string const &thisTypeName,
      std::string const &entryName,
      RC::ConstHandle<CompoundStatement> const &body
      )
      : FunctionBase( location, "", body, !body )
      , m_thisTypeName( thisTypeName )
      , m_entryName( entryName )
    {
    }
    
    void Destructor::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const
    {
      FunctionBase::appendJSONMembers( jsonObjectGenerator, includeLocation );
      jsonObjectGenerator.makeMember( "thisTypeName" ).makeString( m_thisTypeName );
      jsonObjectGenerator.makeMember( "entryName" ).makeString( m_entryName );
    }

    std::string Destructor::getEntryName( RC::Handle<CG::Manager> const &cgManager ) const
    {
      if ( m_entryName.length() > 0 )
        return m_entryName;
      else return CG::destructorOverloadName( cgManager->getAdapter( m_thisTypeName ) );
    }

    RC::ConstHandle<ParamVector> Destructor::getParams( RC::Handle<CG::Manager> const &cgManager ) const
    {
      return ParamVector::Create( Param::Create( getLocation(), "this", m_thisTypeName, CG::USAGE_LVALUE ) );
    }

    void Destructor::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const
    {
      FunctionBase::llvmCompileToModule( moduleBuilder, diagnostics, buildFunctionBodies );
      moduleBuilder.setDestructorName( m_thisTypeName, getEntryName( moduleBuilder.getManager() ) );
    }
  };
};
