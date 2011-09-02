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

FABRIC_EXT_EXPORT void FabricALEMBICDecode(
  KL::Data objData,
  KL::Size objDataSize,
  KL::Data &dataHandle
  )
{
  if( dataHandle == NULL )
  {
    std::string fileName(tmpnam(NULL));
      
    // save the file to disk
    FILE * file = fopen(fileName.c_str(),"wb");
    fwrite(objData,objDataSize,1,file);
    fclose(file);
    file = NULL;

    Alembic::Abc::IArchive localArchive(Alembic::AbcCoreHDF5::ReadArchive(),fileName.c_str());
    
    // load the file    
    Alembic::Abc::IArchive * archive = new Alembic::Abc::IArchive(
      Alembic::AbcCoreHDF5::ReadArchive(),fileName.c_str());

    // store to the handle
    dataHandle = archive;
  }
}

FABRIC_EXT_EXPORT void FabricALEMBICFreeData(
  KL::Data& dataHandle
  )
{
  if( dataHandle != NULL )
  {
    Alembic::Abc::IArchive * archive = (Alembic::Abc::IArchive *)dataHandle;
    delete(archive);
    dataHandle = NULL;
  }
}

FABRIC_EXT_EXPORT void FabricALEMBICGetIdentifiers(
  KL::Data dataHandle,
  KL::VariableArray<KL::String>& identifiers
  )
{
  identifiers.resize(0);
  if( dataHandle != NULL )
  {
    Alembic::Abc::IArchive * archive = (Alembic::Abc::IArchive *)dataHandle;
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
           Alembic::AbcGeom::IPolyMeshSchema::matches(md)||
           Alembic::AbcGeom::ICurvesSchema::matches(md)||
           Alembic::AbcGeom::IFaceSetSchema::matches(md)||
           Alembic::AbcGeom::INuPatchSchema::matches(md)||
           Alembic::AbcGeom::IPointsSchema::matches(md)||
           Alembic::AbcGeom::ISubDSchema::matches(md)||
           Alembic::AbcGeom::ICameraSchema::matches(md))
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
      if(Alembic::AbcGeom::IXformSchema::matches(md))
        identifiers[offset] += "Xform";
      else if(Alembic::AbcGeom::IPolyMeshSchema::matches(md))
        identifiers[offset] += "PolyMesh";
      else if(Alembic::AbcGeom::ICurvesSchema::matches(md))
        identifiers[offset] += "Curves";
      else if(Alembic::AbcGeom::IFaceSetSchema::matches(md))
        identifiers[offset] += "FaceSet";
      else if(Alembic::AbcGeom::INuPatchSchema::matches(md))
        identifiers[offset] += "NuPatch";
      else if(Alembic::AbcGeom::IPointsSchema::matches(md))
        identifiers[offset] += "Points";
      else if(Alembic::AbcGeom::ISubDSchema::matches(md))
        identifiers[offset] += "SubD";
      else if(Alembic::AbcGeom::ICameraSchema::matches(md))
        identifiers[offset] += "Camera";
      offset++;
    }
  }
}

Alembic::Abc::IObject getObjectFromIdentifier(Alembic::Abc::IArchive * archive, KL::String & identifier)
{
  // split the path
  std::vector<std::string> parts;
  std::string stdidentifier(identifier.data());
  boost::split(parts, stdidentifier, boost::is_any_of("/"));

  // recurves to find it
  Alembic::Abc::IObject obj = archive->getTop();
  for(size_t i=1;i<parts.size();i++)
  {
    obj = obj.getChild(parts[i]);
  }
    
  return obj; 
}

FABRIC_EXT_EXPORT void FabricALEMBICParseXform(
  KL::Data dataHandle,
  KL::String & identifier,
  KL::Integer & sample,
  KL::Xfo & transform
  )
{
  if( dataHandle != NULL )
  {
    Alembic::Abc::IArchive * archive = (Alembic::Abc::IArchive *)dataHandle;
    Alembic::AbcGeom::IXform obj(getObjectFromIdentifier(archive,identifier),Alembic::Abc::kWrapExisting);
    if(!obj.valid())
      return;
    
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

