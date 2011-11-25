/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
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
    
    int ArrayImpl::compareData( void const *lhs, void const *rhs ) const
    {
      size_t lhsSize = getNumMembers( lhs );
      size_t rhsSize = getNumMembers( rhs );
      if ( lhsSize < rhsSize )
        return -1;
      else if ( lhsSize > rhsSize )
        return 1;
      else
      {
        for ( size_t i=0; i<lhsSize; ++i )
        {
          int memberResult = m_memberImpl->compareData( getMemberData( lhs, i ), getMemberData( rhs, i ) );
          if ( memberResult )
            return memberResult;
        }
        return 0;
      }
    }
  };
};
