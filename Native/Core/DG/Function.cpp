/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/DG/Function.h>
#include <Fabric/Core/DG/Code.h>
#include <Fabric/Core/DG/ExecutionEngine.h>
#include <Fabric/Core/MT/ParallelCall.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  
  
  namespace DG
  {
    RC::ConstHandle<Function> Function::Create( RC::ConstHandle<Code> const &code, std::string const &functionName )
    {
      return new DG::Function( code, functionName );
    }
    
    Function::Function( RC::ConstHandle<Code> const &code, std::string const &functionName )
      : m_code( code )
      , m_functionName( functionName )
    {
      m_code->registerFunction( this );
      onExecutionEngineChange( code->getExecutionEngine() );
    }
    
    Function::~Function()
    {
      m_code->unregisterFunction( this );
    }

    Function::FunctionPtr Function::getFunctionPtr() const
    {
      return m_functionPtr;
    }
    
    RC::Object const *Function::getObjectOwningFunctionPtr() const
    {
      return m_executionEngine.ptr();
    }

    void Function::onExecutionEngineChange( RC::ConstHandle<ExecutionEngine> const &executionEngine )
    {
      m_executionEngine = executionEngine;
      m_functionPtr = m_executionEngine->getFunctionByName( m_functionName );
      if ( !m_functionPtr )
        throw Exception( "function " + _(m_functionName) + " not found" );
      onFunctionPtrChange( m_functionPtr, m_executionEngine.ptr() );
    }
  };
};
