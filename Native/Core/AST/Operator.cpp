/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "Operator.h"
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Operator );
    
    RC::Handle<Function> Operator::Create(
      CG::Location const &location,
      std::string const &friendlyName,
      std::string const &entryName,
      RC::ConstHandle<ParamVector> const &params,
      RC::ConstHandle<CompoundStatement> const &body
      )
    {
      return new Operator( location, friendlyName, entryName, params, body );
    }
  
    Operator::Operator( 
      CG::Location const &location,
      std::string const &friendlyName,
      std::string const &entryName,
      RC::ConstHandle<ParamVector> const &params,
      RC::ConstHandle<CompoundStatement> const &body
      )
      : Function( location, friendlyName, entryName, "", params, body )
    {
    }
  }
}
