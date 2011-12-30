/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_DG_FABRICFILEHANDLE_H
#define _FABRIC_DG_FABRICFILEHANDLE_H

#include <Fabric/Base/RC/ConstHandle.h>
#include <string>

namespace Fabric
{
  namespace RT
  {
    class Manager;
    class StructDesc;
  };

  namespace DG
  {
    void RegisterFabricFileHandleType( RC::Handle<RT::Manager> const &rtManager );
    RC::ConstHandle<RT::StructDesc> getFabricFileHandleDesc( RC::ConstHandle<RT::Manager> const &rtManager );

    void FabricFileHandleCopy( void **dst, void const *src );
    void FabricFileHandleDelete( void **data );
    bool FabricFileHandleSetFromPath( void **data, char const *pathData, size_t pathLength, bool readWriteAccess );
    char const *FabricFileHandleGetFullPath( void const *data );
    bool FabricFileHandleHasReadWriteAccess( void const *data );
  };
};

#endif //_FABRIC_DG_FABRICFILEHANDLE_H
