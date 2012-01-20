/*
 *  Copyright 2010-2012 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_IO_RESOURCEMANAGER_H
#define _FABRIC_IO_RESOURCEMANAGER_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/ConstHandle.h>
#include <map>
#include <list>
#include <string>

namespace Fabric
{

  namespace IO
  {
    class ResourceManager;

    struct ResourceClient
    {
      //Note: the last issued call is either onProgress() with 100% done or an onFailure()
      virtual void onProgress( char const *mimeType, size_t done, size_t total, void *userData ) = 0;
      virtual void onData( size_t offset, size_t size, void const *data, void *userData ) = 0;
      virtual void onFile( char const *fileName, void *userData ) = 0;
      virtual void onFailure( char const *errorDesc, void *userData ) = 0;

      virtual void retain() const = 0;
      virtual void release() const = 0;
    };

    typedef void (*ScheduleAsynchCallbackFunc)( void* scheduleUserData, void (*callbackFunc)(void *), void *callbackFuncUserData );

    class ResourceProvider : public RC::Object
    {
    public:
      virtual char const * getUrlScheme() const = 0;
      virtual void get( char const *url, bool getAsFile, void* userData ) = 0;
    };

    class ResourceManager : public RC::Object
    {
    public:

      static RC::Handle<ResourceManager> Create( ScheduleAsynchCallbackFunc scheduleFunc, void *scheduleFuncUserData, float progressMaxFrequencySeconds = 0.3f );

      void registerProvider( RC::Handle<ResourceProvider> const &provider, bool setAsDefault = false );
      void get( char const *url, ResourceClient* client, bool getAsFile, void* userData );

      static void onProgress( char const *mimeType, size_t done, size_t total, void *userData );
      static void onData( size_t offset, size_t size, void const *data, void *userData );
      static void onFile( char const *fileName, void *userData );
      static void onFailure( char const *errorDesc, void *userData );

      //Asynch, inter-thread call scheduling helpers
      static void onProgressAsynchThreadCall( char const *mimeType, size_t done, size_t total, void *userData );
      static void onDataAsynchThreadCall( size_t offset, size_t size, void const *data, void *userData );
      static void onFileAsynchThreadCall( char const *fileName, void *userData );
      static void onFailureAsynchThreadCall( char const *errorDesc, void *userData );

    private:

      ResourceManager( ScheduleAsynchCallbackFunc scheduleFunc, void *scheduleFuncUserData, float progressMaxFrequencySeconds );
      ~ResourceManager();

      void onCompletedRequest( void *userData );

      typedef std::map<std::string, RC::Handle<ResourceProvider> > SchemeToProviderMap;
      SchemeToProviderMap m_schemeToProvider;

      RC::Handle<ResourceProvider> m_defaultProvider;

      std::list< void* > m_pendingRequests;

      ScheduleAsynchCallbackFunc m_scheduleFunc;
      void *m_scheduleFuncUserData;
      size_t m_progressMaxFrequencyMS;
    };
  };
};

#endif //_FABRIC_IO_RESOURCEMANAGER_H
