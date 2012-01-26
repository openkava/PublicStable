/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/RT/ProducerImpl.h>

namespace Fabric
{
  namespace RT
  {
    ProducerImpl::ProducerImpl( std::string const &codeName, ImplType type )
      : Impl( codeName, type )
    {
    }
    
    bool ProducerImpl::isExportable() const
    {
      return false;
    }
  }
}
