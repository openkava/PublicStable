#include "SimpleImpl.h"

namespace Fabric
{
  namespace RT
  {
    void SimpleImpl::disposeData( void *data ) const
    {
    }
    
    bool SimpleImpl::isEquivalentTo( RC::ConstHandle<Impl> const &impl ) const
    {
      return getType() == impl->getType();
    }
    
    bool SimpleImpl::isShallow() const
    {
      return true;
    }
  };
};
