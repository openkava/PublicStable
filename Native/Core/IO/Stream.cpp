/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/IO/Stream.h>
#include <Fabric/Core/Util/Assert.h>

namespace Fabric
{
  namespace IO
  {
    Stream::Stream(
      SuccessCallback successCallback,
      FailureCallback failureCallback,
      RC::Handle<RC::Object> const &target
      )
      : m_successCallback( successCallback )
      , m_failureCallback( failureCallback )
      , m_target( target )
    {
    }
    
    Stream::~Stream()
    {
    }
    
    void Stream::indicateSuccess( std::string const &url, std::string const &mimeType, std::string const &filename )
    {
      FABRIC_ASSERT( m_target );
      m_successCallback( url, mimeType, filename, m_target.makeStrong() );
      m_target = 0;
    }
    
    void Stream::indicateFailure( std::string const &url, std::string const &errorDesc )
    {
      FABRIC_ASSERT( m_target );
      m_failureCallback( url, errorDesc, m_target.makeStrong() );
      m_target = 0;
    }
  };
};
