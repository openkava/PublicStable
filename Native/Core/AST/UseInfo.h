/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_USE_INFO_H
#define _FABRIC_AST_USE_INFO_H

#include <Fabric/Core/CG/Location.h>

#include <string>
#include <map>

namespace Fabric
{
  namespace AST
  {
    class UseNameToLocationMap : public std::map< std::string, CG::Location >
    {
    public:
    
      UseNameToLocationMap()
      {
      }
    };
  };
};

#endif //_FABRIC_AST_USE_INFO_H
