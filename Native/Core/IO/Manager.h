/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_IO_MANAGER_H
#define _FABRIC_IO_MANAGER_H

#include <Fabric/Core/IO/Stream.h>

namespace Fabric
{
  namespace IO
  {
    class Manager : public RC::Object
    {
    public:
    
      virtual RC::Handle<Stream> createStream(
        std::string const &url,
        Stream::DataCallback dataCallback,
        Stream::EndCallback endCallback,
        Stream::FailureCallback failureCallback,
        RC::Handle<RC::Object> const &target
        ) const = 0;
    };
  };
};

#endif //_FABRIC_IO_MANAGER_H
