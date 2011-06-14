/*
 *
 *  Created by Peter Zion on 10-11-11.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */

#include "LogCollector.h"

namespace Fabric
{
  namespace CLI
  {
    RC::Handle<LogCollector> LogCollector::Create()
    {
      return new LogCollector();
    }
    
    LogCollector::LogCollector()
    {
    }
    
    LogCollector::~LogCollector()
    {
    }
    
    void LogCollector::logString( char const *data, size_t length )
    {
    }
  };
};
