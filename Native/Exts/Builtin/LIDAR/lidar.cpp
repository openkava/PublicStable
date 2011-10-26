/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>
#include <Fabric/Base/RC/Object.h>

using namespace Fabric::EDK;
IMPLEMENT_FABRIC_EDK_ENTRIES

#include <string>
#include <sstream>
#include <liblas/reader.hpp>

// ====================================================================
// KL structs
FABRIC_EXT_KL_STRUCT( LidarReader, {
  class LocalData : public Fabric::RC::Object {
  public:
  
    LocalData(void * pointer, KL::Size size);
    
    void reset();
    
    std::stringstream mStream;
    liblas::Reader * mReader;
  
  protected:
  
    virtual ~LocalData();
  };

  LocalData * localData;
} );

LidarReader::LocalData::LocalData(void * pointer, KL::Size size) {
  mStream.write((const char*)pointer,size);
  mReader = new liblas::Reader(mStream);
}

LidarReader::LocalData::~LocalData() {
#ifndef NDEBUG
    printf("  { FabricLIDAR } : Calling LidarReader::LocalData::~LocalData()\n");
#endif
  delete(mReader);
  mStream.clear();
}

FABRIC_EXT_EXPORT void FabricLIDAR_Reader_Decode(
  KL::Data resourceData,
  KL::Size resourceDataSize,
  LidarReader & lidar
)
{
  if(lidar.localData == NULL && resourceData != NULL && resourceDataSize > 0) {
#ifndef NDEBUG
    printf("  { FabricLIDAR} : FabricLIDAR_Reader_Decode called.\n");
#endif

    lidar.localData = new LidarReader::LocalData(resourceData,resourceDataSize);

#ifndef NDEBUG
    printf("  { FabricLIDAR } : FabricLIDAR_Reader_Decode completed.\n");
#endif
  }
}

FABRIC_EXT_EXPORT void FabricLIDAR_Reader_Free(
  LidarReader & lidar
)
{
  if(lidar.localData != NULL) {
#ifndef NDEBUG
    printf("  { FabricLIDAR} : FabricLIDAR_Reader_Free called.\n");
#endif

    lidar.localData->release();
    lidar.localData = NULL;

#ifndef NDEBUG
    printf("  { FabricLIDAR } : FabricLIDAR_Reader_Free completed.\n");
#endif
  }
}