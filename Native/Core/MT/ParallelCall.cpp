/*
 *
 *  Created by Peter Zion on 10-10-08.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */
 
#include <Fabric/Core/MT/ParallelCall.h>

namespace Fabric
{
  namespace MT
  {
    ParallelCall::ParallelCall( RC::ConstHandle<Function> const &function, size_t paramCount, std::string const &debugDesc )
      : m_function( function )
      , m_paramCount( paramCount )
      , m_totalParallelCalls( 1 )
      , m_debugDesc( debugDesc )
    {
    }
    
    ParallelCall::~ParallelCall()
    {
    }
  };
};
