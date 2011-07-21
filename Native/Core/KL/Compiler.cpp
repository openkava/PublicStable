/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KL/Compiler.h>
#include <Fabric/Core/KL/StringSource.h>
#include <Fabric/Core/KL/Scanner.h>
#include <Fabric/Core/KL/Parser.hpp>
#include <Fabric/Core/CG/Diagnostics.h>

namespace Fabric
{
  namespace KL
  {
    RC::ConstHandle<Compiler> Compiler::Create()
    {
      return new Compiler;
    }
    
    Compiler::Compiler()
    {
    }
    
    RC::ConstHandle<RC::Object> Compiler::compile( std::string const &klSource ) const
    {
      RC::ConstHandle<Source> source = StringSource::Create( klSource );
      RC::Handle<Scanner> scanner = Scanner::Create( source );
      CG::Diagnostics diagnostics;
      RC::Handle<AST::GlobalVector> ast;
      KL::Parse( scanner, diagnostics, ast );
      if ( diagnostics.containsError() )
        throw Exception( "KL compile failed" );
      return ast;
    }
  }
}
