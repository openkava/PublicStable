// Operator.cpp
// *****************************************************************************
// (C) 2011 Fabric Technologies, Inc.

#include "Operator.h"

namespace Fabric
{
  namespace AST
  {
    Operator::Operator( 
      CG::Location const &location,
      std::string const &name,
      RC::ConstHandle<ParamList> const &params,
      RC::ConstHandle<CompoundStatement> const &body
      )
      : Function( location, name, CG::ExprType(), params, body )
    {
    }

    std::string Operator::localDesc() const
    {
      return "Operator( " + getName() + " )";
    }
  }
}
