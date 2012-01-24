/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Clients/NPAPI/IOManager.h>
#include <Fabric/Clients/NPAPI/Context.h>
#include <Fabric/Core/IO/FileHandleManager.h>
#include <Fabric/Core/IO/FileHandleResourceProvider.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Core/Util/Assert.h>
#include <npapi/npapi.h>
#include "HTTPResourceProvider.h"

namespace Fabric
{
  namespace NPAPI
  {
    RC::Handle<IOManager> IOManager::Create( NPP npp )
    {
      return new IOManager( npp );
    }

    void ScheduleAsynchCallback( void* scheduleUserData, void (*callbackFunc)(void *), void *callbackFuncUserData )
    {
      NPN_PluginThreadAsyncCall( (NPP)scheduleUserData, callbackFunc, callbackFuncUserData );
    }
  
    IOManager::IOManager( NPP npp )
      : IO::Manager( ScheduleAsynchCallback, npp )
      , m_npp( npp )
      , m_httpResourceProvider( HTTPResourceProvider::Create( npp ) )
      , m_context( NULL )
    {
      getResourceManager()->registerProvider( RC::Handle<IO::ResourceProvider>::StaticCast( m_httpResourceProvider ), true );
    }

    RC::Handle< HTTPResourceProvider > IOManager::getHTTPResourceProvider() const
    {
      return m_httpResourceProvider;
    }

    void IOManager::setContext( RC::Handle<Context> const &context )
    {
      m_context = context.ptr();
    }

    std::string IOManager::queryUserFilePath( bool existingFile, std::string const &title, std::string const &defaultFilename, std::string const &extension ) const
    {
      FABRIC_ASSERT( m_context );
      return m_context->queryUserFilePath( existingFile, title, defaultFilename, extension );
    }
  };
};
