/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <ole2.h>
#include <MSR_NuiApi.h>

using namespace Fabric::EDK;
IMPLEMENT_FABRIC_EDK_ENTRIES

HANDLE	videoStreamHandle;
HANDLE	depthStreamHandle;
HANDLE	nextColorFrameEvent;
HANDLE	nextDepthFrameEvent;

// ====================================================================
// KL structs
FABRIC_EXT_KL_STRUCT( KinectCamera, {
  struct LocalData{
    DWORD initFlags;
    HANDLE  colorStream;
    HANDLE  depthStream;
    HANDLE  skeletonStream;
  };
  
  LocalData * localData;
  KL::Boolean initiated;
  KL::Integer tiltAngle;
  KL::Size width;
  KL::Size height;
  KL::Boolean supportsColor;
  KL::Boolean supportsDepth;
  KL::Boolean supportsSkeleton;
  KL::VariableArray<KL::RGB> colorData;
  KL::VariableArray<KL::Scalar> depthData;
  KL::VariableArray<KL::Xfo> skeletonData;
} );

FABRIC_EXT_EXPORT void FabricKINECT_Init(
  KinectCamera & camera
)
{
#ifndef NDEBUG
  printf("  { FabricKINECT } : FabricKINECT_Init called.\n");
#endif
  if(!camera.initiated && camera.localData == NULL)
  {
    // compute the flags
    DWORD initFlags = 0;
    if(camera.supportsColor)
      initFlags = initFlags | NUI_INITIALIZE_FLAG_USES_COLOR;
    if(camera.supportsDepth)
      initFlags = initFlags | NUI_INITIALIZE_FLAG_USES_SKELETON;
    if(camera.supportsSkeleton)
      initFlags = initFlags | NUI_INITIALIZE_FLAG_USES_DEPTH;
      
    if(NuiInitialize(initFlags) >= 0)
    {
      camera.initiated = true;
      
      // create the localdata
      camera.localData = new KinectCamera::LocalData();
      camera.localData->initFlags = initFlags;
      
      // retrieve the angle from the camera
      LONG angle;
      if(NuiCameraElevationGetAngle(&angle) >= 0)
	camera.tiltAngle = (int)angle;
      else
	camera.tiltAngle = 0;
	
      // setup the image resolutions
      camera.width = 640;
      camera.height = 480;
      camera.colorData.resize(camera.width * camera.height);
      camera.depthData.resize(camera.width * camera.height);
    }
  }
#ifndef NDEBUG
  printf("  { FabricKINECT } : FabricKINECT_Init completed.\n");
#endif
}

FABRIC_EXT_EXPORT void FabricKINECT_Shutdown(
  KinectCamera & camera
)
{
#ifndef NDEBUG
  printf("  { FabricKINECT } : FabricKINECT_Shutdown called.\n");
#endif
  if(camera.initiated)
  {
    NuiShutdown();
    camera.initiated = false;
    if(camera.localData != NULL)
    {
      delete(camera.localData);
      camera.localData = NULL;
    }
  }
#ifndef NDEBUG
  printf("  { FabricKINECT } : FabricKINECT_Shutdown completed.\n");
#endif
}

FABRIC_EXT_EXPORT void FabricKINECT_Tilt(
  KinectCamera & camera,
  KL::Integer & angle
)
{
#ifndef NDEBUG
  printf("  { FabricKINECT } : FabricKINECT_Tilt called.\n");
#endif
  if(camera.initiated && camera.tiltAngle != angle)
  {
    if(angle > 27)
      angle = 27;
    else if(angle < -27)
      angle = -27;
    if(NuiCameraElevationSetAngle(angle) >= 0)
      camera.tiltAngle = angle;
  }
#ifndef NDEBUG
  printf("  { FabricKINECT } : FabricKINECT_Tilt completed.\n");
#endif
}

FABRIC_EXT_EXPORT void FabricKINECT_GetColorPixels(
  KinectCamera & camera
)
{
#ifndef NDEBUG
  printf("  { FabricKINECT } : FabricKINECT_GetColorPixels called.\n");
#endif
  if(camera.initiated)
  {
    
  }
#ifndef NDEBUG
  printf("  { FabricKINECT } : FabricKINECT_GetColorPixels completed.\n");
#endif
}