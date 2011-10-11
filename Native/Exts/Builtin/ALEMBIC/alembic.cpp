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
  KL::Integer numSamples;
};

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
    
    // determine the number of samples
    handle.numSamples = 0;
    std::vector<Alembic::Abc::IObject> iObjects;
    iObjects.push_back(archive->getTop());
    for(size_t i=0;i<iObjects.size();i++)
    {
      Alembic::Abc::IObject obj(iObjects[i],Alembic::Abc::kWrapExisting);

      const Alembic::Abc::MetaData &md = obj.getMetaData();
      KL::Integer numCurrent = 0;
      if(Alembic::AbcGeom::IXform::matches(md))
        numCurrent = Alembic::AbcGeom::IXform(obj,Alembic::Abc::kWrapExisting).getSchema().getNumSamples();
      else if(Alembic::AbcGeom::IPolyMesh::matches(md))
        numCurrent = Alembic::AbcGeom::IPolyMesh(obj,Alembic::Abc::kWrapExisting).getSchema().getNumSamples();
      else if(Alembic::AbcGeom::ICurves::matches(md))
        numCurrent = Alembic::AbcGeom::ICurves(obj,Alembic::Abc::kWrapExisting).getSchema().getNumSamples();
      else if(Alembic::AbcGeom::INuPatch::matches(md))
        numCurrent = Alembic::AbcGeom::INuPatch(obj,Alembic::Abc::kWrapExisting).getSchema().getNumSamples();
      else if(Alembic::AbcGeom::IPoints::matches(md))
        numCurrent = Alembic::AbcGeom::IPoints(obj,Alembic::Abc::kWrapExisting).getSchema().getNumSamples();
      else if(Alembic::AbcGeom::ISubD::matches(md))
        numCurrent = Alembic::AbcGeom::ISubD(obj,Alembic::Abc::kWrapExisting).getSchema().getNumSamples();
      else if(Alembic::AbcGeom::ICamera::matches(md))
        numCurrent = Alembic::AbcGeom::ICamera(obj,Alembic::Abc::kWrapExisting).getSchema().getNumSamples();

      if(numCurrent > handle.numSamples)
        handle.numSamples = numCurrent;

      for(size_t j=0;j<obj.getNumChildren();j++)
        iObjects.push_back(obj.getChild(j));
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
      const Alembic::Abc::MetaData &md = iObjects[i].getMetaData();
      if(Alembic::AbcGeom::IXform::matches(md))
        identifiers[offset] += "Xform";
      else if(Alembic::AbcGeom::IPolyMesh::matches(md))
        identifiers[offset] += "PolyMesh";
      else if(Alembic::AbcGeom::ICurves::matches(md))
        identifiers[offset] += "Curves";
      else if(Alembic::AbcGeom::INuPatch::matches(md))
        identifiers[offset] += "NuPatch";
      else if(Alembic::AbcGeom::IPoints::matches(md))
        identifiers[offset] += "Points";
      else if(Alembic::AbcGeom::ISubD::matches(md))
        identifiers[offset] += "SubD";
      else if(Alembic::AbcGeom::ICamera::matches(md))
        identifiers[offset] += "Camera";
      offset++;
    }
  }
}

FABRIC_EXT_EXPORT void FabricALEMBICParseXform(
  AlembicHandle &handle,
  KL::String & identifier,
  KL::Integer & sample,
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
    if(sample < 0)
      sample = 0;
    else if(sample >= KL::Integer(schema.getNumSamples()))
      sample = schema.getNumSamples()-1;
      
    // get the sample
    Alembic::AbcGeom::XformSample xform;
    schema.get(xform,sample);
    
    // extract the information
    float halfAngle = 0.5f * xform.getAngle() * 3.14f / 180.0f;
    transform.ori.v.x = xform.getAxis().x * sin(halfAngle);
    transform.ori.v.y = xform.getAxis().y * sin(halfAngle);
    transform.ori.v.z = xform.getAxis().z * sin(halfAngle);
    transform.ori.w = cos(halfAngle);
    transform.tr.x = xform.getTranslation().x;
    transform.tr.y = xform.getTranslation().y;
    transform.tr.z = xform.getTranslation().z;
    transform.sc.x = xform.getScale().x;
    transform.sc.y = xform.getScale().y;
    transform.sc.z = xform.getScale().z;
  }
}

FABRIC_EXT_EXPORT void FabricALEMBICParseCamera(
  AlembicHandle &handle,
  KL::String & identifier,
  KL::Integer & sample,
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
    if(sample < 0)
      sample = 0;
    else if(sample >= KL::Integer(schema.getNumSamples()))
      sample = schema.getNumSamples()-1;
      
    // get the sample
    Alembic::AbcGeom::CameraSample camera;
    schema.get(camera,sample);

    // clipping planes
    near = camera.getNearClippingPlane();    
    far = camera.getFarClippingPlane();
    
    // compute fovY from aspect and aperture
    KL::Scalar focallength = camera.getFocalLength();
    KL::Scalar aperture = camera.getVerticalAperture();
    fovY = 1.0f * atanf(50.0f * aperture / focallength);
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
    std::vector<KL::Integer> vecIndices;
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
  KL::Integer & sampleIndex,
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
    if(sampleIndex < 0)
      sampleIndex = 0;
    else if(sampleIndex >= KL::Integer(schema.getNumSamples()))
      sampleIndex = schema.getNumSamples()-1;
      
    // get the sample
    Alembic::AbcCoreAbstract::index_t timeIndex = (Alembic::AbcCoreAbstract::index_t)sampleIndex;
    Alembic::AbcGeom::IPolyMeshSchema::Sample firstSample = schema.getValue(0);
    Alembic::AbcGeom::IPolyMeshSchema::Sample sample = schema.getValue(timeIndex);
    Alembic::Abc::P3fArraySamplePtr abcPoints = sample.getPositions();
    Alembic::Abc::Int32ArraySamplePtr faceCounts = firstSample.getFaceCounts();
    Alembic::Abc::Int32ArraySamplePtr faceIndices= firstSample.getFaceIndices();
    
    // load the vertices
    size_t vertexOffset = 0;
    size_t indexOffset = 0;
    for(size_t i=0;i<faceCounts->size();i++)
    {
      size_t count = faceCounts->get()[i];
      if(count == 3)
      {
        for(int j=0;j<3;j++)
        {
          vertices[vertexOffset].x = abcPoints->get()[faceIndices->get()[indexOffset+j]].x;
          vertices[vertexOffset].y = abcPoints->get()[faceIndices->get()[indexOffset+j]].y;
          vertices[vertexOffset++].z = abcPoints->get()[faceIndices->get()[indexOffset+j]].z;
        }
      }
      else if(count == 4)
      {
        for(int j=0;j<3;j++)
        {
          vertices[vertexOffset].x = abcPoints->get()[faceIndices->get()[indexOffset+j]].x;
          vertices[vertexOffset].y = abcPoints->get()[faceIndices->get()[indexOffset+j]].y;
          vertices[vertexOffset++].z = abcPoints->get()[faceIndices->get()[indexOffset+j]].z;
        }
        for(int j=2;j<5;j++)
        {
          vertices[vertexOffset].x = abcPoints->get()[faceIndices->get()[indexOffset+(j%4)]].x;
          vertices[vertexOffset].y = abcPoints->get()[faceIndices->get()[indexOffset+(j%4)]].y;
          vertices[vertexOffset++].z = abcPoints->get()[faceIndices->get()[indexOffset+(j%4)]].z;
        }
      }
      else
        continue;
      indexOffset += count;
    }
    
    // load the normals
    Alembic::AbcGeom::IN3fGeomParam N = schema.getNormalsParam();
    if(N.valid())
    {
      Alembic::AbcGeom::N3fArraySamplePtr nsp = N.getExpandedValue(sampleIndex).getVals();
      
      if(nsp->size() > 0)
      {
        size_t vertexOffset = 0;
        size_t indexOffset = 0;
        for(size_t i=0;i<faceCounts->size();i++)
        {
          size_t count = faceCounts->get()[i];
          if(count == 3)
          {
            for(int j=0;j<3;j++)
            {
              normals[vertexOffset].x = nsp->get()[indexOffset+j].x;
              normals[vertexOffset].y = nsp->get()[indexOffset+j].y;
              normals[vertexOffset++].z = nsp->get()[indexOffset+j].z;
            }
          }
          else if(count == 4)
          {
            for(int j=0;j<3;j++)
            {
              normals[vertexOffset].x = nsp->get()[indexOffset+j].x;
              normals[vertexOffset].y = nsp->get()[indexOffset+j].y;
              normals[vertexOffset++].z = nsp->get()[indexOffset+j].z;
            }
            for(int j=2;j<5;j++)
            {
              normals[vertexOffset].x = nsp->get()[indexOffset+(j%4)].x;
              normals[vertexOffset].y = nsp->get()[indexOffset+(j%4)].y;
              normals[vertexOffset++].z = nsp->get()[indexOffset+(j%4)].z;
            }
          }
          else
            continue;
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
          size_t vertexOffset = 0;
          size_t indexOffset = 0;
          for(size_t i=0;i<faceCounts->size();i++)
          {
            size_t count = faceCounts->get()[i];
            if(count == 3)
            {
              for(int j=0;j<3;j++)
              {
                uvs[vertexOffset].x = usp->get()[indexOffset+j].x;
                uvs[vertexOffset++].y = 1.0f - usp->get()[indexOffset+j].y;
              }
            }
            else if(count == 4)
            {
              for(int j=0;j<3;j++)
              {
                uvs[vertexOffset].x = usp->get()[indexOffset+j].x;
                uvs[vertexOffset++].y = 1.0f - usp->get()[indexOffset+j].y;
              }
              for(int j=2;j<5;j++)
              {
                uvs[vertexOffset].x = usp->get()[indexOffset+(j%4)].x;
                uvs[vertexOffset++].y = 1.0f - usp->get()[indexOffset+(j%4)].y;
              }
            }
            else
              continue;
            indexOffset += count;
          }
        }
        uvsLoaded = true;
      }
    }
  }
}
