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

// ====================================================================
// KL structs
FABRIC_EXT_KL_STRUCT( KinectCamera, {
  KL::Boolean initiated;
  KL::Integer tiltAngle;
} );

FABRIC_EXT_EXPORT void FabricKINECT_Init(
  KinectCamera & camera
)
{
#ifndef NDEBUG
  printf("  { FabricKINECT } : FabricKINECT_Init called.\n");
#endif
  if(!camera.initiated)
  {
    if(NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR) >= 0)
    {
      camera.initiated = true;
      
      // retrieve the angle from the camera
      LONG angle;
      if(NuiCameraElevationGetAngle(&angle) >= 0)
	camera.tiltAngle = (int)angle;
      else
	camera.tiltAngle = 0;
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
