#include "ImplType.h"
#include "Desc.h"

#include <Fabric/Base/Exception.h>

namespace Fabric
{
  
  
  namespace RT
  {
    RC::ConstHandle<Desc> getStrongerTypeOrNone( RC::ConstHandle<Desc> const &lhsDesc, RC::ConstHandle<Desc> const &rhsDesc )
    {
      ImplType lhsType = lhsDesc->getType(), rhsType = rhsDesc->getType();
      if ( isSimple( lhsType ) && isSimple( rhsType ) )
      {
        // Arrange by casting priority.
        if( lhsType >= rhsType )
          return( lhsDesc );
        else
          return( rhsDesc );
      }
      else if( lhsType == rhsType )
      {
        return lhsDesc;
      }
      else if ( isString( lhsType ) )
      {
        return lhsDesc;
      }
      else if ( isString( rhsType ) )
      {
        return rhsDesc;
      }
      else return 0;
    }
  };
};
