/*
 *
 *  Created by Peter Zion on 10-08-19.
 *  Copyright 2010 Fabric 3D Inc.. All rights reserved.
 *
 */

#ifndef _FABRIC_OCL_H
#define _FABRIC_OCL_H

#include <Fabric/Base/RC/Handle.h>

#include <string>

namespace Fabric
{
  namespace RT
  {
    class Manager;
  };
  
  namespace CG
  {
    class ModuleBuilder;
  };
  
  namespace OCL
  {
    void registerTypes( RC::Handle<RT::Manager> const &rtManager );
    void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, RC::Handle<RT::Manager> const &rtManager );
    void *llvmResolveExternalFunction( std::string const &name );
  };
};

#endif //_FABRIC_OCL_H
