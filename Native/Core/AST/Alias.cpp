/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/AST/Alias.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/Util/SimpleString.h>

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
    
    void Alias::appendJSONMembers( Util::SimpleString &ss ) const
    {
      Global::appendJSONMembers(ss);
      ss.append( ",{\"newTypeName\":" );
      ss.appendJSONString( m_name );
      ss.append( ",\"oldTypeName\":" );
      ss.appendJSONString( m_adapterName );
    }
    
    void Alias::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      RC::ConstHandle<CG::Adapter> adapter = moduleBuilder.getAdapter( m_adapterName, getLocation() );
      try
      {
        RC::ConstHandle<RT::Desc> aliasDesc = moduleBuilder.getManager()->getRTManager()->registerAlias( m_name, adapter->getDesc() );
        RC::ConstHandle<CG::Adapter> aliasAdapter = moduleBuilder.getAdapter( m_adapterName, getLocation() );
        aliasAdapter->llvmPrepareModule( moduleBuilder, true );
      }
      catch ( Exception e )
      {
        addError( diagnostics, e.getDesc() );
      }
    }
  };
};
