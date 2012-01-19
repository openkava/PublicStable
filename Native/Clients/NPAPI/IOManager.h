/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_NPAPI_IO_MANAGER_H
#define _FABRIC_NPAPI_IO_MANAGER_H

#include <Fabric/Core/IO/Manager.h>

#include <npapi/npapi.h>

namespace Fabric
{
  namespace NPAPI
  {
    class Context;
    class HTTPResourceProvider;

    class IOManager : public IO::Manager
    {
    public:
    
      static RC::Handle<IOManager> Create( NPP npp );
    
      virtual std::string queryUserFilePath(
        bool existingFile,
        std::string const &title,
        std::string const &defaultFilename,
        std::string const &extension
        ) const;

      IOManager( NPP npp );
      void setContext( RC::Handle<Context> const &context );

      RC::Handle< HTTPResourceProvider > getHTTPResourceProvider() const;
      
    private:
    
      NPP m_npp;
      RC::Handle< HTTPResourceProvider > m_httpResourceProvider;
      Context* m_context;
    };
  };
};

#endif //_FABRIC_NPAPI_IO_MANAGER_H
