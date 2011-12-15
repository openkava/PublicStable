/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KLC/Operator.h>
#include <Fabric/Core/KLC/Executable.h>

namespace Fabric
{
  namespace KLC
  {
    Operator::Operator(
      RC::ConstHandle<Executable> const &executable,
      GenericFunctionPtr functionPtr
      )
      : m_executable( executable )
      , m_functionPtr( functionPtr )
    {
    }

    GenericFunctionPtr Operator::getGenericFunctionPtr() const
    {
      return m_functionPtr;
    }
  }
}
