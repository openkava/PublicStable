/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MT_FUNCTION_H
#define _FABRIC_MT_FUNCTION_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Core/MT/Mutex.h>

#include <set>

namespace Fabric
{
  namespace MT
  {
    class ParallelCall;
    
    class Function : public RC::Object
    {
      typedef std::multiset<ParallelCall const *> RegisteredParallelCallSet;
      
    public:
    
      typedef void (*FunctionPtr)( ... );
    
      virtual FunctionPtr getFunctionPtr() const = 0;
      virtual RC::Object const *getObjectOwningFunctionPtr() const = 0;
    
      void registerParallelCall( ParallelCall const *parallelCall ) const;
      void unregisterParallelCall( ParallelCall const *parallelCall ) const;
      
    protected:
    
      Function();
    
      void onFunctionPtrChange( FunctionPtr functionPtr, RC::Object const *objectOwningFunctionPtr );
      
    private:
    
      mutable RegisteredParallelCallSet m_registeredParallelCalls;
      mutable MT::Mutex m_registeredParallelCallsMutex;
    };
  };
};

#endif //_FABRIC_MT_FUNCTION_H
