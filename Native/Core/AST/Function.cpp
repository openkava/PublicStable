/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "Function.h"
#include <Fabric/Core/AST/Param.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Function );
    
    RC::ConstHandle<Function> Function::Create(
      CG::Location const &location,
      std::string const &friendlyName,
      std::string const &entryName,
      std::string const &returnTypeName,
      RC::ConstHandle<ParamVector> const &params,
      RC::ConstHandle<CompoundStatement> const &body
      )
    {
      return new Function( location, friendlyName, entryName, returnTypeName, params, body, !body );
    }
    
    RC::ConstHandle<Function> Function::Create(
      CG::Location const &location,
      std::string const &friendlyName,
      std::string const *entryName,
      std::string const &returnTypeName,
      RC::ConstHandle<ParamVector> const &params,
      RC::ConstHandle<CompoundStatement> const &body
      )
    {
      return new Function( location, friendlyName, entryName? *entryName: friendlyName, returnTypeName, params, body, !body );
    }
    
    Function::Function(
      CG::Location const &location,
      std::string const &friendlyName,
      std::string const &entryName,
      std::string const &returnTypeName,
      RC::ConstHandle<ParamVector> const &params,
      RC::ConstHandle<CompoundStatement> const &body,
      bool exportSymbol
      )
      : FunctionBase( location, returnTypeName, body, exportSymbol )
      , m_friendlyName( friendlyName )
      , m_entryName( entryName )
      , m_params( params )
    {
    }
    
    void Function::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      FunctionBase::appendJSONMembers( jsonObjectGenerator );
      jsonObjectGenerator.makeMember( "friendlyName" ).makeString( m_friendlyName );
      jsonObjectGenerator.makeMember( "entryName" ).makeString( m_entryName );
      m_params->appendJSON( jsonObjectGenerator.makeMember( "params" ) );
    }
    
    std::string const *Function::getFriendlyName( RC::Handle<CG::Manager> const &cgManager ) const
    {
      if ( cgManager->maybeGetAdapter( m_friendlyName ) )
        return 0;
      else return &m_friendlyName;
    }

    std::string Function::getEntryName( RC::Handle<CG::Manager> const &cgManager ) const
    {
      RC::ConstHandle<CG::Adapter> adapter = cgManager->maybeGetAdapter( m_friendlyName );
      if ( adapter )
        return CG::constructOverloadName( adapter, m_params->getAdapters( cgManager ) );
      else return m_entryName;
    }

    RC::ConstHandle<ParamVector> Function::getParams( RC::Handle<CG::Manager> const &cgManager ) const
    {
      if ( cgManager->maybeGetAdapter( m_friendlyName ) )
        return ParamVector::Create( Param::Create( getLocation(), "self", m_friendlyName, CG::USAGE_LVALUE ), m_params );
      else return m_params;
    }
  };
};
