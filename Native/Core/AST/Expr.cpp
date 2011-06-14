#include "Expr.h"
#include <Fabric/Core/CG/Location.h>
#include <Fabric/Core/CG/Error.h>

namespace Fabric
{
  namespace AST
  {
    Expr::Expr( CG::Location const &location )
      : Node( location )
    {
    }
  };
};
