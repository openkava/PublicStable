/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KLC_COMPILATION_H
#define _FABRIC_KLC_COMPILATION_H

#include <Fabric/Core/GC/Object.h>
#include <Fabric/Core/CG/CompileOptions.h>
#include <Fabric/Core/CG/Diagnostics.h>

namespace Fabric
{
  namespace AST
  {
    class GlobalList;
  };
  
  namespace CG
  {
    class CompileOptions;
    class Manager;
  };
  
  namespace KLC
  {
    class Executable;
    
    class Compilation : public GC::Object
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
      struct Source
      {
        std::string sourceCode;
        CG::Diagnostics diagnostics;
        RC::ConstHandle<AST::GlobalList> ast;
      };
      
      typedef std::map< std::string, Source > SourceMap;
      
    public:
    
      static RC::Handle<Compilation> Create(
        GC::Container *gcContainer,
        RC::Handle<CG::Manager> const &cgManager,
        CG::CompileOptions const &compileOptions
        );
      
      void add( std::string const &sourceName, std::string const &sourceCode );
      void remove( std::string const &sourceName );
      
      RC::Handle<Executable> run() const;
        
      virtual void jsonExec(
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );

    protected:
    
      Compilation(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        GC::Container *gcContainer,
        RC::Handle<CG::Manager> const &cgManager,
        CG::CompileOptions const &compileOptions
        );
      ~Compilation();
      
      void compileAll() const;
      
    private:
    
      void jsonExecAddSource(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      void jsonExecRemoveSource(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      void jsonExecGetSources(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      void jsonExecRun(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );

      GC::Container *m_gcContainer;
      RC::Handle<CG::Manager> m_cgManager;
      CG::CompileOptions m_compileOptions;
      mutable SourceMap m_sources;
    };
  }
}

#endif //_FABRIC_KLC_COMPILATION_H