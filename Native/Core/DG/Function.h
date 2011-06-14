/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_DG_FUNCTION_H
#define _FABRIC_DG_FUNCTION_H

#include <Fabric/Core/MT/Function.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace DG
  {
    class Code;
    class ExecutionEngine;
    
    class Function : public MT::Function
    {
    public:
    
      static RC::ConstHandle<Function> Create( RC::ConstHandle<Code> const &code, std::string const &functionName );
    
      virtual FunctionPtr getFunctionPtr() const;
      virtual RC::Object const *getObjectOwningFunctionPtr() const;
      
      void onExecutionEngineChange( RC::ConstHandle<ExecutionEngine> const &executionEngine );
    
    protected:
    
      Function( RC::ConstHandle<Code> const &code, std::string const &functionName );
      ~Function();
      
    private:
    
      RC::ConstHandle<Code> m_code;
      std::string m_functionName;
      mutable RC::ConstHandle<ExecutionEngine> m_executionEngine;
      mutable FunctionPtr m_functionPtr;
    };
  };
};

#endif //_FABRIC_DG_FUNCTION_H
