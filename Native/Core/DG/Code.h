/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_DG_CODE_H
#define _FABRIC_DG_CODE_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>
#include <Fabric/Core/CG/Diagnostics.h>
#include <Fabric/Core/MT/Mutex.h>

#include <set>
#include <llvm/ADT/OwningPtr.h>

namespace llvm
{
  class LLVMContext;
  class Module;
  class Function;
  class Value;
  class ExecutionEngine;
};

namespace Fabric
{
  namespace AST
  {
    class GlobalList;
    class Operator;
  };
  
  namespace DG
  {
    class Context;
    class ExecutionEngine;
    class Function;
    class IRCache;
    
    class Code : public RC::Object
    {
      typedef std::multiset<Function *> RegisteredFunctionSet;
      
    public:
    
      typedef void (*FunctionPtr)( ... );
    
      static RC::ConstHandle<Code> Create( RC::ConstHandle<Context> const &context, std::string const &sourceCode, RC::Handle<IRCache> const &irCache );
      
      std::string const &getSourceCode() const;
      RC::ConstHandle<AST::GlobalList> getAST() const;
      RC::ConstHandle<ExecutionEngine> getExecutionEngine() const;
      CG::Diagnostics const &getDiagnostics() const;
#if defined(FABRIC_BUILD_DEBUG)
      std::string const &getByteCode() const;
#endif

      void registerFunction( Function *function ) const;
      void unregisterFunction( Function *function ) const;

    protected:
    
      Code( RC::ConstHandle<Context> const &context, std::string const &sourceCode, RC::Handle<IRCache> const &irCache );
      ~Code();
      
      void compileSourceCode();
      void compileAST( bool optimize );
      void linkModule( llvm::OwningPtr<llvm::Module> &module, bool optimize );
      
    private:
    
      static void CompileOptimizedAST( void *userdata )
      {
        Code *code = static_cast<Code *>( userdata );
        code->compileAST( true );
      }
    
      RC::ConstHandle<Context> m_context;
      std::string m_sourceCode;
#if defined(FABRIC_BUILD_DEBUG)
      std::string m_byteCode;
#endif
      RC::ConstHandle<AST::GlobalList> m_ast;
      CG::Diagnostics m_diagnostics;
      RC::ConstHandle<ExecutionEngine> m_executionEngine;
      
      mutable MT::Mutex m_registeredFunctionSetMutex;
      mutable RegisteredFunctionSet m_registeredFunctionSet;
      
      RC::Handle<IRCache> m_irCache;
    };
  };
};

#endif //_FABRIC_DG_CODE_H
