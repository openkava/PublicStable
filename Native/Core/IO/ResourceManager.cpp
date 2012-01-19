/*
 *  Copyright 2010-2012 Fabric Technologies Inc. All rights reserved.
 */

#include <vector>

#include "ResourceManager.h"

#include <Fabric/Base/Exception.h>
#include <Fabric/Util/Timer.h>
#include <Fabric/Core/MT/LogCollector.h>

namespace Fabric
{
  namespace IO
  {
    struct PendingRequestInfo
    {
      ResourceClient* m_client;
      void* m_clientUserData;
      std::string m_url;
      Util::Timer m_lastProgressTimer;
      RC::WeakHandle< IO::ResourceManager > m_manager;
      std::list< void* >::iterator m_pendingRequestsListIter;
    };

    RC::Handle<ResourceManager> ResourceManager::Create( ScheduleAsynchCallbackFunc scheduleFunc, void *scheduleFuncUserData, float progressMaxFrequencySeconds )
    {
      return new ResourceManager( scheduleFunc, scheduleFuncUserData );
    }

    ResourceManager::ResourceManager( ScheduleAsynchCallbackFunc scheduleFunc, void *scheduleFuncUserData, float progressMaxFrequencySeconds )
      : m_scheduleFunc(scheduleFunc)
      , m_scheduleFuncUserData(scheduleFuncUserData)
      , m_progressMaxFrequencyMS( size_t( progressMaxFrequencySeconds*1000 ) )
    {
    }

    ResourceManager::~ResourceManager()
    {
      while( !m_pendingRequests.empty() )
      {
        PendingRequestInfo* requestInfo = (PendingRequestInfo*)m_pendingRequests.front();
        //Note: we don't delete the PendingRequestInfo* structs as there might be some pending asynch calls; leak instead of crash in this exceptional race condition.
        requestInfo->m_client->onFailure( "Resource request for \"" + requestInfo->m_url + "\" failed because of termination", requestInfo->m_clientUserData );
        requestInfo->m_client->release();
        m_pendingRequests.pop_front();
      }
    }

    ResourceManager::registerProvider( RC::Handle<ResourceProvider> const &provider, bool setAsDefault )
    {
      std::string scheme = provider->getUrlScheme();

      pair<SchemeToProviderMap::iterator, bool> result = m_schemeToProvider.insert( std::make_pair(scheme, provider) );
      if( result.second && result->first != provider )
        throw Exception( "Error: another resource provider is already registered for URL scheme " + scheme );

      if( setAsDefault )
      {
        if( m_defaultProvider && m_defaultProvider != provider )
          throw Exception( "Error: there is already another default provider " );
        m_defaultProvider = provider;
      }
    }

    void ResourceManager::get( char const *url, ResourceClient* client, bool getAsFile, void* userData )
    {
      PendingRequestInfo* requestInfo = new PendingRequestInfo();
      requestInfo->m_client = client;
      requestInfo->m_clientUserData = userData;
      requestInfo->m_manager = this;
      requestInfo->m_pendingRequestsListIter = m_pendingRequests.begin();
      requestInfo->m_url = url;
      requestInfo->m_lastProgressTimer.reset();

      client->retain();
      m_pendingRequests.push_front( requestInfo );

      try
      {
        ResourceProvider* provider = m_defaultProvider.ptr();
        char const *separator = url;
        int separatorPos = -1;
        while( *separator )
        {
          if( *separator == ':')
          {
            separatorPos = separator-url;
            break;
          }
          ++separator;
        }
        if( separatorPos != -1 )
        {
          std::string scheme(url, separatorPos);
          SchemeToProviderMap::iterator providerIt = m_schemeToProvider.find( scheme );
          if( providerIt != m_schemeToProvider.end() )
            provider = providerIt->second.ptr();
        }
        if( !provider )
          throw Exception( "No suitable provider registered" );
        else
          provider->get( url, this, getAsFile, requestInfo );
      }
      //Simplify error handling on client side: turn synch errors into asynch failure callbacks
      catch ( Exception e )
      {
        onFailureAsynchThreadCall( std::string("Error: " + e, requestInfo );
      }
      catch ( ... )
      {
        onFailureAsynchThreadCall( std::string("Unknown error", requestInfo );
      }
    }

    void ResourceManager::onCompletedRequest( void *userData )
    {
      PendingRequestInfo* requestInfo = (PendingRequestInfo*)userData;
      m_pendingRequests.erase( requestInfo->m_pendingRequestsListIter );
      requestInfo->m_client->release();
      delete requestInfo;
    }

    void ResourceManager::onProgress( char const *mimeType, size_t done, size_t total, void *userData )
    {
      PendingRequestInfo* requestInfo = (PendingRequestInfo*)userData;
      if( !requestInfo->m_manager )
        return;//Manager might have been destroyed, in which case we should ignore this call

      if( done < total)
      {
        int deltaMS = (int)requestInfo->m_lastProgressTimer.getElapsedMS(false);
        if( deltaMS < (int)m_progressMaxFrequencyMS )
          return;
      }
      requestInfo->m_lastProgressTimer.reset();

      try
      {
        requestInfo->m_client->onProgress( mimeType, done, total, requestInfo->m_clientUserData );
      }
      catch ( Exception e )
      {
        FABRIC_LOG( "ResourceManager: error while calling client's onProgress for \"" + requestInfo->m_url + "\": " + e.getDesc() );
      }
      catch ( ... )
      {
        FABRIC_LOG( "ResourceManager: error while calling client's onProgress for \"" + requestInfo->m_url + "\"." );
      }

      if(done == total)
        requestInfo->m_manager->onCompletedRequest( userData );
    }

    void ResourceManager::onData( size_t offset, size_t size, void const *data, void *userData )
    {
      PendingRequestInfo* requestInfo = (PendingRequestInfo*)userData;
      if( !requestInfo->m_manager )
        return;//Manager might have been destroyed, in which case we should ignore this call

      try
      {
        requestInfo->m_client->onData( offset, size, data, requestInfo->m_clientUserData );
      }
      catch ( Exception e )
      {
        FABRIC_LOG( "ResourceManager: error while calling client's onData for \"" + requestInfo->m_url + "\": " + e.getDesc() );
      }
      catch ( ... )
      {
        FABRIC_LOG( "ResourceManager: error while calling client's onData for \"" + requestInfo->m_url + "\"." );
      }
    }

    void ResourceManager::onFile( char const *fileName, void *userData )
    {
      PendingRequestInfo* requestInfo = (PendingRequestInfo*)userData;
      if( !requestInfo->m_manager )
        return;//Manager might have been destroyed, in which case we should ignore this call

      try
      {
        requestInfo->m_client->onFile( fileName, requestInfo->m_clientUserData );
      }
      catch ( Exception e )
      {
        FABRIC_LOG( "ResourceManager: error while calling client's onFile for \"" + requestInfo->m_url + "\": " + e.getDesc() );
      }
      catch ( ... )
      {
        FABRIC_LOG( "ResourceManager: error while calling client's onFile for \"" + requestInfo->m_url + "\"." );
      }
    }

    void ResourceManager::onFailure( char const *errorDesc, void *userData )
    {
      PendingRequestInfo* requestInfo = (PendingRequestInfo*)userData;
      if( !requestInfo->m_manager )
        return;//Manager might have been destroyed, in which case we should ignore this call

      try
      {
        requestInfo->m_client->onFailure( std::string( "Error while processing request for URL \"") + requestInfo->m_url + "\": " + errorDesc, requestInfo->m_clientUserData );
      }
      catch ( Exception e )
      {
        FABRIC_LOG( "ResourceManager: error while calling client's onFailure for \"" + requestInfo->m_url + "\": " + e.getDesc() );
      }
      catch ( ... )
      {
        FABRIC_LOG( "ResourceManager: error while calling client's onFailure for \"" + requestInfo->m_url + "\"." );
      }

      requestInfo->m_manager->onCompletedRequest( userData );
    }

    struct OnProgressCallbackStruct
    {
      std::string m_mimeType;
      size_t m_done;
      size_t m_total;
      void *m_userData;
    };

    void ResourceManager::OnProgressSynchCallback( void *userData )
    {
      OnProgressCallbackStruct* callStruct = (OnProgressCallbackStruct*)userData;
      ResourceManager::onProgress( callStruct->m_mimeType.c_str(), callStruct->m_done, callStruct->m_total, callStruct->m_userData );
      delete callStruct;
    }

    void ResourceManager::onProgressAsynchThreadCall( char const *mimeType, size_t done, size_t total, void *userData )
    {
      OnProgressCallStruct* callStruct = new OnProgressCallbackStruct();
      callStruct->m_mimeType = mimeType;
      callStruct->m_done = done;
      callStruct->m_total = total;
      callStruct->m_userData = userData;
      (*m_scheduleFunc)(m_scheduleFuncUserData, OnProgressSynchCallback, callStruct);
    }

    struct OnDataCallbackStruct
    {
      size_t m_offset;
      std::vector<char> m_data;
      void *m_userData;
    };

    void ResourceManager::OnDataSynchCallback( void *userData )
    {
      OnDataCallbackStruct* callStruct = (OnDataCallbackStruct*)userData;
      ResourceManager::onData( callStruct->m_offset, callStruct->m_data.size(), callStruct->m_data.empty() ? NULL : &callStruct->m_data.front(), callStruct->m_userData );
      delete callStruct;
    }

    void ResourceManager::onDataAsynchThreadCall( size_t offset, size_t size, void const *data, void *userData )
    {
      OnDataCallStruct* callStruct = new OnDataCallbackStruct();
      callStruct->m_offset = offset;
      if(size)
      {
        callStruct->m_data.resize(size);
        memcpy( &callStruct->m_data.front(), data, size );
      }
      callStruct->m_userData = userData;
      (*m_scheduleFunc)(m_scheduleFuncUserData, OnDataSynchCallback, callStruct);
    }

    struct OnStringCallbackStruct
    {
      std::string m_string;
      void *m_userData;
    };

    void ResourceManager::OnFileSynchCallback( void *userData )
    {
      OnStringCallbackStruct* callStruct = (OnStringCallbackStruct*)userData;
      ResourceManager::onFile( callStruct->m_string.c_str(), callStruct->m_userData );
      delete callStruct;
    }

    void ResourceManager::onFileAsynchThreadCall( char const *fileName, void *userData )
    {
      OnStringCallbackStruct* callStruct = new OnStringCallbackStruct();
      callStruct->m_string = fileName;
      callStruct->m_userData = userData;
      (*m_scheduleFunc)(m_scheduleFuncUserData, OnFileSynchCallback, callStruct);
    }

    void ResourceManager::OnFailureSynchCallback( void *userData )
    {
      OnStringCallbackStruct* callStruct = (OnStringCallbackStruct*)userData;
      ResourceManager::onFailure( callStruct->m_string.c_str(), callStruct->m_userData );
      delete callStruct;
    }

    void ResourceManager::onFailureAsynchThreadCall( char const *errorDesc, void *userData )
    {
      OnStringCallbackStruct* callStruct = new OnStringCallbackStruct();
      callStruct->m_string = errorDesc;
      callStruct->m_userData = userData;
      (*m_scheduleFunc)(m_scheduleFuncUserData, OnFailureSynchCallback, callStruct);
    }
};
