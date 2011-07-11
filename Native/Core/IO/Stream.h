/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_IO_STREAM_H
#define _FABRIC_IO_STREAM_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/WeakHandle.h>

#include <string>

namespace Fabric
{
  namespace IO
  {
    class Stream : public RC::Object
    {
    public:
    
      typedef void (*SuccessCallback)( std::string const &url, std::string const &mimeType, std::string const &filename, RC::Handle<RC::Object> const &target );
      typedef void (*FailureCallback)( std::string const &url, std::string const &errorDesc, RC::Handle<RC::Object> const &target );
      
      virtual void start() = 0;
    
    protected:
    
      Stream(
        SuccessCallback successCallback,
        FailureCallback failureCallback,
        RC::Handle<RC::Object> const &target
        );
      ~Stream();
      
      void indicateSuccess( std::string const &url, std::string const &mimeType, std::string const &filename );
      void indicateFailure( std::string const &url, std::string const &errorDesc );
    
    private:
    
      SuccessCallback m_successCallback;
      FailureCallback m_failureCallback;
      RC::WeakHandle<RC::Object> m_target;
    };
  };
};

#endif //_FABRIC_IO_STREAM_H
