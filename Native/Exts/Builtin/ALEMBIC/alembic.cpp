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

std::map<KL::Integer,Alembic::Abc::IArchive *> gArchives;

Alembic::Abc::IArchive * getArchiveFromID(KL::Integer id)
{
  std::map<KL::Integer,Alembic::Abc::IArchive *>::iterator it;
  it = gArchives.find(id);
  if(it == gArchives.end())
    return NULL;
  
  return it->second;
}

KL::Integer addArchive(Alembic::Abc::IArchive * archive)
{
  KL::Integer id = 0;
  std::map<KL::Integer,Alembic::Abc::IArchive *>::iterator it;
  for(it = gArchives.begin();it != gArchives.end(); it++)
  {
    id++;
    if(it->first > id)
      break;
  }
  gArchives.insert(std::pair<KL::Integer,Alembic::Abc::IArchive *>(id,archive));
  return id;  
}

void deleteArchive(KL::Integer id)
{
  std::map<KL::Integer,Alembic::Abc::IArchive *>::iterator it;
  it = gArchives.find(id);
  if(it == gArchives.end())
    return;
  delete(it->second);
  gArchives.erase(it);
}

bool usesVertexNormals(Alembic::AbcGeom::IPolyMeshSchema & schema, Alembic::AbcGeom::IPolyMeshSchema::Sample & sample)
{
  Alembic::Abc::P3fArraySamplePtr abcPoints = sample.getPositions();
  Alembic::AbcGeom::IN3fGeomParam N = schema.getNormalsParam();
  if(!N.valid())
    return true;

  Alembic::AbcGeom::N3fArraySamplePtr nsp = N.getExpandedValue().getVals();
  return nsp->size() == abcPoints->size();
}

Alembic::Abc::IObject getObjectFromArchive(KL::Integer id, const std::string & identifier)
{
  Alembic::Abc::IArchive * archive = getArchiveFromID(id);
  if(archive == NULL)
    return Alembic::Abc::IObject();
  
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
  KL::Integer &archiveID
  )
{
  Alembic::Abc::IArchive * archive = getArchiveFromID(archiveID);
  if( archive == NULL )
  {
    std::string fileName(tmpnam(NULL));
      
    // save the file to disk
    FILE * file = fopen(fileName.c_str(),"wb");
    fwrite(objData,objDataSize,1,file);
    fclose(file);
    file = NULL;

    // load the file    
    archive = new Alembic::Abc::IArchive(
      Alembic::AbcCoreHDF5::ReadArchive(),fileName.c_str());

    archiveID = addArchive(archive);
  }
}

FABRIC_EXT_EXPORT void FabricALEMBICFreeData(
  KL::Integer& archiveID
  )
{
  deleteArchive(archiveID);
  archiveID = -1;
}

FABRIC_EXT_EXPORT void FabricALEMBICGetIdentifiers(
  KL::Integer archiveID,
  KL::VariableArray<KL::String>& identifiers
  )
{
  identifiers.resize(0);
  Alembic::Abc::IArchive * archive = getArchiveFromID(archiveID);
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
           Alembic::AbcGeom::IFaceSet::matches(md)||
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
      else if(Alembic::AbcGeom::IFaceSet::matches(md))
        identifiers[offset] += "FaceSet";
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
  KL::Integer archiveID,
  KL::String & identifier,
  KL::Integer & sample,
  KL::Xfo & transform
  )
{
  Alembic::AbcGeom::IXform obj(getObjectFromArchive(archiveID,identifier.data()),Alembic::Abc::kWrapExisting);
  if( obj.valid() )
  {
    // get the schema
    Alembic::AbcGeom::IXformSchema schema = obj.getSchema();
    if(sample < 0)
      sample = 0;
    else if(sample >= schema.getNumSamples())
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
  KL::Integer archiveID,
  KL::String & identifier,
  KL::Integer & sample,
  KL::Scalar & near,
  KL::Scalar & far,
  KL::Scalar & fovY
  )
{
  Alembic::AbcGeom::ICamera obj(getObjectFromArchive(archiveID,identifier.data()),Alembic::Abc::kWrapExisting);
  if( obj.valid() )
  {
    // get the schema
    Alembic::AbcGeom::ICameraSchema schema = obj.getSchema();
    if(sample < 0)
      sample = 0;
    else if(sample >= schema.getNumSamples())
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
  KL::Integer archiveID,
  KL::String & identifier,
  KL::VariableArray<KL::Integer>& indices
  )
{
  Alembic::AbcGeom::IPolyMesh obj(getObjectFromArchive(archiveID,identifier.data()),Alembic::Abc::kWrapExisting);
  if( obj.valid() )
  {
    // get the schema
    Alembic::AbcGeom::IPolyMeshSchema schema = obj.getSchema();

    // get the sample
    Alembic::AbcGeom::IPolyMeshSchema::Sample sample;
    schema.get(sample,0);

    // store the indices
    std::vector<KL::Integer> vecIndices;
    if(!usesVertexNormals(schema,sample))
    {
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
    }
    else
    {
      Alembic::Abc::Int32ArraySamplePtr faceCounts = sample.getFaceCounts();
      Alembic::Abc::Int32ArraySamplePtr faceIndices = sample.getFaceIndices();

      // add all of the faces that we can support
      size_t offset = 0;
      for(size_t i=0;i<faceCounts->size();i++)
      {
        size_t count = faceCounts->get()[i];
        if(count == 3)
        {
          vecIndices.push_back(faceIndices->get()[offset+2]);
          vecIndices.push_back(faceIndices->get()[offset+1]);
          vecIndices.push_back(faceIndices->get()[offset]);
          offset += 3;
        }
        else if(count == 4)
        {
          vecIndices.push_back(faceIndices->get()[offset+2]);
          vecIndices.push_back(faceIndices->get()[offset+1]);
          vecIndices.push_back(faceIndices->get()[offset]);
          
          vecIndices.push_back(faceIndices->get()[offset]);
          vecIndices.push_back(faceIndices->get()[offset+3]);
          vecIndices.push_back(faceIndices->get()[offset+2]);
          offset += 4;
        }
      }
    }
    
    // finally copy them out
    indices.resize(vecIndices.size());
    memcpy(&indices[0],&vecIndices[0],sizeof(KL::Integer) * vecIndices.size());
  }
}

FABRIC_EXT_EXPORT void FabricALEMBICParsePolyMeshCount(
  KL::Integer archiveID,
  KL::String & identifier,
  KL::Size & count
  )
{
  Alembic::AbcGeom::IPolyMesh obj(getObjectFromArchive(archiveID,identifier.data()),Alembic::Abc::kWrapExisting);
  if( obj.valid() )
  {
    // get the schema
    Alembic::AbcGeom::IPolyMeshSchema schema = obj.getSchema();
      
    // get the sample
    Alembic::AbcGeom::IPolyMeshSchema::Sample sample;
    schema.get(sample,0);

    if(usesVertexNormals(schema,sample))
    {
      // store the slice count
      count = sample.getPositions()->size();
    }
    else
    {
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
}

FABRIC_EXT_EXPORT void FabricALEMBICParsePolyMeshAttributes(
  KL::Integer archiveID,
  KL::String & identifier,
  KL::Integer & sampleIndex,
  KL::SlicedArray<KL::Vec3>& vertices,
  KL::SlicedArray<KL::Vec3>& normals,
  KL::Boolean & uvsLoaded,
  KL::SlicedArray<KL::Vec2>& uvs
  )
{
  Alembic::AbcGeom::IPolyMesh obj(getObjectFromArchive(archiveID,identifier.data()),Alembic::Abc::kWrapExisting);
  if( obj.valid() )
  {
    // get the schema
    Alembic::AbcGeom::IPolyMeshSchema schema = obj.getSchema();
    if(sampleIndex < 0)
      sampleIndex = 0;
    else if(sampleIndex >= schema.getNumSamples())
      sampleIndex = schema.getNumSamples()-1;
      
    // get the sample
    Alembic::AbcCoreAbstract::index_t timeIndex = (Alembic::AbcCoreAbstract::index_t)sampleIndex;
    Alembic::AbcGeom::IPolyMeshSchema::Sample firstSample = schema.getValue(0);
    Alembic::AbcGeom::IPolyMeshSchema::Sample sample = schema.getValue(timeIndex);
    Alembic::Abc::P3fArraySamplePtr abcPoints = sample.getPositions();
    Alembic::Abc::Int32ArraySamplePtr faceCounts = firstSample.getFaceCounts();
    Alembic::Abc::Int32ArraySamplePtr faceIndices= firstSample.getFaceIndices();
    
    // load the vertices
    bool vertexNormals = usesVertexNormals(schema,firstSample);
    if(vertexNormals)
    {
      memcpy(&vertices[0],abcPoints->getData(),sizeof(float) * 3 * vertices.size());
    }
    else if(!vertexNormals)
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
    }
    
    // load the normals
    Alembic::AbcGeom::IN3fGeomParam N = schema.getNormalsParam();
    if(N.valid())
    {
      Alembic::AbcGeom::N3fArraySamplePtr nsp = N.getExpandedValue(sampleIndex).getVals();
      
      if(vertexNormals && normals.size() == nsp->size())
      {
        memcpy(&normals[0],nsp->getData(),sizeof(float) * 3 * normals.size());
      }
      else if(!vertexNormals && nsp->size() > 0)
      {
        size_t vertexOffset = 0;
        size_t indexOffset = 0;
        for(size_t i=0;i<faceCounts->size();i++)
        {
          size_t count = faceCounts->get()[i];
          if(count == 3)
          {
            for(int j=2;j>=0;j--)
            {
              normals[vertexOffset].x = nsp->get()[indexOffset+j].x;
              normals[vertexOffset].y = nsp->get()[indexOffset+j].y;
              normals[vertexOffset++].z = nsp->get()[indexOffset+j].z;
            }
          }
          else if(count == 4)
          {
            for(int j=2;j>=0;j--)
            {
              normals[vertexOffset].x = nsp->get()[indexOffset+j].x;
              normals[vertexOffset].y = nsp->get()[indexOffset+j].y;
              normals[vertexOffset++].z = nsp->get()[indexOffset+j].z;
            }
            for(int j=4;j>=2;j--)
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
        if(vertexNormals && uvs.size() == usp->size())
        {
          for(size_t i=0;i<usp->size();i++)
          {
            uvs[i].x = usp->get()[i].x;
            uvs[i].y = 1.0f - usp->get()[i].y;
          }
          //memcpy(&uvs[0],usp->getData(),sizeof(float) * 2 * uvs.size());
        }
        else if(usp->size() > 0)
        {
          size_t vertexOffset = 0;
          size_t indexOffset = 0;
          for(size_t i=0;i<faceCounts->size();i++)
          {
            size_t count = faceCounts->get()[i];
            if(count == 3)
            {
              for(int j=2;j>=0;j--)
              {
                uvs[vertexOffset].x = usp->get()[indexOffset+j].x;
                uvs[vertexOffset++].y = 1.0f - usp->get()[indexOffset+j].y;
              }
            }
            else if(count == 4)
            {
              for(int j=2;j>=0;j--)
              {
                uvs[vertexOffset].x = usp->get()[indexOffset+j].x;
                uvs[vertexOffset++].y = 1.0f - usp->get()[indexOffset+j].y;
              }
              for(int j=4;j>=2;j--)
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
