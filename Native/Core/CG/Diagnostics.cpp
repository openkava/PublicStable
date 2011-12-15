/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/CG/Diagnostics.h>

namespace Fabric
{
  namespace CG
  {
    Diagnostics::Diagnostics( Diagnostics const &that )
    {
      append( that );
    }
    
    void Diagnostics::append( Diagnostics const &that )
    {
      for ( Impl::const_iterator it = that.m_impl.begin(); it != that.m_impl.end(); ++it )
        m_impl.push_back( *it );
    }
  }
}
