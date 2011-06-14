/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_JSON_COMMAND_CHANNEL_H
#define _FABRIC_JSON_COMMAND_CHANNEL_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/ConstHandle.h>

#include <string>
#include <vector>

namespace Fabric
{
  namespace JSON
  {
    class Value;
    
    class CommandChannel : public RC::Object
    {
    public:
    
      virtual void jsonNotify( std::vector<std::string> const &src, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg ) = 0;
    };
  };
};

#endif //_FABRIC_JSON_COMMAND_CHANNEL_H
