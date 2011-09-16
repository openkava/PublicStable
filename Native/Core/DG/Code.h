/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_DG_CODE_H
#define _FABRIC_DG_CODE_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>
#include <Fabric/Base/RC/WeakConstHandle.h>
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
  
  namespace CG
  {
    class Context;
  };
  
  namespace DG
  {
    class Context;
    class ExecutionEngine;
    class Function;
    class IRCache;
    
    class Code : public RC::Object
    {
    public:
    
      typedef void (*FunctionPtr)( ... );
    
      static RC::ConstHandle<Code> Create( RC::ConstHandle<Context> const &context, std::string const &sourceCode );
      
      std::string const &getSourceCode() const;
      RC::ConstHandle<AST::GlobalList> getAST() const;
      CG::Diagnostics const &getDiagnostics() const;
#if defined(FABRIC_BUILD_DEBUG)
      std::string const &getByteCode() const;
#endif
      
      FunctionPtr getFunctionPtrByName( std::string const &name, RC::ConstHandle<RC::Object> &objectToAvoidFreeDuringExecution ) const;

    protected:
    
      Code( RC::ConstHandle<Context> const &context, std::string const &sourceCode );
      ~Code();
      
      void compileSourceCode();
      void compileAST( bool optimize );
      void linkModule( RC::Handle<CG::Context> const &cgContext, llvm::OwningPtr<llvm::Module> &module, bool optimize );
      
    private:
    
      static void CompileOptimizedAST( void *userdata )
      {
        Code *code = static_cast<Code *>( userdata );
        code->compileAST( true );
        code->release();
      }
    
      RC::WeakConstHandle<Context> m_contextWeakRef;
      mutable MT::Mutex m_mutex;
      std::string m_sourceCode;
#if defined(FABRIC_BUILD_DEBUG)
      std::string m_byteCode;
#endif
      RC::ConstHandle<AST::GlobalList> m_ast;
      CG::Diagnostics m_diagnostics;
      RC::ConstHandle<ExecutionEngine> m_executionEngine;
    };
  };
};

#endif //_FABRIC_DG_CODE_H
