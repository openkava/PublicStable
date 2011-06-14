#include "ArrayImpl.h"

#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Core/Util/Decoder.h>

namespace Fabric
{
  

  namespace RT
  {
    std::string ArrayImpl::descData( void const *data ) const
    {
      std::string result = "[";
      size_t numMembers = getNumMembers( data );
      size_t numMembersToDisplay = numMembers;
      if ( numMembersToDisplay > 4 )
        numMembersToDisplay = 4;
      for ( size_t i=0; i<numMembersToDisplay; ++i )
      {
        if ( result.length() > 1 )
          result += ",";
        result += m_memberImpl->descData( getMemberData( data, i ) );
      }
      if ( numMembers > numMembersToDisplay )
        result += "...";
      result += "]";
      return result;
    }
  };
};
