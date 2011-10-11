/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>

//#include <boost/smart_ptr.hpp>
//#include <boost/format.hpp>
//#include <boost/variant.hpp>
#include <boost/algorithm/string.hpp>

#include <utility>
#include <limits>

#include <set>
#include <vector>
#include <map>
#include <list>

#include <stdexcept>
#include <exception>

#include <string>

#include <math.h>
#include <float.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <Alembic/AbcCoreHDF5/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/Abc/All.h>

namespace AbcA = ::Alembic::AbcCoreAbstract::ALEMBIC_VERSION_NS;
using namespace AbcA;

using namespace Fabric::EDK;
IMPLEMENT_FABRIC_EDK_ENTRIES

struct AlembicHandle{
  KL::Data pointer;
  KL::Vec2 timeRange;
};

struct SampleInfo
{
   Alembic::AbcCoreAbstract::index_t floorIndex;
   Alembic::AbcCoreAbstract::index_t ceilIndex;
   double alpha;
};

SampleInfo getSampleInfo
(
   double iFrame,
   Alembic::AbcCoreAbstract::TimeSamplingPtr iTime,
   size_t numSamps
)
{
   SampleInfo result;
   if (numSamps == 0)
      numSamps = 1;

   std::pair<Alembic::AbcCoreAbstract::index_t, double> floorIndex =
   iTime->getFloorIndex(iFrame, numSamps);

   result.floorIndex = floorIndex.first;
   result.ceilIndex = result.floorIndex;

   if (fabs(iFrame - floorIndex.second) < 0.0001) {
      result.alpha = 0.0f;
      return result;
   }

   std::pair<Alembic::AbcCoreAbstract::index_t, double> ceilIndex =
   iTime->getCeilIndex(iFrame, numSamps);

   if (fabs(iFrame - ceilIndex.second) < 0.0001) {
      result.floorIndex = ceilIndex.first;
      result.ceilIndex = result.floorIndex;
      result.alpha = 0.0f;
      return result;
   }

   if (result.floorIndex == ceilIndex.first) {
      result.alpha = 0.0f;
      return result;
   }

   result.ceilIndex = ceilIndex.first;

   result.alpha = (iFrame - floorIndex.second) / (ceilIndex.second - floorIndex.second);
   return result;
}

Alembic::Abc::IObject getObjectFromArchive(Alembic::Abc::IArchive * archive, const std::string & identifier)
{
  // split the path
  std::vector<std::string> parts;
  boost::split(parts, identifier, boost::is_any_of("/"));

  // recurse to find it
  Alembic::Abc::IObject obj = archive->getTop();
  for(size_t i=1;i<parts.size();i++)
  {
    Alembic::Abc::IObject child(obj,parts[i]);
    obj = child;
  }
    
  return obj; 
}

FABRIC_EXT_EXPORT void FabricALEMBICDecode(
  KL::Data objData,
  KL::Size objDataSize,
  AlembicHandle &handle
  )
{
  Alembic::Abc::IArchive * archive = (Alembic::Abc::IArchive *)handle.pointer;
  if( archive == NULL )
  {
#if defined(FABRIC_OS_WINDOWS)
    char const *dir = getenv("APPDATA");
    if(dir == NULL)
      dir = getenv("TEMP");
    if(dir == NULL)
      dir = getenv("TMP");
    if(dir == NULL)
      Fabric::EDK::throwException("Alembic extension: environment variable APP_DATA or TMP or TEMP is undefined");
    std::string fileName( _tempnam( dir, "tmpfab_" ) );
#else
    std::string fileName(tmpnam(NULL));
#endif
      
    // save the file to disk
    FILE * file = fopen(fileName.c_str(),"wb");
    fwrite(objData,objDataSize,1,file);
    fclose(file);
    file = NULL;

    // load the file    
    archive = new Alembic::Abc::IArchive(
      Alembic::AbcCoreHDF5::ReadArchive(),fileName.c_str());
    handle.pointer = archive;
    
    // determine the time range
    handle.timeRange.x = 1000000;
    handle.timeRange.y = -1000000;
    
    // let's skip the first one
    for(uint32_t i=1;i<archive->getNumTimeSamplings();i++){
      AbcA::TimeSamplingPtr ts = archive->getTimeSampling(i);
      const std::vector<chrono_t> & times = ts->getStoredTimes();
      if(times.size() == 0)
        continue;
      if(handle.timeRange.x > times[0])
        handle.timeRange.x = times[0];
      if(handle.timeRange.y < times[times.size()-1])
        handle.timeRange.y = times[times.size()-1];
    }
    
    if(handle.timeRange.x == 1000000) {
      handle.timeRange.x = handle.timeRange.y = 0.0f;
    }
  }
}

FABRIC_EXT_EXPORT void FabricALEMBICFreeAlembicHandle(
  AlembicHandle &handle
)
{
  if(handle.pointer != NULL) {
    Alembic::Abc::IArchive * archive = (Alembic::Abc::IArchive *)handle.pointer;
    delete(archive);
    handle.pointer = NULL;
  }
}

FABRIC_EXT_EXPORT void FabricALEMBICGetIdentifiers(
  AlembicHandle &handle,
  KL::VariableArray<KL::String> & identifiers
)
{
  identifiers.resize(0);
  Alembic::Abc::IArchive * archive = (Alembic::Abc::IArchive *)handle.pointer;
  if( archive != NULL )
  {
    std::vector<Alembic::Abc::IObject> iObjects;
    iObjects.push_back(archive->getTop());

    size_t offset = 0;
    for(size_t i=0;i<iObjects.size();i++)
    {
      Alembic::Abc::IObject obj(iObjects[i],Alembic::Abc::kWrapExisting);
      
      for(size_t j=0;j<obj.getNumChildren();j++)
      {
        Alembic::Abc::IObject child = obj.getChild(j);

        const Alembic::Abc::MetaData &md = child.getMetaData();
        if(Alembic::AbcGeom::IXformSchema::matches(md) ||
           Alembic::AbcGeom::IPolyMesh::matches(md)||
           Alembic::AbcGeom::ICurves::matches(md)||
           Alembic::AbcGeom::INuPatch::matches(md)||
           Alembic::AbcGeom::IPoints::matches(md)||
           Alembic::AbcGeom::ISubD::matches(md)||
           Alembic::AbcGeom::ICamera::matches(md))
        {
          iObjects.push_back(child);
          offset++;
        }
      }
    }
    
    identifiers.resize(offset);
    offset = 0;
    for(size_t i=1;i<iObjects.size();i++)
    {
      identifiers[offset] = iObjects[i].getFullName().c_str();
      identifiers[offset] += "|";
      int numSamples = 1;
      const Alembic::Abc::MetaData &md = iObjects[i].getMetaData();
      if(Alembic::AbcGeom::IXform::matches(md)) {
        identifiers[offset] += "Xform|";
        numSamples = Alembic::AbcGeom::IXform(iObjects[i],Alembic::Abc::kWrapExisting).getSchema().getNumSamples();
      } else if(Alembic::AbcGeom::IPolyMesh::matches(md)) {
        identifiers[offset] += "PolyMesh|";
        numSamples = Alembic::AbcGeom::IPolyMesh(iObjects[i],Alembic::Abc::kWrapExisting).getSchema().getNumSamples();
      } else if(Alembic::AbcGeom::ICurves::matches(md)) {
        identifiers[offset] += "Curves|";
        numSamples = Alembic::AbcGeom::ICurves(iObjects[i],Alembic::Abc::kWrapExisting).getSchema().getNumSamples();
      } else if(Alembic::AbcGeom::INuPatch::matches(md)) {
        identifiers[offset] += "NuPatch|";
        numSamples = Alembic::AbcGeom::INuPatch(iObjects[i],Alembic::Abc::kWrapExisting).getSchema().getNumSamples();
      } else if(Alembic::AbcGeom::IPoints::matches(md)) {
        identifiers[offset] += "Points|";
        numSamples = Alembic::AbcGeom::IPoints(iObjects[i],Alembic::Abc::kWrapExisting).getSchema().getNumSamples();
      } else if(Alembic::AbcGeom::ISubD::matches(md)) {
        identifiers[offset] += "SubD|";
        numSamples = Alembic::AbcGeom::ISubD(iObjects[i],Alembic::Abc::kWrapExisting).getSchema().getNumSamples();
      } else if(Alembic::AbcGeom::ICamera::matches(md)) {
        identifiers[offset] += "Camera|";
        numSamples = Alembic::AbcGeom::ICamera(iObjects[i],Alembic::Abc::kWrapExisting).getSchema().getNumSamples();
      }
      identifiers[offset] += boost::lexical_cast<std::string>(numSamples).c_str();
      offset++;
    }
  }
}

FABRIC_EXT_EXPORT void FabricALEMBICParseXform(
  AlembicHandle &handle,
  KL::String & identifier,
  KL::Scalar & time,
  KL::Xfo & transform
  )
{
  Alembic::Abc::IArchive * archive = (Alembic::Abc::IArchive *)handle.pointer;
  if(archive == NULL)
    return;
    
  Alembic::AbcGeom::IXform obj(getObjectFromArchive(archive,identifier.data()),Alembic::Abc::kWrapExisting);
  if( obj.valid() )
  {
    // get the schema
    Alembic::AbcGeom::IXformSchema schema = obj.getSchema();

    // get the sample information
    SampleInfo sampleInfo = getSampleInfo(time,schema.getTimeSampling(),schema.getNumSamples());
      
    // get the sample
    Alembic::AbcGeom::XformSample sample;
    schema.get(sample,sampleInfo.floorIndex);

    // blend
    Alembic::Abc::M44d matrix = sample.getMatrix();
    if(sampleInfo.alpha != 0.0)
    {
      obj.getSchema().get(sample,sampleInfo.ceilIndex);
      Alembic::Abc::M44d ceilMatrix = sample.getMatrix();
      matrix = (1.0 - sampleInfo.alpha) * matrix + sampleInfo.alpha * ceilMatrix;
    }
    
    Alembic::Abc::Quatd ori = extractQuat(matrix);
    Alembic::Abc::V3d sc;
    extractScaling(matrix,sc);
    
    transform.ori.v.x = (KL::Scalar)ori.v.x;
    transform.ori.v.y = (KL::Scalar)ori.v.y;
    transform.ori.v.z = (KL::Scalar)ori.v.z;
    transform.ori.w = (KL::Scalar)ori.r;
    transform.tr.x = (KL::Scalar)matrix[3][0];
    transform.tr.y = (KL::Scalar)matrix[3][1];
    transform.tr.z = (KL::Scalar)matrix[3][2];
    transform.sc.x = (KL::Scalar)sc.x;
    transform.sc.y = (KL::Scalar)sc.y;
    transform.sc.z = (KL::Scalar)sc.z;
  }
}

FABRIC_EXT_EXPORT void FabricALEMBICParseCamera(
  AlembicHandle &handle,
  KL::String & identifier,
  KL::Scalar & time,
  KL::Scalar & near,
  KL::Scalar & far,
  KL::Scalar & fovY
  )
{
  Alembic::Abc::IArchive * archive = (Alembic::Abc::IArchive *)handle.pointer;
  if(archive == NULL)
    return;
    
  Alembic::AbcGeom::ICamera obj(getObjectFromArchive(archive,identifier.data()),Alembic::Abc::kWrapExisting);
  if( obj.valid() )
  {
    // get the schema
    Alembic::AbcGeom::ICameraSchema schema = obj.getSchema();
      
    // get the sample information
    SampleInfo sampleInfo = getSampleInfo(time,schema.getTimeSampling(),schema.getNumSamples());

    // get the sample
    Alembic::AbcGeom::CameraSample sample;
    schema.get(sample,sampleInfo.floorIndex);

    // clipping planes
    near = sample.getNearClippingPlane();    
    far = sample.getFarClippingPlane();
    
    // compute fovY from aspect and aperture
    KL::Scalar focallength = sample.getFocalLength();
    KL::Scalar aperture = sample.getVerticalAperture();
    fovY = 1.0f * atanf(50.0f * aperture / focallength);
    
    // blend
    if(sampleInfo.alpha != 0.0)
    {
      schema.get(sample,sampleInfo.ceilIndex);
      float ialpha = 1.0f - sampleInfo.alpha;
      near = ialpha * near + sampleInfo.alpha * sample.getNearClippingPlane();
      far = ialpha * far + sampleInfo.alpha * sample.getFarClippingPlane();
      focallength = sample.getFocalLength();
      aperture = sample.getVerticalAperture();
      fovY = ialpha * fovY + sampleInfo.alpha * (1.0f * atanf(50.0f * aperture / focallength));
    }
  }
}

FABRIC_EXT_EXPORT void FabricALEMBICParsePolyMeshUniforms(
  AlembicHandle &handle,
  KL::String & identifier,
  KL::VariableArray<KL::Integer>& indices
  )
{
  Alembic::Abc::IArchive * archive = (Alembic::Abc::IArchive *)handle.pointer;
  if(archive == NULL)
    return;
    
  Alembic::AbcGeom::IPolyMesh obj(getObjectFromArchive(archive,identifier.data()),Alembic::Abc::kWrapExisting);
  if( obj.valid() )
  {
    // get the schema
    Alembic::AbcGeom::IPolyMeshSchema schema = obj.getSchema();

    // get the sample
    Alembic::AbcGeom::IPolyMeshSchema::Sample sample;
    schema.get(sample,0);

    // store the indices
    std::vector<KL::Integer> vecIndices;
    Alembic::Abc::Int32ArraySamplePtr faceCounts = sample.getFaceCounts();
    for(size_t i=0;i<faceCounts->size();i++)
    {
      size_t curSize = vecIndices.size();
      if(faceCounts->get()[i] == 3)
      {
        vecIndices.push_back(curSize + 2);
        vecIndices.push_back(curSize + 1);
        vecIndices.push_back(curSize);
      }
      else if(faceCounts->get()[i] == 4)
      {
        vecIndices.push_back(curSize + 2);
        vecIndices.push_back(curSize + 1);
        vecIndices.push_back(curSize);
        vecIndices.push_back(curSize + 5);
        vecIndices.push_back(curSize + 4);
        vecIndices.push_back(curSize + 3);
      }
    }
    
    // finally copy them out
    indices.resize(vecIndices.size());
    memcpy(&indices[0],&vecIndices[0],sizeof(KL::Integer) * vecIndices.size());
  }
}

FABRIC_EXT_EXPORT void FabricALEMBICParsePolyMeshCount(
  AlembicHandle &handle,
  KL::String & identifier,
  KL::Size & count
  )
{
  Alembic::Abc::IArchive * archive = (Alembic::Abc::IArchive *)handle.pointer;
  if(archive == NULL)
    return;
    
  Alembic::AbcGeom::IPolyMesh obj(getObjectFromArchive( archive,identifier.data()),Alembic::Abc::kWrapExisting);
  if( obj.valid() )
  {
    // get the schema
    Alembic::AbcGeom::IPolyMeshSchema schema = obj.getSchema();
      
    // get the sample
    Alembic::AbcGeom::IPolyMeshSchema::Sample sample;
    schema.get(sample,0);

    // loop on all counts
    Alembic::Abc::Int32ArraySamplePtr faceCounts = sample.getFaceCounts();
    count = 0;
    for(size_t i=0;i<faceCounts->size();i++)
    {
      if(faceCounts->get()[i] == 3)
        count += 3;
      else if(faceCounts->get()[i] == 4)
        count += 6;
    }
  }
}

FABRIC_EXT_EXPORT void FabricALEMBICParsePolyMeshAttributes(
  AlembicHandle &handle,
  KL::String & identifier,
  KL::Scalar & time,
  KL::SlicedArray<KL::Vec3>& vertices,
  KL::SlicedArray<KL::Vec3>& normals,
  KL::Boolean & uvsLoaded,
  KL::SlicedArray<KL::Vec2>& uvs
  )
{
  Alembic::Abc::IArchive * archive = (Alembic::Abc::IArchive *)handle.pointer;
  if(archive == NULL)
    return;
    
  Alembic::AbcGeom::IPolyMesh obj(getObjectFromArchive(archive,identifier.data()),Alembic::Abc::kWrapExisting);
  if( obj.valid() )
  {
    // get the schema
    Alembic::AbcGeom::IPolyMeshSchema schema = obj.getSchema();

    // get the sample information
    SampleInfo sampleInfo = getSampleInfo(time,schema.getTimeSampling(),schema.getNumSamples());
    
    // get the sample
    Alembic::AbcGeom::IPolyMeshSchema::Sample sample = schema.getValue(sampleInfo.floorIndex);
    Alembic::AbcGeom::IPolyMeshSchema::Sample sampleB;
    Alembic::Abc::P3fArraySamplePtr abcPoints = sample.getPositions();
    Alembic::Abc::P3fArraySamplePtr abcPointsB;
    bool requiresInterpolation = sampleInfo.alpha != 0.0;
    if(requiresInterpolation)
    {
       sampleB = schema.getValue(sampleInfo.ceilIndex);
       abcPointsB = sampleB.getPositions();
    }
    float iAlpha = 1.0 - sampleInfo.alpha;
    
    Alembic::Abc::Int32ArraySamplePtr faceCounts = sample.getFaceCounts();
    Alembic::Abc::Int32ArraySamplePtr faceIndices= sample.getFaceIndices();
    
    // load the vertices
    size_t vertexOffset = 0;
    size_t indexOffset = 0;
    size_t currentIndex;
    for(size_t i=0;i<faceCounts->size();i++)
    {
      size_t count = faceCounts->get()[i];
      if(count == 3)
      {
        for(int j=0;j<3;j++)
        {
          currentIndex = indexOffset + j;
          vertices[vertexOffset].x = iAlpha * abcPoints->get()[faceIndices->get()[currentIndex]].x;
          vertices[vertexOffset].y = iAlpha * abcPoints->get()[faceIndices->get()[currentIndex]].y;
          vertices[vertexOffset].z = iAlpha * abcPoints->get()[faceIndices->get()[currentIndex]].z;
          if(requiresInterpolation)
          {
            vertices[vertexOffset].x += sampleInfo.alpha * abcPointsB->get()[faceIndices->get()[currentIndex]].x;
            vertices[vertexOffset].y += sampleInfo.alpha * abcPointsB->get()[faceIndices->get()[currentIndex]].y;
            vertices[vertexOffset].z += sampleInfo.alpha * abcPointsB->get()[faceIndices->get()[currentIndex]].z;
          }
          vertexOffset++;
        }
      }
      else if(count == 4)
      {
        for(int j=0;j<3;j++)
        {
          currentIndex = indexOffset + j;
          vertices[vertexOffset].x = iAlpha * abcPoints->get()[faceIndices->get()[currentIndex]].x;
          vertices[vertexOffset].y = iAlpha * abcPoints->get()[faceIndices->get()[currentIndex]].y;
          vertices[vertexOffset].z = iAlpha * abcPoints->get()[faceIndices->get()[currentIndex]].z;
          if(requiresInterpolation)
          {
            vertices[vertexOffset].x += sampleInfo.alpha * abcPointsB->get()[faceIndices->get()[currentIndex]].x;
            vertices[vertexOffset].y += sampleInfo.alpha * abcPointsB->get()[faceIndices->get()[currentIndex]].y;
            vertices[vertexOffset].z += sampleInfo.alpha * abcPointsB->get()[faceIndices->get()[currentIndex]].z;
          }
          vertexOffset++;
        }
        for(int j=2;j<5;j++)
        {
          currentIndex = indexOffset+(j%4);
          vertices[vertexOffset].x = iAlpha * abcPoints->get()[faceIndices->get()[currentIndex]].x;
          vertices[vertexOffset].y = iAlpha * abcPoints->get()[faceIndices->get()[currentIndex]].y;
          vertices[vertexOffset].z = iAlpha * abcPoints->get()[faceIndices->get()[currentIndex]].z;
          if(requiresInterpolation)
          {
            vertices[vertexOffset].x += sampleInfo.alpha * abcPointsB->get()[faceIndices->get()[currentIndex]].x;
            vertices[vertexOffset].y += sampleInfo.alpha * abcPointsB->get()[faceIndices->get()[currentIndex]].y;
            vertices[vertexOffset].z += sampleInfo.alpha * abcPointsB->get()[faceIndices->get()[currentIndex]].z;
          }
          vertexOffset++;
        }
      }
      indexOffset += count;
    }
    
    // load the normals
    Alembic::AbcGeom::IN3fGeomParam N = schema.getNormalsParam();
    if(N.valid())
    {
      Alembic::AbcGeom::N3fArraySamplePtr nsp = N.getExpandedValue(sampleInfo.floorIndex).getVals();
      Alembic::AbcGeom::N3fArraySamplePtr nspB;
      if(requiresInterpolation)
        nspB = N.getExpandedValue(sampleInfo.ceilIndex).getVals();
      
      if(nsp->size() > 0)
      {
        vertexOffset = 0;
        indexOffset = 0;
        for(size_t i=0;i<faceCounts->size();i++)
        {
          size_t count = faceCounts->get()[i];
          if(count == 3)
          {
            for(int j=0;j<3;j++)
            {
              currentIndex = indexOffset + j;
              normals[vertexOffset].x = iAlpha * nsp->get()[currentIndex].x;
              normals[vertexOffset].y = iAlpha * nsp->get()[currentIndex].y;
              normals[vertexOffset].z = iAlpha * nsp->get()[currentIndex].z;
              if(requiresInterpolation)
              {
                normals[vertexOffset].x += sampleInfo.alpha * nspB->get()[currentIndex].x;
                normals[vertexOffset].y += sampleInfo.alpha * nspB->get()[currentIndex].y;
                normals[vertexOffset].z += sampleInfo.alpha * nspB->get()[currentIndex].z;
                float length = normals[vertexOffset].x * normals[vertexOffset].x +
                               normals[vertexOffset].y * normals[vertexOffset].y +
                               normals[vertexOffset].z * normals[vertexOffset].z;
                if(length > 0.0f)
                {
                  length = sqrtf(length);
                  normals[vertexOffset].x /= length;
                  normals[vertexOffset].y /= length;
                  normals[vertexOffset].z /= length;
                }
              }
              vertexOffset++;
            }
          }
          else if(count == 4)
          {
            for(int j=0;j<3;j++)
            {
              currentIndex = indexOffset + j;
              normals[vertexOffset].x = iAlpha * nsp->get()[currentIndex].x;
              normals[vertexOffset].y = iAlpha * nsp->get()[currentIndex].y;
              normals[vertexOffset].z = iAlpha * nsp->get()[currentIndex].z;
              if(requiresInterpolation)
              {
                normals[vertexOffset].x += sampleInfo.alpha * nspB->get()[currentIndex].x;
                normals[vertexOffset].y += sampleInfo.alpha * nspB->get()[currentIndex].y;
                normals[vertexOffset].z += sampleInfo.alpha * nspB->get()[currentIndex].z;
                float length = normals[vertexOffset].x * normals[vertexOffset].x +
                               normals[vertexOffset].y * normals[vertexOffset].y +
                               normals[vertexOffset].z * normals[vertexOffset].z;
                if(length > 0.0f)
                {
                  length = sqrtf(length);
                  normals[vertexOffset].x /= length;
                  normals[vertexOffset].y /= length;
                  normals[vertexOffset].z /= length;
                }
              }
              vertexOffset++;
            }
            for(int j=2;j<5;j++)
            {
              currentIndex = indexOffset+(j%4);
              normals[vertexOffset].x = iAlpha * nsp->get()[currentIndex].x;
              normals[vertexOffset].y = iAlpha * nsp->get()[currentIndex].y;
              normals[vertexOffset].z = iAlpha * nsp->get()[currentIndex].z;
              if(requiresInterpolation)
              {
                normals[vertexOffset].x += sampleInfo.alpha * nspB->get()[currentIndex].x;
                normals[vertexOffset].y += sampleInfo.alpha * nspB->get()[currentIndex].y;
                normals[vertexOffset].z += sampleInfo.alpha * nspB->get()[currentIndex].z;
                float length = normals[vertexOffset].x * normals[vertexOffset].x +
                               normals[vertexOffset].y * normals[vertexOffset].y +
                               normals[vertexOffset].z * normals[vertexOffset].z;
                if(length > 0.0f)
                {
                  length = sqrtf(length);
                  normals[vertexOffset].x /= length;
                  normals[vertexOffset].y /= length;
                  normals[vertexOffset].z /= length;
                }
              }
              vertexOffset++;
            }
          }
          indexOffset += count;
        }
      }
    }

    if(!uvsLoaded)
    {
      Alembic::AbcGeom::IV2fGeomParam U = schema.getUVsParam();
      if(U.valid())
      {
        Alembic::AbcGeom::V2fArraySamplePtr usp = U.getExpandedValue(0).getVals();
        if(usp->size() > 0)
        {
          vertexOffset = 0;
          indexOffset = 0;
          for(size_t i=0;i<faceCounts->size();i++)
          {
            size_t count = faceCounts->get()[i];
            if(count == 3)
            {
              for(int j=0;j<3;j++)
              {
                currentIndex = indexOffset + j;
                uvs[vertexOffset].x = usp->get()[currentIndex].x;
                uvs[vertexOffset++].y = usp->get()[currentIndex].y;
              }
            }
            else if(count == 4)
            {
              for(int j=0;j<3;j++)
              {
                currentIndex = indexOffset + j;
                uvs[vertexOffset].x = usp->get()[currentIndex].x;
                uvs[vertexOffset++].y = usp->get()[currentIndex].y;
              }
              for(int j=2;j<5;j++)
              {
                currentIndex = indexOffset+(j%4);
                uvs[vertexOffset].x = usp->get()[currentIndex].x;
                uvs[vertexOffset++].y = usp->get()[currentIndex].y;
              }
            }
            indexOffset += count;
          }
        }
        uvsLoaded = true;
      }
    }
  }
}
