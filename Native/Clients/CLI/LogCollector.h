/*
 *
 *  Created by Peter Zion on 10-11-11.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_CLI_LOG_COLLECTOR_H
#define _FABRIC_CLI_LOG_COLLECTOR_H

#include <Fabric/Core/MT/LogCollector.h>

namespace Fabric
{
  namespace CLI
  {
    class LogCollector : public MT::LogCollector
    {
    public:
    
      static RC::Handle<LogCollector> Create();
    
    protected:
    
      LogCollector();
      ~LogCollector();
      
      virtual void logString( char const *data, size_t length );
      
    private:
    
    };
  };
};

#endif //_FABRIC_CLI_LOG_COLLECTOR_H
