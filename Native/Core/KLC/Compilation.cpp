/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KLC/Executable.h>
#include <Fabric/Core/KLC/Compilation.h>
#include <Fabric/Core/KL/Scanner.h>
#include <Fabric/Core/KL/StringSource.h>
#include <Fabric/Core/CG/Manager.h>

namespace Fabric
{
  namespace KLC
  {
    Compilation::Compilation(
      RC::Handle<CG::Manager> const &cgManager,
      CG::CompileOptions const &compileOptions
      )
      : m_cgManager( cgManager )
      , m_compileOptions( compileOptions )
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
    
    RC::ConstHandle<Executable> Compilation::run() const
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
  }
}
