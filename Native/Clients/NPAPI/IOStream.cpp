/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Clients/NPAPI/IOStream.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Core/Util/Assert.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/MT/LogCollector.h>

namespace Fabric
{
  namespace NPAPI
  {
    RC::Handle<IOStream> IOStream::Create(
      NPP npp,
      std::string const &url,
      SuccessCallback successCallback,
      FailureCallback failureCallback,
      RC::Handle<RC::Object> const &target
      )
    {
      return new IOStream( npp, url, successCallback, failureCallback, target );
    }
  
    IOStream::IOStream(
      NPP npp,
      std::string const &url,
      SuccessCallback successCallback,
      FailureCallback failureCallback,
      RC::Handle<RC::Object> const &target
      )
      : IO::Stream( successCallback, failureCallback, target )
      , m_npp( npp )
      , m_url( url )
      , m_started( false )
      , m_finished( false )
    {
    }
  
    void IOStream::start()
    {
      if ( !m_started )
      {
        retain();
        NPError npError = NPN_GetURLNotify( m_npp, m_url.c_str(), 0, this );
        if ( npError != NPERR_NO_ERROR )
          throw Exception( "stream " + _(m_url) + ": error calling NPN_GetURLNotify()" );
        m_started = true;
      }
    }
  
    NPError IOStream::nppNewStream( NPP npp, NPMIMEType mimeType, NPStream *stream, NPBool seekable, uint16_t *stype )
    {
      FABRIC_ASSERT( m_started );
      FABRIC_ASSERT( !m_finished );
      FABRIC_ASSERT( npp == m_npp );
      m_mimeType = mimeType;
      FABRIC_ASSERT( stream->notifyData == this );
      *stype = NP_ASFILEONLY;
      return NPERR_NO_ERROR;
    }
      
    void IOStream::nppStreamAsFile( NPP npp, NPStream *stream, const char *filename )
    {
      FABRIC_ASSERT( m_started );
      FABRIC_ASSERT( !m_finished );
      FABRIC_ASSERT( npp == m_npp );
      FABRIC_ASSERT( stream->notifyData == this );
      m_filename = filename;
      try
      {
        indicateSuccess( m_url, m_mimeType, m_filename );
      }
      catch ( Exception e )
      {
        FABRIC_LOG( e.getDesc() );
      }
    }
    
    NPError IOStream::nppDestroyStream( NPP npp, NPStream *stream, NPReason reason )
    {
      FABRIC_ASSERT( m_started );
      FABRIC_ASSERT( !m_finished );
      FABRIC_ASSERT( npp == m_npp );
      FABRIC_ASSERT( stream->notifyData == this );
      try
      {
        switch ( reason )
        {
          case NPRES_DONE:
            break;
          case NPRES_USER_BREAK:
            indicateFailure( m_url, "cancelled at user request" );
            break;
          case NPRES_NETWORK_ERR:
            indicateFailure( m_url, "network error" );
            break;
        }
      }
      catch ( Exception e )
      {
        FABRIC_LOG( e.getDesc() );
      }
      m_finished = true;
      release();
      return NPERR_NO_ERROR;
    }
  };
};
