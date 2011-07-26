/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_NPAPI_IO_MANAGER_H
#define _FABRIC_NPAPI_IO_MANAGER_H

#include <Fabric/Core/IO/Manager.h>

#include <npapi/npapi.h>

namespace Fabric
{
  namespace MT
  {
    class LogCollector;
  };
  
  namespace NPAPI
  {
    class IOManager : public IO::Manager
    {
    public:
    
      static RC::Handle<IOManager> Create( NPP npp );
    
      virtual RC::Handle<IO::Stream> createStream(
        std::string const &url,
        IO::Stream::DataCallback dataCallback,
        IO::Stream::EndCallback endCallback,
        IO::Stream::FailureCallback failureCallback,
        RC::Handle<RC::Object> const &target,
        void *userData
        ) const;
    
      NPError nppNewStream( NPP npp, NPMIMEType type, NPStream *stream, NPBool seekable, uint16_t *stype );
      int32_t nppWriteReady( NPP npp, NPStream* stream );
      int32_t nppWrite( NPP npp, NPStream* stream, int32_t offset, int32_t len, void* buffer );
      NPError nppDestroyStream( NPP npp, NPStream *stream, NPReason reason );
    
    protected:
    
      IOManager( NPP npp );
      
    private:
    
      NPP m_npp;
    };
  };
};

#endif //_FABRIC_NPAPI_IO_MANAGER_H
