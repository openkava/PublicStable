/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/AST/GlobalList.h>
#include <Fabric/Core/AST/Function.h>
#include <Fabric/Core/AST/Global.h>
#include <Fabric/Core/CG/Diagnostics.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    RC::ConstHandle<GlobalList> GlobalList::Create( RC::ConstHandle<Global> const &first, RC::ConstHandle<GlobalList> const &remaining )
    {
      return new GlobalList( 0, first, remaining );
    }
    
    RC::ConstHandle<GlobalList> GlobalList::Create( RC::ConstHandle<GlobalList> const &lhs, RC::ConstHandle<GlobalList> const &rhs )
    {
      return new GlobalList( lhs, 0, rhs );
    }
    
    GlobalList::GlobalList( RC::ConstHandle<GlobalList> const &before, RC::ConstHandle<Global> const &global, RC::ConstHandle<GlobalList> const &after )
      : m_before( before )
      , m_global( global )
      , m_after( after )
    {
    }
    
    Util::SimpleString GlobalList::toJSON( bool includeLocation ) const
    {
      Util::SimpleString ss;
      Util::JSONGenerator jsonGenerator( &ss );
      Util::JSONArrayGenerator jsonArrayGenerator = jsonGenerator.makeArray();
      appendJSON( jsonArrayGenerator, includeLocation );
      return ss;
    }
    
    void GlobalList::appendJSON( Util::JSONArrayGenerator const &jsonArrayGenerator, bool includeLocation ) const
    {
      if ( m_before )
        m_before->appendJSON( jsonArrayGenerator, includeLocation );
      if ( m_global )
        m_global->appendJSON( jsonArrayGenerator.makeElement(), includeLocation );
      if ( m_after )
        m_after->appendJSON( jsonArrayGenerator, includeLocation );
    }
    
    void GlobalList::collectUses( UseNameToLocationMap &uses ) const
    {
      if ( m_before )
        m_before->collectUses( uses );
      if ( m_global )
        m_global->collectUses( uses );
      if ( m_after )
        m_after->collectUses( uses );
    }
    
    void GlobalList::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      if ( m_before )
        m_before->registerTypes( cgManager, diagnostics );
      if ( m_global )
      {
        try
        {
          m_global->registerTypes( cgManager, diagnostics );
        }
        catch ( CG::Error e )
        {
          diagnostics.addError( e.getLocation(), e.getDesc() );
        }
      }
      if ( m_after )
        m_after->registerTypes( cgManager, diagnostics );
    }
    
    void GlobalList::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctions ) const
    {
      if ( m_before )
        m_before->llvmCompileToModule( moduleBuilder, diagnostics, buildFunctions );
      if ( m_global )
      {
        try
        {
          m_global->llvmCompileToModule( moduleBuilder, diagnostics, buildFunctions );
        }
        catch ( CG::Error e )
        {
          diagnostics.addError( e.getLocation(), e.getDesc() );
        }
      }
      if ( m_after )
        m_after->llvmCompileToModule( moduleBuilder, diagnostics, buildFunctions );
    }

    void GlobalList::collectFunctions( std::vector< RC::ConstHandle<AST::Function> > &result ) const
    {
      if ( m_before )
        m_before->collectFunctions( result );
      if ( !!m_global && m_global->isFunction() )
        result.push_back( RC::ConstHandle<AST::Function>::StaticCast( m_global ) );
      if ( m_after )
        m_after->collectFunctions( result );
    }
  };
};
