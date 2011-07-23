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
      DataCallback dataCallback,
      EndCallback endCallback,
      FailureCallback failureCallback,
      RC::Handle<RC::Object> const &target
      )
      : m_dataCallback( dataCallback )
      , m_endCallback( endCallback )
      , m_failureCallback( failureCallback )
      , m_target( target )
    {
    }
    
    Stream::~Stream()
    {
    }
    
    void Stream::onData( std::string const &url, std::string const &mimeType, size_t offset, size_t size, void const *data )
    {
      FABRIC_ASSERT( m_target );
      m_dataCallback( url, mimeType, offset, size, data, m_target.makeStrong() );
    }

    void Stream::onEnd( std::string const &url, std::string const &mimeType )
    {
      FABRIC_ASSERT( m_target );
      m_endCallback( url, mimeType, m_target.makeStrong() );
      m_target = 0;
    }

    void Stream::onFailure( std::string const &url, std::string const &errorDesc )
    {
      FABRIC_ASSERT( m_target );
      m_failureCallback( url, errorDesc, m_target.makeStrong() );
      m_target = 0;
    }
  };
};
