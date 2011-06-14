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
      : m_registeredParallelCallsMutex("MT::Function")
    {
    }
    
    void Function::registerParallelCall( ParallelCall const *parallelCall ) const
    {
      MT::Mutex::Lock lock( m_registeredParallelCallsMutex );
      m_registeredParallelCalls.insert( parallelCall );
    }
    
    void Function::unregisterParallelCall( ParallelCall const *parallelCall ) const
    {
      MT::Mutex::Lock lock( m_registeredParallelCallsMutex );
      RegisteredParallelCallSet::iterator it = m_registeredParallelCalls.find( parallelCall );
      FABRIC_ASSERT( it != m_registeredParallelCalls.end() );
      m_registeredParallelCalls.erase( it );
    }
      
    void Function::onFunctionPtrChange( FunctionPtr functionPtr, RC::Object const *objectOwningFunctionPtr )
    {
      for ( RegisteredParallelCallSet::const_iterator it=m_registeredParallelCalls.begin();
        it!=m_registeredParallelCalls.end(); ++it )
      {
        ParallelCall const *registeredParallelCall = *it;
        registeredParallelCall->onFunctionPtrChange( functionPtr, objectOwningFunctionPtr );
      }
    }
  };
};
