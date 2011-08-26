/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/AST/Alias.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Alias );
    
    RC::ConstHandle<Alias> Alias::Create(
      CG::Location const &location,
      std::string const &name,
      std::string const &adapterName
      )
    {
      return new Alias( location, name, adapterName );
    }

    Alias::Alias(
      CG::Location const &location,
      std::string const &name,
      std::string const &adapterName
      )
      : Global( location )
      , m_name( name )
      , m_adapterName( adapterName )
    {
    }
    
    void Alias::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Global::appendJSONMembers( jsonObjectGenerator );
      jsonObjectGenerator.makeMember( "newTypeName" ).makeString( m_name );
      jsonObjectGenerator.makeMember( "oldTypeName" ).makeString( m_adapterName );
    }
    
    void Alias::registerTypes( RC::Handle<RT::Manager> const &rtManager, CG::Diagnostics &diagnostics ) const
    {
      try
      {
        RC::ConstHandle<RT::Desc> desc = rtManager->getDesc( m_adapterName );
        rtManager->registerAlias( m_name, desc );
      }
      catch ( Exception e )
      {
        addError( diagnostics, e );
      }
    }
    
    void Alias::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctions ) const
    {
      try
      {
        RC::ConstHandle<CG::Adapter> adapter = moduleBuilder.getAdapter( m_adapterName, getLocation() );
        moduleBuilder.getManager()->getRTManager()->registerAlias( m_name, adapter->getDesc() );
        RC::ConstHandle<CG::Adapter> aliasAdapter = moduleBuilder.getAdapter( m_name, getLocation() );
        aliasAdapter->llvmPrepareModule( moduleBuilder, buildFunctions );
      }
      catch ( Exception e )
      {
        addError( diagnostics, e );
      }
    }
  };
};
