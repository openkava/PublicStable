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
    }
    
    Function::~Function()
    {
    }

    Function::FunctionPtr Function::getFunctionPtr( RC::ConstHandle<RC::Object> &objectToAvoidFreeDuringExecution ) const
    {
      return m_code->getFunctionPtrByName( m_functionName, objectToAvoidFreeDuringExecution );
    }
  };
};
