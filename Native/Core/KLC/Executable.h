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
  
  namespace KLC
  {
    class Operator;
    
    class Executable : public GC::Object
    {
      friend class Compilation;
      
      RC::Handle<Operator> resolveOperator( std::string const &operatorName ) const;
      
      CG::Diagnostics const &getDiagnostics() const;

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
        RC::Handle<CG::Manager> const &cgManager,
        RC::ConstHandle<AST::GlobalList> const &ast,
        CG::CompileOptions const &compileOptions,
        CG::Diagnostics const &diagnostics
        );
    
    private:
    
      void *lazyFunctionCreator( std::string const &functionName ) const;
      static void *LazyFunctionCreator( std::string const &functionName );

      static void Report( char const *data, size_t length );

      RC::Handle<CG::Manager> m_cgManager;
      RC::ConstHandle<AST::GlobalList> m_ast;
      CG::Diagnostics m_diagnostics;
      
      llvm::OwningPtr<llvm::ExecutionEngine> m_llvmExecutionEngine;

      static Util::TLSVar<Executable const *> s_currentExecutable;
    };
  }
}

#endif //_FABRIC_KL_EXECUTABLE_H
