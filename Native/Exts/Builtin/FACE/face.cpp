/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>

using namespace Fabric::EDK;
IMPLEMENT_FABRIC_EDK_ENTRIES

#include <opencv/cv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>

FABRIC_EXT_KL_STRUCT( FaceLocation, {
  KL::Size left;
  KL::Size right;
  KL::Size top;
  KL::Size bottom;
} );

FABRIC_EXT_EXPORT void FabricFACE_Detection(
  KL::Data pixels,
  KL::Size width,
  KL::Size height,
  KL::Size channels,
  KL::VariableArray<FaceLocation> & faces
)
{
  // convert our image to an opencv image
  cv::Mat img(height,width,CV_8UC1+channels-1,pixels);
  //LibFace* libFace = new LibFace(libface::DETECT, std::string("."));
  return;
}
