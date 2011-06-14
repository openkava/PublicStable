/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <Fabric/Core/AST/GlobalList.h>
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
        KL::Scanner *scanner,
        CG::Diagnostics &diagnostics,
        RC::Handle<CG::Manager> const &cgManager ) 
        : m_scanner( scanner ),
          m_diagnostics( diagnostics ),
          m_cgManager( cgManager )
      {
      }

      KL::Scanner *m_scanner;
      CG::Diagnostics &m_diagnostics;
      RC::Handle<AST::GlobalList> m_resultGlobalList;
      RC::Handle<CG::Manager> const &m_cgManager;

      // Variable declaration list
      CG::Adapter const *m_varType;
    };
  }
}

#endif // __CONTEXT_H__
