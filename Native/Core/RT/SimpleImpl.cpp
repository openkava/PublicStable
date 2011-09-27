#include "SimpleImpl.h"

namespace Fabric
{
  namespace RT
  {
    void SimpleImpl::disposeDataImpl( void *data ) const
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
