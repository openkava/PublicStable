/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_OPERATOR_H
#define _FABRIC_KL_OPERATOR_H

#include <Fabric/Core/GC/Object.h>
#include <Fabric/Core/KLC/GenericFunctionPtr.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace KLC
  {
    class Executable;
    
    class Operator : public GC::Object
    {
      friend class Executable;
      
    public:
    
      GenericFunctionPtr getGenericFunctionPtr() const;
      
    protected:
    
      Operator(
        RC::ConstHandle<Executable> const &executable,
        GenericFunctionPtr functionPtr
        );
    
    private:
    
      RC::ConstHandle<Executable> m_executable;
      GenericFunctionPtr m_functionPtr;
    };
  }
}

#endif //_FABRIC_KL_OPERATOR_H
