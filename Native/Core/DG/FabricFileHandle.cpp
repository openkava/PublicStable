/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/DG/FabricFileHandle.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/RT/StructDesc.h>
#include <Fabric/Core/RT/OpaqueDesc.h>
#include <Fabric/Core/IO/Helpers.h>
#include <Fabric/Base/Util/Assert.h>

namespace Fabric
{
  namespace DG
  {
    void RegisterFabricFileHandleType( RC::Handle<RT::Manager> const &rtManager )
    {
      //[JeromeCG 20111222] Note: we define a FileHandle as a struct of 1 data member instead of an alias to data so we can 
      // eventually add KL member functions to it (eg: ability to read/write bytes)
      RT::StructMemberInfoVector memberInfos;
      memberInfos.resize(1);

      memberInfos[0].name = "data";
      memberInfos[0].desc = rtManager->getDataDesc();

      rtManager->registerStruct( "FabricFileHandle", memberInfos );
    }

    RC::ConstHandle<RT::StructDesc> getFabricFileHandleDesc( RC::ConstHandle<RT::Manager> const &rtManager )
    {
      RC::ConstHandle<RT::Desc> fabricFileHandleDesc = rtManager->getDesc("FabricFileHandle");
      FABRIC_ASSERT( fabricFileHandleDesc && RT::isStruct( fabricFileHandleDesc->getType() ) );
      return RC::ConstHandle<RT::StructDesc>::StaticCast( fabricFileHandleDesc );
    }

    struct FabricFileHandleInternalData {
      std::string m_fullPath;
      bool m_readWriteAccess;

      FabricFileHandleInternalData() 
        : m_readWriteAccess(false)
      {}

      bool IsValid()
      {
        return !m_fullPath.empty() && IO::FileExists(m_fullPath);
      }
    };

    //[JeromeCG 20111222] We cast the KL struct data directly in a void*, assuming that the 1st member (data) is directly at offset 0.
    // A cleaner way could be to go through RT::Manager (getFabricFileHandleDesc()) however since the EDK callback have no DG::Context
    // associated with them, and the RT::Manager is part of the DG::Context, we can't. This might be solved eventually by having a
    // common RT::Manager for all base types, which would be shared among all DG::Context.

    //[JeromeCG 20111222] For now we don't share handles through refcounting since it would required more care (such as CopyOnWrite)
    // to be safe in all scenarios. This might change once we implement some actual IO member functions for a FabricFileHandle.

    //[JeromeCG 20111222] TODO: This is a "leaking" version of the FileHandle. We need to add to RT::StructImpl and CG::StructAdapter 
    // the notion of registered copy constructors and destructors (callback functions) in order to have a clean implementation of this 
    // opaque type, to support cases where a FileHandle is being copied in KL (eg: to a local variable).

    FabricFileHandleInternalData *getOrCreateFabricFileHandleInternalData( void** data )
    {
      FabricFileHandleInternalData *internalData = (FabricFileHandleInternalData*)(*data);
      if( internalData == NULL )
      {
        internalData = new FabricFileHandleInternalData();
        *data = internalData;
      }
      return internalData;
    }

    void FabricFileHandleCopy( void **dst, void const *src )
    {
      if( src == NULL )
        FabricFileHandleDelete( dst );
      else
      {
        FabricFileHandleInternalData *dstInternalData = getOrCreateFabricFileHandleInternalData(dst);
        FabricFileHandleInternalData const *srcInternalData = (FabricFileHandleInternalData const *)src;
        *dstInternalData = *srcInternalData;
      }
    }

    void FabricFileHandleDelete( void **data )
    {
      if( *data != NULL )
      {
        delete getOrCreateFabricFileHandleInternalData( data );
        *data = NULL;
      }
    }

    bool FabricFileHandleSetFromPath( void **data, char const *pathData, size_t pathLength, bool readWriteAccess )
    {
        FabricFileHandleInternalData *internalData = getOrCreateFabricFileHandleInternalData(data);
        internalData->m_fullPath.assign(pathData, pathLength);
        internalData->m_readWriteAccess = readWriteAccess;
        return internalData->IsValid();
    }

    char const *FabricFileHandleGetFullPath( void const *data )
    {
      if( data == NULL )
        return "";
      FabricFileHandleInternalData const *internalData = (FabricFileHandleInternalData const *)data;
      return internalData->m_fullPath.c_str();
    }

    bool FabricFileHandleHasReadWriteAccess( void const *data )
    {
      if( data == NULL )
        return false;
      FabricFileHandleInternalData const *internalData = (FabricFileHandleInternalData const *)data;
      return internalData->m_readWriteAccess;
    }

  };
};
