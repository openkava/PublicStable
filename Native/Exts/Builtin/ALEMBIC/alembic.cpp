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
    transform.ori.v.x = xform.getAxis().x * sin(xform.getAngle() * 0.5);
    transform.ori.v.y = xform.getAxis().y * sin(xform.getAngle() * 0.5);
    transform.ori.v.z = xform.getAxis().z * sin(xform.getAngle() * 0.5);
    transform.ori.w = cos(xform.getAngle() * 0.5);
    transform.tr.x = xform.getTranslation().x;
    transform.tr.y = xform.getTranslation().y;
    transform.tr.z = xform.getTranslation().z;
    transform.sc.x = xform.getScale().x;
    transform.sc.y = xform.getScale().y;
    transform.sc.z = xform.getScale().z;
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
    Alembic::Abc::Int32ArraySamplePtr faceIndices = sample.getFaceIndices();
    Alembic::Abc::Int32ArraySamplePtr faceCounts = sample.getFaceCounts();
    
    // add all of the faces that we can support
    std::vector<KL::Integer> vecIndices;
    size_t offset = 0;
    for(size_t i=0;i<faceCounts->size();i++)
    {
      size_t count = (*faceCounts)[i];
      if(count == 3)
      {
        vecIndices.push_back((*faceIndices)[offset++]);
        vecIndices.push_back((*faceIndices)[offset++]);
        vecIndices.push_back((*faceIndices)[offset++]);
      }
      else if(count == 4)
      {
        vecIndices.push_back((*faceIndices)[offset]);
        vecIndices.push_back((*faceIndices)[offset+1]);
        vecIndices.push_back((*faceIndices)[offset+2]);
        vecIndices.push_back((*faceIndices)[offset+2]);
        vecIndices.push_back((*faceIndices)[offset+3]);
        vecIndices.push_back((*faceIndices)[offset]);
        offset += 4;
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

    // store the slice count
    count = sample.getPositions()->size();
  }
}

FABRIC_EXT_EXPORT void FabricALEMBICParsePolyMeshAttributes(
  KL::Integer archiveID,
  KL::String & identifier,
  KL::Integer & sampleIndex,
  KL::VariableArray<KL::Vec3>& vertices,
  KL::VariableArray<KL::Vec3>& normals
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
    Alembic::AbcGeom::IPolyMeshSchema::Sample sample;
    schema.get(sample,sampleIndex);

    // load the vertices
    if(vertices.size() == sample.getPositions()->size())
      memcpy(&vertices[0],sample.getPositions()->getData(),sizeof(float) * 3 * vertices.size());
    
    
    // load the normals
    Alembic::AbcGeom::IN3fGeomParam N = schema.getNormalsParam();
    if(N.valid())
    {
      Alembic::AbcGeom::N3fArraySamplePtr nsp = N.getExpandedValue().getVals();
      printf(" { ALEMBIC } Num Vertices : %d\n",(int)vertices.size());
      printf(" { ALEMBIC } Num Normals : %d\n",(int)nsp->size());
    }

    //IV2fGeomParam uv = schema.getUVsParam();
  }
}

int main(int argc, char ** argv)
{
  
printf("Opening archive...\n");
  KL::Integer id = addArchive(new Alembic::Abc::IArchive(Alembic::AbcCoreHDF5::ReadArchive(),"/development/octopus.abc"));
  
  printf("Opened archive.\n");
  
  std::string identifier = "/octopus_low/octopus_lowShape";
  
  printf("Looking for identifier...\n");
  
  Alembic::AbcGeom::IPolyMesh obj(getObjectFromArchive(id,identifier.data()),Alembic::Abc::kWrapExisting);
  Alembic::AbcGeom::IXform xform(obj.getParent(),Alembic::Abc::kWrapExisting);
  printf("found object: %s\n",obj.getFullName().c_str());
  
  printf("Requesting schema...\n");
  
  Alembic::AbcGeom::IPolyMeshSchema schema = obj.getSchema();
  
  printf("Nb Samples: %d\n",(int)schema.getNumSamples());
  printf("Requesting sample...\n");
  
  Alembic::AbcGeom::IPolyMeshSchema::Sample sample;
  schema.get(sample);
  
  printf("Received sample.\n");
  
  printf("Nb Points: %d\n",(int)sample.getPositions()->size());
  
  deleteArchive(id);

  return 0;
}
