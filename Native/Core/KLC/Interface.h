/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KLC_INTERFACE_H
#define _FABRIC_KLC_INTERFACE_H

#include <Fabric/Core/KLC/Compilation.h>
#include <Fabric/Core/KLC/Executable.h>
#include <Fabric/Core/GC/Object.h>
#include <Fabric/Core/GC/Container.h>
#include <Fabric/Core/CG/CompileOptions.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace Util
  {
    class JSONArrayGenerator;
  };
  
  namespace JSON
  {
    class Value;
  };
  
  namespace KLC
  {
    class Interface
    {
    public:
    
      Interface(
        GC::Container *gcContainer,
        RC::Handle<CG::Manager> const &cgManager,
        CG::CompileOptions const &compileOptions
        );
    
      void jsonRoute(
        std::vector<std::string> const &dst,
        size_t dstOffset,
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
        
      void jsonExec(
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      
      void jsonExecCreateCompilation(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      
      void jsonExecCreateExecutable(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      
      template<class T> void jsonExecCreateOperator(
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
        
        std::string operatorName;
        try
        {
          operatorName = argObject->get( "operatorName" )->toString()->value();
        }
        catch ( Exception e )
        {
          throw "operatorName: " + e;
        }
        
        RC::Handle<Compilation> compilation = Compilation::Create( m_gcContainer, m_cgManager, m_compileOptions );
        compilation->add( sourceName, sourceCode );
        RC::Handle<Executable> executable = compilation->run();
        executable->resolveOperator<T>( operatorName )->reg( m_gcContainer, id_ );
      }
        
    private:
    
      GC::Container *m_gcContainer;
      RC::Handle<CG::Manager> m_cgManager;
      CG::CompileOptions m_compileOptions;
    };
  };
};

#endif //_FABRIC_KLC_INTERFACE_H
