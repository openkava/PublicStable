/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KLC/Executable.h>
#include <Fabric/Core/KLC/Compilation.h>
#include <Fabric/Core/KL/Scanner.h>
#include <Fabric/Core/KL/StringSource.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace KLC
  {
    Compilation::Compilation(
      GC::Container *gcContainer,
      RC::Handle<CG::Manager> const &cgManager,
      CG::CompileOptions const &compileOptions
      )
      : m_gcContainer( gcContainer )
      , m_cgManager( cgManager )
      , m_compileOptions( compileOptions )
    {
    }

    Compilation::~Compilation()
    {
    }
    
    void Compilation::add( std::string const &sourceName, std::string const &sourceCode )
    {
      Source &source = m_sources[sourceName];
      if ( source.sourceCode != sourceCode )
      {
        source.sourceCode = sourceCode;
        source.diagnostics.clear();
        source.ast = 0;
      }
    }
    
    void Compilation::remove( std::string const &sourceName )
    {
      SourceMap::iterator it = m_sources.find( sourceName );
      if ( it != m_sources.end() )
        m_sources.erase( it );
    }
    
    RC::Handle<Executable> Compilation::run() const
    {
      CG::Diagnostics diagnostics;
      
      for ( SourceMap::iterator it = m_sources.begin(); it != m_sources.end(); ++it )
      {
        std::string const &sourceName = it->first;
        Source &source = it->second;
        if ( source.diagnostics.empty() && !source.ast )
        {
          RC::ConstHandle<KL::Source> stringSource = KL::StringSource::Create( sourceName, source.sourceCode );
          RC::Handle<KL::Scanner> scanner = KL::Scanner::Create( stringSource );
          source.ast = KL::Parse( scanner, source.diagnostics );
        }
        diagnostics.append( source.diagnostics );
      }
      
      RC::ConstHandle<AST::GlobalList> globalAST;
      for ( SourceMap::const_iterator it = m_sources.begin(); it != m_sources.end(); ++it )
        globalAST = AST::GlobalList::Create( globalAST, it->second.ast );
        
      return new Executable( m_cgManager, globalAST, m_compileOptions, diagnostics );
    }
        
    void Compilation::jsonExec(
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( cmd == "addSource" )
        jsonExecAddSource( arg, resultJAG );
      else if ( cmd == "removeSource" )
        jsonExecRemoveSource( arg, resultJAG );
      else if ( cmd == "getSources" )
        jsonExecGetSources( arg, resultJAG );
      else if ( cmd == "run" )
        jsonExecRun( arg, resultJAG );
      else GC::Object::jsonExec( cmd, arg, resultJAG );
    }
    
    void Compilation::jsonExecAddSource(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      RC::ConstHandle<JSON::Object> argObject = arg->toObject();
      
      std::string sourceName;
      try
      {
        sourceName = argObject->get("sourceName")->toString()->value();
      }
      catch ( Exception e )
      {
        throw "sourceName: " + e;
      }
      
      std::string sourceCode;
      try
      {
        sourceCode = argObject->get("sourceCode")->toString()->value();
      }
      catch ( Exception e )
      {
        throw "sourceCode: " + e;
      }
      
      add( sourceName, sourceCode );
    }
    
    void Compilation::jsonExecRemoveSource(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      RC::ConstHandle<JSON::Object> argObject = arg->toObject();
      
      std::string sourceName;
      try
      {
        sourceName = argObject->get("sourceName")->toString()->value();
      }
      catch ( Exception e )
      {
        throw "sourceName: " + e;
      }
      
      remove( sourceName );
    }
    
    void Compilation::jsonExecGetSources(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      Util::JSONGenerator jg = resultJAG.makeElement();
      Util::JSONObjectGenerator jog = jg.makeObject();
      for ( SourceMap::const_iterator it = m_sources.begin(); it != m_sources.end(); ++it )
      {
        Util::JSONGenerator subJG = jog.makeMember( it->first );
        subJG.makeString( it->second.sourceCode );
      }
    }
    
    void Compilation::jsonExecRun(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      RC::ConstHandle<JSON::Object> argObject = arg->toObject();
      
      std::string id_;
      try
      {
        id_ = argObject->get( "id" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "id: " + e;
      }
      
      RC::Handle<Executable> executable = run();
      executable->reg( m_gcContainer, id_ );
    }
  }
}
