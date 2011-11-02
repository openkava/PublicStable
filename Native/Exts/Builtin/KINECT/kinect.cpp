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
    size_t colorWidth;
    size_t depthWidth;
    size_t colorHeight;
    size_t depthHeight;
    size_t numColorPixels;
    size_t numDepthPixels;
    size_t colorBytes;
    size_t depthBytes;
    HANDLE  colorStream;
    HANDLE  depthStream;
    HANDLE  colorEvent;
    HANDLE  depthEvent;
    HANDLE  skeletonEvent;
    const NUI_IMAGE_FRAME* colorFrame;
    const NUI_IMAGE_FRAME* depthFrame;
  };
  
  LocalData * localData;
  KL::Boolean initiated;
  KL::Integer tiltAngle;
  KL::Boolean supportsColor;
  KL::Boolean supportsDepth;
  KL::Boolean supportsSkeleton;
  KL::VariableArray<KL::RGBA> colorData;
  KL::VariableArray<KL::Integer> depthData;
  KL::VariableArray<KL::Integer> playerData;
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
    if(camera.supportsDepth)
      initFlags = initFlags | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX;
    if(camera.supportsSkeleton)
      initFlags = initFlags | NUI_INITIALIZE_FLAG_USES_SKELETON;
    if(camera.supportsColor)
      initFlags = initFlags | NUI_INITIALIZE_FLAG_USES_COLOR;
      
    if(SUCCEEDED(NuiInitialize(initFlags)))
    {
      camera.initiated = true;
      
      // create the localdata
      camera.localData = new KinectCamera::LocalData();
      camera.localData->initFlags = initFlags;
      
      // retrieve the angle from the camera
      LONG angle;
      if(SUCCEEDED(NuiCameraElevationGetAngle(&angle)))
	camera.tiltAngle = (int)angle;
      else
	camera.tiltAngle = 0;
	
      // setup the image resolutions
      camera.localData->colorWidth = 640;
      camera.localData->colorHeight = 480;
      camera.localData->depthWidth = 320;
      camera.localData->depthHeight = 240;
      camera.localData->numColorPixels = camera.localData->colorWidth * camera.localData->colorHeight;
      camera.localData->numDepthPixels = camera.localData->depthWidth * camera.localData->depthHeight;
      camera.colorData.resize(camera.localData->numColorPixels);
      camera.playerData.resize(camera.localData->numDepthPixels);
      camera.depthData.resize(camera.localData->numDepthPixels);
      
      // setup the handles
      if(camera.supportsDepth)
      {
	camera.localData->depthEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	camera.localData->depthFrame = NULL;
	camera.localData->depthBytes = camera.localData->numDepthPixels * 2;
	if(FAILED(NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX, NUI_IMAGE_RESOLUTION_320x240 , 0, 2, camera.localData->depthEvent, &camera.localData->depthStream)))
	  camera.supportsDepth = false;
      }
      if(camera.supportsSkeleton)
      {
	camera.localData->skeletonEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(FAILED(NuiSkeletonTrackingEnable(camera.localData->skeletonEvent,0)))
	  camera.supportsSkeleton = false;
      }
      if(camera.supportsColor)
      {
	camera.localData->colorEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	camera.localData->colorFrame = NULL;
	camera.localData->colorBytes = camera.localData->numColorPixels * 4;
	if(FAILED(NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480 , 0, 2, camera.localData->colorEvent, &camera.localData->colorStream)))
	  camera.supportsColor = false;
      }
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
    if(SUCCEEDED(NuiCameraElevationSetAngle(angle)))
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
  if(camera.initiated && camera.supportsColor && camera.localData != NULL)
  {
    // pull the frame
    if(SUCCEEDED(NuiImageStreamGetNextFrame(camera.localData->colorStream, 0, &camera.localData->colorFrame)))
    {
      // get the actual frame struct
      NuiImageBuffer * buffer = camera.localData->colorFrame->pFrameTexture;
      KINECT_LOCKED_RECT LockedRect;
      buffer->LockRect(0, &LockedRect, NULL, 0);
      
      // extract the pixel information
      if(LockedRect.Pitch != 0)
      {
	BYTE * pBuffer = (BYTE*)LockedRect.pBits;
	size_t offset = 0;
	for(size_t i=0;i<camera.localData->numColorPixels;i++)
	{
	  camera.colorData[i].b = pBuffer[offset++];
	  camera.colorData[i].g = pBuffer[offset++];
	  camera.colorData[i].r = pBuffer[offset++];
	  camera.colorData[i].a = pBuffer[offset++];
	}
      }
      
      // release the frame
      buffer->UnlockRect(0);
      if(SUCCEEDED(NuiImageStreamReleaseFrame(camera.localData->colorStream,camera.localData->colorFrame)))
	camera.localData->colorFrame = NULL;
    }
  }
#ifndef NDEBUG
  printf("  { FabricKINECT } : FabricKINECT_GetColorPixels completed.\n");
#endif
}

FABRIC_EXT_EXPORT void FabricKINECT_GetDepthPixels(
  KinectCamera & camera
)
{
#ifndef NDEBUG
  printf("  { FabricKINECT } : FabricKINECT_GetDepthPixels called.\n");
#endif
  if(camera.initiated && camera.supportsDepth && camera.localData != NULL)
  {
    // pull the frame
    if(SUCCEEDED(NuiImageStreamGetNextFrame(camera.localData->depthStream, 0, &camera.localData->depthFrame)))
    {
      // get the actual frame struct
      NuiImageBuffer * buffer = camera.localData->depthFrame->pFrameTexture;
      KINECT_LOCKED_RECT LockedRect;
      buffer->LockRect(0, &LockedRect, NULL, 0);
      
      // extract the pixel information
      if(LockedRect.Pitch != 0)
      {
	BYTE* pBuffer = (BYTE*)LockedRect.pBits;
	size_t offset = 0;
	for(size_t i=0;i<camera.localData->numDepthPixels;i++)
	{
	  BYTE lower = pBuffer[offset++];
	  BYTE upper = pBuffer[offset++];
	  camera.playerData[i] = lower & 0x07;
	  camera.depthData[i] = upper<<5 | lower >>3;
	}
      }
      
      // release the frame
      buffer->UnlockRect(0);
      if(SUCCEEDED(NuiImageStreamReleaseFrame(camera.localData->depthStream,camera.localData->depthFrame)))
	camera.localData->depthFrame = NULL;
    }
  }
#ifndef NDEBUG
  printf("  { FabricKINECT } : FabricKINECT_GetDepthPixels completed.\n");
#endif
}