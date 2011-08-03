/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/KL/SourceRange.h>
#include <Fabric/Core/KL/SourceReader.h>

namespace Fabric
{
  namespace KL
  {
    std::string SourceRange::toString() const
    {
      SourceReader sourceReader( m_source, m_startLocation );
      std::string result;
      while ( sourceReader.getLocationForEnd().getIndex() < m_endLocation.getIndex() )
        result += sourceReader.advance();
      return result;
    }
  };
};

