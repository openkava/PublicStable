/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_NPAPI_IO_STREAM_H
#define _FABRIC_NPAPI_IO_STREAM_H

#include <Fabric/Core/IO/Stream.h>

#include <npapi/npapi.h>

namespace Fabric
{
  namespace MT
  {
    class LogCollector;
  };
  
  namespace NPAPI
  {
    class IOStream : public IO::Stream
    {
    public:
    
      static RC::Handle<IOStream> Create(
        NPP npp,
        std::string const &url,
        DataCallback dataCallback,
        EndCallback endCallback,
        FailureCallback failureCallback,
        RC::Handle<RC::Object> const &target,
        void *userData
        );
      
      NPError nppNewStream( NPP npp, NPMIMEType type, NPStream *stream, NPBool seekable, uint16_t *stype );
      int32_t nppWriteReady( NPP npp, NPStream* stream );
      int32_t nppWrite( NPP npp, NPStream* stream, int32_t offset, int32_t len, void* buffer );
      NPError nppDestroyStream( NPP npp, NPStream *stream, NPReason reason );
    
    protected:
    
      IOStream(
        NPP npp,
        std::string const &url,
        DataCallback dataCallback,
        EndCallback endCallback,
        FailureCallback failureCallback,
        RC::Handle<RC::Object> const &target,
        void *userData
        );
    
    private:
    
      NPP m_npp;
      std::string m_url;
      std::string m_mimeType;
      bool m_finished;
    };
  };
};

#endif //_FABRIC_NPAPI_IO_MANAGER_H
