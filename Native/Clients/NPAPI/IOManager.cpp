/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Clients/NPAPI/IOManager.h>
#include <Fabric/Clients/NPAPI/IOStream.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Core/Util/Assert.h>
#include <Fabric/Core/MT/LogCollector.h>

namespace Fabric
{
  namespace NPAPI
  {
    RC::Handle<IOManager> IOManager::Create( NPP npp )
    {
      return new IOManager( npp );
    }
  
    IOManager::IOManager( NPP npp )
      : m_npp( npp )
    {
    }
    
    RC::Handle<IO::Stream> IOManager::createStream(
      std::string const &url,
      IO::Stream::DataCallback dataCallback,
      IO::Stream::EndCallback endCallback,
      IO::Stream::FailureCallback failureCallback,
      RC::Handle<RC::Object> const &target,
      void *userData
      ) const
    {
      return IOStream::Create( m_npp, url, dataCallback, endCallback, failureCallback, target, userData );
    }
  
    NPError IOManager::nppNewStream( NPP npp, NPMIMEType type, NPStream *stream, NPBool seekable, uint16_t *stype )
    {
      FABRIC_ASSERT( npp == m_npp );
      return static_cast<IOStream *>( stream->notifyData )->nppNewStream( npp, type, stream, seekable, stype );
    }
    
    int32_t IOManager::nppWriteReady( NPP npp, NPStream* stream )
    {
      FABRIC_ASSERT( npp == m_npp );
      return static_cast<IOStream *>( stream->notifyData )->nppWriteReady( npp, stream );
    }

    int32_t IOManager::nppWrite( NPP npp, NPStream* stream, int32_t offset, int32_t len, void* buffer )
    {
      FABRIC_ASSERT( npp == m_npp );
      return static_cast<IOStream *>( stream->notifyData )->nppWrite( npp, stream, offset, len, buffer );
    }
    
    NPError IOManager::nppDestroyStream( NPP npp, NPStream *stream, NPReason reason )
    {
      FABRIC_ASSERT( npp == m_npp );
      return static_cast<IOStream *>( stream->notifyData )->nppDestroyStream( npp, stream, reason );
    }
  };
};
