/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>
#include <Fabric/Base/RC/Object.h>

using namespace Fabric::EDK;
IMPLEMENT_FABRIC_EDK_ENTRIES

#include <string>
#include <iostream>
#include <fstream>
#include <liblas/liblas.hpp>

// ====================================================================
// KL structs
FABRIC_EXT_KL_STRUCT( LidarReader, {
  class LocalData : public Fabric::RC::Object {
  public:
  
    LocalData(KL::String & fileName, KL::Boolean & compressed);
    
    void reset();

    std::ifstream mStream;    
    liblas::Reader * mReader;
  
  protected:
  
    virtual ~LocalData();
  };

  LocalData * localData;
  KL::String url;
  KL::Boolean compressed;
} );

LidarReader::LocalData::LocalData(KL::String & fileName, KL::Boolean & compressed) {
#ifndef NDEBUG
    printf("  { FabricLIDAR } : Opening file %s\n",fileName.data());
#endif
  mStream.open(fileName.data());
  liblas::ReaderFactory f;
  try
  {
    mReader = new liblas::Reader(f.CreateWithStream(mStream));
  }
  catch(std::runtime_error e)
  {
    printf("exception: %s\n",e.what());
  }
}

LidarReader::LocalData::~LocalData() {
#ifndef NDEBUG
    printf("  { FabricLIDAR } : Calling LidarReader::LocalData::~LocalData()\n");
#endif
  delete(mReader);
  mStream.clear();
}

FABRIC_EXT_EXPORT void FabricLIDAR_Reader_Open(
  KL::String & fileName,
  LidarReader & lidar
)
{
  if(lidar.localData == NULL && fileName.data() != NULL) {
#ifndef NDEBUG
    printf("  { FabricLIDAR} : FabricLIDAR_Reader_Open called.\n");
#endif

    lidar.compressed = lidar.url.data()[lidar.url.length()-1] == 'z' || lidar.url.data()[lidar.url.length()-1] == 'Z';
    lidar.localData = new LidarReader::LocalData(fileName,lidar.compressed);

#ifndef NDEBUG
    printf("  { FabricLIDAR } : FabricLIDAR_Reader_Open completed.\n");
#endif
  }
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

#if defined(FABRIC_OS_WINDOWS)
    char const *dir = getenv("APPDATA");
    if(dir == NULL)
      dir = getenv("TEMP");
    if(dir == NULL)
      dir = getenv("TMP");
    if(dir == NULL)
      Fabric::EDK::throwException("Lidar extension: environment variable APP_DATA or TMP or TEMP is undefined");
    KL::String fileName( _tempnam( dir, "tmpfab_" ) );
#else
    KL::String fileName(tmpnam(NULL));
#endif

    // save the file to disk
    FILE * file = fopen(fileName.data(),"wb");
    fwrite(resourceData,resourceDataSize,1,file);
    fclose(file);
    file = NULL;
    
    FabricLIDAR_Reader_Open(fileName,lidar);

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

FABRIC_EXT_EXPORT void FabricLIDAR_Reader_GetCount(
  LidarReader & lidar,
  KL::Size & count
)
{
  if(lidar.localData != NULL) {
#ifndef NDEBUG
    printf("  { FabricLIDAR} : FabricLIDAR_Reader_Count called.\n");
#endif

    count = (KL::Size)lidar.localData->mReader->GetHeader().GetPointRecordsCount();

#ifndef NDEBUG
    printf("  { FabricLIDAR } : FabricLIDAR_Reader_Count completed.\n");
#endif
  }
}

FABRIC_EXT_EXPORT void FabricLIDAR_Reader_GetPoints(
  LidarReader & lidar,
  KL::SlicedArray<KL::Vec3> positions,
  KL::SlicedArray<KL::Color> colors
)
{
  if(lidar.localData != NULL) {
#ifndef NDEBUG
    printf("  { FabricLIDAR} : FabricLIDAR_Reader_GetPoints called.\n");
#endif

    // only do this if the counts match
    KL::Size count = (KL::Size)lidar.localData->mReader->GetHeader().GetPointRecordsCount();
    if(positions.size() == count)
    {
      KL::Size offset = 0;
      while (lidar.localData->mReader->ReadNextPoint())
      {
          liblas::Point const& p = lidar.localData->mReader->GetPoint();
          positions[offset].x = (KL::Scalar)p.GetX();
          positions[offset].y = (KL::Scalar)p.GetZ();
          positions[offset].z = (KL::Scalar)p.GetY();
          colors[offset].r = float(p.GetColor().GetRed()) / 255.0f;
          colors[offset].g = float(p.GetColor().GetGreen()) / 255.0f;
          colors[offset].b = float(p.GetColor().GetBlue()) / 255.0f;
          colors[offset].a = 1.0;
          offset++;
      }
    }

#ifndef NDEBUG
    printf("  { FabricLIDAR } : FabricLIDAR_Reader_GetPoints completed.\n");
#endif
  }
}