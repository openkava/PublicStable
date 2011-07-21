/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_CONTEXT_H
#define _FABRIC_KL_CONTEXT_H

#include <Fabric/Core/AST/GlobalVector.h>
#include <Fabric/Core/CG/Diagnostics.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Manager.h>

#include "Scanner.h"

namespace Fabric
{
  namespace KL
  {
    struct Context
    {
      Context( 
        RC::Handle<KL::Scanner> const &scanner,
        CG::Diagnostics &diagnostics
        ) 
        : m_scanner( scanner )
        , m_diagnostics( diagnostics )
      {
      }
      
      ~Context()
      {
      }

      RC::Handle<KL::Scanner> m_scanner;
      CG::Diagnostics &m_diagnostics;
      RC::Handle<AST::GlobalVector> m_resultGlobalList;
    };
  }
}

#endif //_FABRIC_KL_CONTEXT_H
