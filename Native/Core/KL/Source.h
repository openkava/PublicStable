/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_SOURCE_H
#define _FABRIC_KL_SOURCE_H

#include <Fabric/Base/RC/Object.h>

namespace Fabric
{
  namespace KL
  {
    class Source : public RC::Object
    {
    public:
    
      virtual char const *data() const = 0;
      virtual size_t length() const = 0;
    };
  };
};

#endif //_FABRIC_KL_SOURCE_H
