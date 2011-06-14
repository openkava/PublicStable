/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "Parse.h"

#include "Context.h"

extern int kl_parse( Fabric::KL::Context &ctx );
extern int kl_debug;

namespace Fabric
{
  namespace KL
  {
    void Parse( 
      Source &source, 
      RC::Handle<CG::Manager> const &cgManager, 
      CG::Diagnostics &diagnostics,
      Fabric::RC::Handle<Fabric::AST::GlobalList> *result,
      bool debugParse
      )
    {
      Scanner     scanner( source, cgManager );
      Context     ctx( &scanner, diagnostics, cgManager );

      // TODO: Do we check the status?
      kl_debug = debugParse ? 1 : 0;

      kl_parse( ctx );

      *result = ctx.m_resultGlobalList;
    }
  }
}
