/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MT/Function.h>
#include <Fabric/Core/MT/ParallelCall.h>
#include <Fabric/Core/MT/Util.h>

namespace Fabric
{
  namespace MT
  {
    Function::Function()
      : m_mutex("MT::Function")
    {
    }
    
    Function::FunctionPtr Function::getFunctionPtr( RC::ConstHandle<RC::Object> &handleToObjectOwningFunctionPtr ) const
    {
      MT::Mutex::Lock mutexLock( m_mutex );
      handleToObjectOwningFunctionPtr = m_objectOwningFunctionPtr;
      return m_functionPtr;
    }
    
    void Function::onFunctionPtrChange( FunctionPtr functionPtr, RC::ConstHandle<RC::Object> const &objectOwningFunctionPtr )
    {
      MT::Mutex::Lock mutexLock( m_mutex );
      m_functionPtr = functionPtr;
      m_objectOwningFunctionPtr = objectOwningFunctionPtr;
    }
  };
};
