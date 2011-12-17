/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_EXECUTABLE_H
#define _FABRIC_KL_EXECUTABLE_H

#include <Fabric/Core/GC/Object.h>
#include <Fabric/Core/CG/Diagnostics.h>
#include <Fabric/Core/CG/CompileOptions.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/Util/TLS.h>

#include <llvm/ADT/OwningPtr.h>

namespace llvm
{
  class ExecutionEngine;
  class Module;
};

namespace Fabric
{
  namespace AST
  {
    class GlobalList;
  };
  
  namespace CG
  {
    class Context;
  };
  
  namespace KLC
  {
    class ArrayGeneratorOperator;
    class MapOperator;
    
    class Executable : public GC::Object
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
    public:
    
      static RC::Handle<Executable> Create(
        GC::Container *gcContainer,
        RC::Handle<CG::Manager> const &cgManager,
        RC::ConstHandle<AST::GlobalList> const &ast,
        CG::CompileOptions const &compileOptions,
        CG::Diagnostics const &diagnostics
        );
      
      RC::ConstHandle<AST::GlobalList> getAST() const;
      CG::Diagnostics const &getDiagnostics() const;
      RC::Handle<CG::Manager> getCGManager() const;
      
      RC::Handle<MapOperator> resolveMapOperator( std::string const &mapOperatorName ) const;
      RC::Handle<ArrayGeneratorOperator> resolveArrayGeneratorOperator( std::string const &arrayGeneratorOperatorName ) const;
        
      virtual void jsonExec(
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );

    protected:
    
      class CurrentExecutableSetter
      {
      public:
      
        CurrentExecutableSetter( Executable const *executable )
          : m_oldExecutable( s_currentExecutable )
        {
          s_currentExecutable = executable;
        }
        
        ~CurrentExecutableSetter()
        {
          s_currentExecutable = m_oldExecutable;
        }
        
      private:
      
        Executable const *m_oldExecutable;
      };
    
      Executable(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        GC::Container *gcContainer,
        RC::Handle<CG::Manager> const &cgManager,
        RC::ConstHandle<AST::GlobalList> const &ast,
        CG::CompileOptions const &compileOptions,
        CG::Diagnostics const &diagnostics
        );
    
    private:
    
      void jsonExecGetAST(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
    
      void jsonExecGetDiagnostics(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
    
      void jsonExecResolveMapOperator(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
    
      void jsonExecResolveArrayGeneratorOperator(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
    
      void *lazyFunctionCreator( std::string const &functionName ) const;
      static void *LazyFunctionCreator( std::string const &functionName );

      static void Report( char const *data, size_t length );

      GC::Container *m_gcContainer;
      RC::Handle<CG::Manager> m_cgManager;
      RC::ConstHandle<AST::GlobalList> m_ast;
      CG::Diagnostics m_diagnostics;
      
      RC::Handle<CG::Context> m_cgContext;
      llvm::OwningPtr<llvm::ExecutionEngine> m_llvmExecutionEngine;

      static Util::TLSVar<Executable const *> s_currentExecutable;
    };
  }
}

#endif //_FABRIC_KL_EXECUTABLE_H
