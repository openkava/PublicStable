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
      bool asFile,
      DataCallback dataCallback,
      EndCallback endCallback,
      FailureCallback failureCallback,
      RC::Handle<RC::Object> const &target,
      void *userData
      )
    {
      return new IOStream( npp, url, asFile, dataCallback, endCallback, failureCallback, target, userData );
    }
  
    IOStream::IOStream(
      NPP npp,
      std::string const &url,
      bool asFile,
      DataCallback dataCallback,
      EndCallback endCallback,
      FailureCallback failureCallback,
      RC::Handle<RC::Object> const &target,
      void *userData
      )
      : IO::Stream( dataCallback, endCallback, failureCallback, target, userData )
      , m_npp( npp )
      , m_url( url )
      , m_finished( false )
      , m_asFile( asFile )
    {
        retain();
        NPError npError = NPN_GetURLNotify( m_npp, m_url.c_str(), 0, this );
        if ( npError != NPERR_NO_ERROR )
          throw Exception( "stream " + _(m_url) + ": error calling NPN_GetURLNotify()" );
    }
  
    NPError IOStream::nppNewStream( NPP npp, NPMIMEType mimeType, NPStream *stream, NPBool seekable, uint16_t *stype )
    {
      FABRIC_ASSERT( !m_finished );
      FABRIC_ASSERT( npp == m_npp );
      m_mimeType = mimeType;
      FABRIC_ASSERT( stream->notifyData == this );
      *stype = m_asFile ? NP_ASFILE : NP_NORMAL;
      return NPERR_NO_ERROR;
    }

    int32_t IOStream::nppWriteReady( NPP npp, NPStream* stream )
    {
      FABRIC_ASSERT( !m_finished );
      FABRIC_ASSERT( npp == m_npp );
      FABRIC_ASSERT( stream->notifyData == this );
      return 1<<16;//<= 64K buffers
    }

    int32_t IOStream::nppWrite( NPP npp, NPStream* stream, int32_t offset, int32_t len, void* buffer )
    {
      FABRIC_ASSERT( !m_finished );
      FABRIC_ASSERT( npp == m_npp );
      FABRIC_ASSERT( stream->notifyData == this );
      try
      {
        onData( m_url, m_mimeType, stream->end, offset, len, buffer );
      }
      catch ( Exception e )
      {
        FABRIC_LOG( e.getDesc() );
      }
      return len;
    }
    
    NPError IOStream::nppDestroyStream( NPP npp, NPStream *stream, NPReason reason )
    {
      if(m_finished && m_asFile)//Might happen if nppStreamAsFile was called before
        return NPERR_NO_ERROR;

      FABRIC_ASSERT( !m_finished );
      FABRIC_ASSERT( npp == m_npp );
      FABRIC_ASSERT( stream->notifyData == this );
      try
      {
        switch ( reason )
        {
          case NPRES_DONE:
            if(m_asFile)
              return NPERR_NO_ERROR;//Will call onEnd under nppStreamAsFile
            onEnd( m_url, m_mimeType, NULL );
            break;
          case NPRES_USER_BREAK:
            onFailure( m_url, "cancelled at user request" );
            m_asFile = false;//Play safe; ignore further nppStreamAsFile calls
            break;
          case NPRES_NETWORK_ERR:
            onFailure( m_url, "network error" );
            m_asFile = false;//Play safe; ignore further nppStreamAsFile calls
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

    NPError IOStream::nppStreamAsFile( NPP npp, NPStream *stream, const char* fname )
    {
      FABRIC_ASSERT( !m_finished );
      FABRIC_ASSERT( m_asFile );
      std::string filename(fname);
      onEnd( m_url, m_mimeType, &filename );
      m_finished = true;
      release();
      return NPERR_NO_ERROR;
    }
  };
};
