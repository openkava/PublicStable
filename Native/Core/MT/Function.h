/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MT_FUNCTION_H
#define _FABRIC_MT_FUNCTION_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/ConstHandle.h>
#include <Fabric/Core/MT/Mutex.h>

#include <set>

namespace Fabric
{
  namespace MT
  {
    class ParallelCall;
    
    class Function : public RC::Object
    {
    public:
    
      typedef void (*FunctionPtr)( ... );
    
      virtual FunctionPtr getFunctionPtr( RC::ConstHandle<RC::Object> &objectToAvoidFreeDuringExecution ) const = 0;
      
    protected:
    
      Function();
    };
  };
};

#endif //_FABRIC_MT_FUNCTION_H
