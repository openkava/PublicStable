/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>

#include "CImg.h"

using namespace Fabric::EDK;
IMPLEMENT_FABRIC_EDK_ENTRIES

using namespace cimg_library;

FABRIC_EXT_EXPORT void FabricCIMGDecode(
  KL::Data data,
  KL::Size dataSize,
  KL::Size &imageWidth,
  KL::Size &imageHeight,
  KL::VariableArray<KL::RGBA> &imagePixels
  )
{
#if defined(FABRIC_OS_WINDOWS)
  char const *dir = getenv("APPDATA");
  if(dir == NULL)
    dir = getenv("TEMP");
  if(dir == NULL)
    dir = getenv("TMP");
  if(dir == NULL)
    Fabric::EDK::throwException("CIMG extension: environment variable APP_DATA or TMP or TEMP is undefined");
  KL::String fileName( _tempnam( dir, "tmpfab_" ) );
#else
  KL::String fileName(tmpnam(NULL));
#endif

  // save the file to disk
  FILE * file = fopen(fileName.data(),"wb");
  if(!file)
    Fabric::EDK::throwException("CIMG extension: Cannot write to temporary file.");
  fwrite(data,dataSize,1,file);
  fclose(file);
  file = NULL;

  CImg<unsigned char> img(fileName.data());
  unsigned char * imgData = img.data();
  imageWidth = img.width();
  imageHeight = img.height();
  
  size_t nbPixels = imageWidth * imageHeight;
  size_t offsetG = nbPixels;
  size_t offsetB = nbPixels * 2;
  
  imagePixels.resize(nbPixels);
  for(size_t i=0;i<nbPixels;i++,offsetG++,offsetB++)
  {
    imagePixels[i].r = imgData[i];
    imagePixels[i].g = imgData[offsetG];
    imagePixels[i].b = imgData[offsetB];
    imagePixels[i].a = 255;
  }
}

