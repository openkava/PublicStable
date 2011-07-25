/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "Function.h"
#include <Fabric/Core/AST/Param.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Core/Util/SimpleString.h>

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
      return new Function( location, friendlyName, entryName, returnTypeName, params, body );
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
      return new Function( location, friendlyName, entryName? *entryName: friendlyName, returnTypeName, params, body );
    }
    
    Function::Function(
        CG::Location const &location,
        std::string const &friendlyName,
        std::string const &entryName,
        std::string const &returnTypeName,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        )
      : FunctionBase( location, returnTypeName, body )
      , m_friendlyName( friendlyName )
      , m_entryName( entryName )
      , m_params( params )
    {
    }
    
    void Function::appendJSONMembers( Util::SimpleString &ss ) const
    {
      FunctionBase::appendJSONMembers(ss);
      ss.append( ",\"friendlyName\":" );
      ss.appendJSONString( m_friendlyName );
      ss.append( ",\"entryName\":" );
      ss.appendJSONString( m_entryName );
      ss.append( ",\"params\":" );
      m_params->appendJSON( ss );
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
