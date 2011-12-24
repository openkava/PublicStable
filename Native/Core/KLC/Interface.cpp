/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KLC/Interface.h>
#include <Fabric/Core/KLC/ArrayGeneratorOperator.h>
#include <Fabric/Core/KLC/ArrayTransformOperator.h>
#include <Fabric/Core/KLC/Compilation.h>
#include <Fabric/Core/KLC/Executable.h>
#include <Fabric/Core/KLC/MapOperator.h>
#include <Fabric/Core/KLC/ReduceOperator.h>
#include <Fabric/Core/KLC/ValueMapOperator.h>
#include <Fabric/Core/KLC/ValueTransformOperator.h>
#include <Fabric/Core/MR/ConstArray.h>
#include <Fabric/Core/GC/Object.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Core/Util/Parse.h>
#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace KLC
  {
    Interface::Interface(
      GC::Container *gcContainer,
      RC::Handle<CG::Manager> const &cgManager,
      CG::CompileOptions const &compileOptions
      )
      : m_gcContainer( gcContainer )
      , m_cgManager( cgManager )
      , m_compileOptions( compileOptions )
    {
    }
  
    void Interface::jsonRoute(
      std::vector<std::string> const &dst,
      size_t dstOffset,
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( dst.size() - dstOffset == 0 )
        jsonExec( cmd, arg, resultJAG );
      else m_gcContainer->jsonRoute( dst, dstOffset, cmd, arg, resultJAG );
    }
      
    void Interface::jsonExec(
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( cmd == "createCompilation" )
        jsonExecCreateCompilation( arg, resultJAG );
      else if ( cmd == "createExecutable" )
        jsonExecCreateExecutable( arg, resultJAG );
      else if ( cmd == "createMapOperator" )
        jsonExecCreateOperator<MapOperator>( arg, resultJAG );
      else if ( cmd == "createReduceOperator" )
        jsonExecCreateOperator<ReduceOperator>( arg, resultJAG );
      else if ( cmd == "createArrayGeneratorOperator" )
        jsonExecCreateOperator<ArrayGeneratorOperator>( arg, resultJAG );
      else if ( cmd == "createValueMapOperator" )
        jsonExecCreateOperator<ValueMapOperator>( arg, resultJAG );
      else if ( cmd == "createValueTransformOperator" )
        jsonExecCreateOperator<ValueTransformOperator>( arg, resultJAG );
      else if ( cmd == "createArrayTransformOperator" )
        jsonExecCreateOperator<ArrayTransformOperator>( arg, resultJAG );
      else throw Exception( "unknown command: " + _(cmd) );
    }
    
    void Interface::jsonExecCreateCompilation(
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
      
      std::string sourceName;
      try
      {
        RC::ConstHandle<JSON::Value> sourceNameJSONValue = argObject->maybeGet( "sourceName" );
        if ( sourceNameJSONValue )
          sourceName = sourceNameJSONValue->toString()->value();
      }
      catch ( Exception e )
      {
        throw "sourceName: " + e;
      }
      
      std::string sourceCode;
      try
      {
        RC::ConstHandle<JSON::Value> sourceCodeJSONValue = argObject->maybeGet( "sourceCode" );
        if ( sourceCodeJSONValue )
          sourceCode = sourceCodeJSONValue->toString()->value();
      }
      catch ( Exception e )
      {
        throw "sourceCode: " + e;
      }
      
      RC::Handle<Compilation> compilation = Compilation::Create( m_gcContainer, m_cgManager, m_compileOptions );
      if ( sourceName.length() > 0 || sourceCode.length() > 0 )
        compilation->add( sourceName, sourceCode );
      compilation->reg( m_gcContainer, id_ );
    }
    
    void Interface::jsonExecCreateExecutable(
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
      
      std::string sourceName;
      try
      {
        sourceName = argObject->get( "sourceName" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "sourceName: " + e;
      }
      
      std::string sourceCode;
      try
      {
        sourceCode = argObject->get( "sourceCode" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "sourceCode: " + e;
      }
      
      RC::Handle<Compilation> compilation = Compilation::Create( m_gcContainer, m_cgManager, m_compileOptions );
      compilation->add( sourceName, sourceCode );
      RC::Handle<Executable> executable = compilation->run();
      executable->reg( m_gcContainer, id_ );
    }
  };
};
