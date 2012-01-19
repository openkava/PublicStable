/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Base/Exception.h>
#include <Fabric/Base/RC/WeakHandle.h>
#include <Fabric/Core/Util/Assert.h>

namespace Fabric
{
  namespace NPAPI
  {
    RC::Handle<IOManager> HTTPResourceProvider::Create( NPP npp )
    {
      return new HTTPResourceProvider( npp );
    }

    HTTPResourceProvider::HTTPResourceProvider( NPP npp )
      : m_npp( npp )
    {
    }

    struct HTTPStream
    {
      std::string m_mimeType;
      bool m_asFile;
      void* m_userData;
      size_t m_nbReceived;
    };

    char const *HTTPResourceProvider::getUrlScheme() const
    {
      return "http";//More is supported (like ftp), but we should be the default resource provider so all these should come our way
    }

    virtual void HTTPResourceProvider::get( char const *url, bool getAsFile, void* userData )
    {
      //Don't validate the url ourselves; the browser supports multiple schemes, relative paths, etc.
      HTTPStream *requestStruct = new HTTPStream();
      requestStruct->m_asFile = getAsFile;
      requestStruct->m_userData = userData;
      requestStruct->m_nbReceived = 0;

      NPError npError = NPN_GetURLNotify( m_npp, url, 0, requestStruct );
      if ( npError != NPERR_NO_ERROR )
        throw Exception( "Error calling NPN_GetURLNotify()" );
    }

    NPError IOManager::nppNewStream( NPP npp, NPMIMEType type, NPStream *stream, NPBool seekable, uint16_t *stype )
    {
      FABRIC_ASSERT( npp == m_npp );

      HTTPStream *streamStruct = (HTTPStream*)stream->notifyData;
      stream->m_mimeType = mimeType;
      *stype = stream->m_asFile ? NP_ASFILE : NP_NORMAL;
      return NPERR_NO_ERROR;
    }
    
    int32_t IOManager::nppWriteReady( NPP npp, NPStream* stream )
    {
      FABRIC_ASSERT( npp == m_npp );
      return 1<<17;//<= 128K buffers
    }

    int32_t IOManager::nppWrite( NPP npp, NPStream* stream, int32_t offset, int32_t len, void* buffer )
    {
      FABRIC_ASSERT( npp == m_npp );

      HTTPStream *streamStruct = (HTTPStream*)stream->notifyData;
      if( streamStruct->m_asFile )
        ResourceManager::onData( offset, len, buffer, streamStruct->m_userData );

      streamStruct->m_nbReceived += len;
      size_t total = stream->end;

      //Note: ensure that onProgress(100%) is issued after data is ready; if m_asFile we wait after the onFile call
      if( !streamStruct->m_asFile || streamStruct->m_nbReceived != total )
        ResourceManager::onProgress( streamStruct->m_mimeType.c_str(), streamStruct->m_nbReceived, total, streamStruct->m_userData );

      return len;
    }
    
    NPError IOManager::nppDestroyStream( NPP npp, NPStream *stream, NPReason reason )
    {
      FABRIC_ASSERT( npp == m_npp );

      HTTPStream *streamStruct = (HTTPStream*)stream->notifyData;
      switch ( reason )
      {
        case NPRES_DONE:
          //onProgress(100%) callback has already been called
          break;
        case NPRES_USER_BREAK:
          ResourceManager::onFailure( "Cancelled by user", streamStruct->m_userData );
          break;
        case NPRES_NETWORK_ERR:
          ResourceManager::onFailure( "Network error", streamStruct->m_userData );
          break;
      }
      return NPERR_NO_ERROR;
    }

    NPError IOManager::nppStreamAsFile( NPP npp, NPStream *stream, const char* fname )
    {
      FABRIC_ASSERT( npp == m_npp );

      HTTPStream *streamStruct = (HTTPStream*)stream->notifyData;
      ResourceManager::onFile( fname, streamStruct->m_userData );
      size_t total = stream->end;
      ResourceManager::onProgress( requestStruct->m_mimeType.c_str(), total, total, streamStruct->m_userData );
      return NPERR_NO_ERROR;
    }
  };
};
