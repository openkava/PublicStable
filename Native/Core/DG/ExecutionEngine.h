/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_DG_EXECUTION_ENGINE_H
#define _FABRIC_DG_EXECUTION_ENGINE_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/ConstHandle.h>

#include <string>
#include <llvm/ADT/OwningPtr.h>

namespace llvm
{
  class ExecutionEngine;
  class Module;
};

namespace Fabric
{
  namespace DG
  {
    class Context;
    
    class ExecutionEngine : public RC::Object
    {
    public:
    
      typedef void (*GenericFunctionPtr)( ... );
    
      static RC::ConstHandle<ExecutionEngine> Create( RC::ConstHandle<Context> const &context, llvm::Module *llvmModule );
      
      GenericFunctionPtr getFunctionByName( std::string const &functionName ) const;
      
      class ContextSetter
      {
      public:
      
        ContextSetter( RC::ConstHandle<Context> const &context );
        ~ContextSetter();
      
      private:
      
        RC::ConstHandle<Context> m_oldContext;
      };      

    protected:
    
      ExecutionEngine( RC::ConstHandle<Context> const &context, llvm::Module *llvmModule );
      
    private:
    
      static void *LazyFunctionCreator( std::string const &functionName );
      static void Report( char const *data, size_t length );
    
      Context const *m_context;
      llvm::OwningPtr<llvm::ExecutionEngine> m_llvmExecutionEngine;
      
      static RC::ConstHandle<Context> s_currentContext;
    };
  };
};

#endif //_FABRIC_DG_EXECUTION_ENGINE_H
