/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>

//#include <boost/smart_ptr.hpp>
//#include <boost/format.hpp>
//#include <boost/variant.hpp>

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

    printf(" { ALEMBIC } fileName : %s\n",fileName.c_str());
      
    // save the file to disk
    FILE * file = fopen(fileName.c_str(),"wb");
    fwrite(objData,objDataSize,1,file);
    fclose(file);
    file = NULL;

    printf(" { ALEMBIC } fileName : %s saved of size %d.\n",fileName.c_str(),int(objDataSize));

    Alembic::Abc::IArchive localArchive(Alembic::AbcCoreHDF5::ReadArchive(),fileName.c_str());
    printf(" { ALEMBIC } archive: %s loaded.\n",fileName.c_str());
    
    // load the file    
    Alembic::Abc::IArchive * archive = new Alembic::Abc::IArchive(
      Alembic::AbcCoreHDF5::ReadArchive(),fileName.c_str());

    printf(" { ALEMBIC } archive: %s loaded into ptr.\n",fileName.c_str());
    
    // store to the handle
    dataHandle = archive;

    printf(" { ALEMBIC } dataHandle stored.\n",fileName.c_str());
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

FABRIC_EXT_EXPORT void FabricALEMBICGetObjects(
  KL::Data dataHandle,
  KL::VariableArray<KL::String>& objects
  )
{
  objects.resize(0);
  if( dataHandle != NULL )
  {
    printf(" { ALEMBIC } Parsing archive....\n");
    
    Alembic::Abc::IArchive * archive = (Alembic::Abc::IArchive *)dataHandle;
    std::vector<Alembic::Abc::IObject> iObjects;
    iObjects.push_back(archive->getTop());

    size_t offset = 0;
    for(size_t i=0;i<iObjects.size();i++)
    {
      Alembic::Abc::IObject obj(iObjects[i],Alembic::Abc::kWrapExisting);
      printf(" { ALEMBIC } IObject %d: '%s, NumChildren %d'\n",(int)i,obj.getFullName().c_str(),(int)obj.getNumChildren());
      
      for(size_t j=0;j<obj.getNumChildren();j++)
      {
        printf(" { ALEMBIC } IObject %d, Child %d\n",(int)i,(int)j);

        Alembic::Abc::IObject child = obj.getChild(j);

        printf(" { ALEMBIC } IObject %d, Child %d, name: '%s'\n",(int)i,(int)j,child.getName().c_str());

        const Alembic::Abc::MetaData &md = child.getMetaData();
        if(Alembic::AbcGeom::IXformSchema::matches(md) ||
           Alembic::AbcGeom::ICameraSchema::matches(md))
        {
          iObjects.push_back(child);
          offset++;
        }
      }
    }
    
    printf(" { ALEMBIC } Parsed archive.\n");
    printf(" { ALEMBIC } Found %d objects.\n",(int)(iObjects.size()-1));
    
    objects.resize(offset);
    offset = 0;
    for(size_t i=1;i<iObjects.size();i++)
    {
      objects[offset++] = iObjects[i].getFullName().c_str();
    }
  }
}

/*
int main(int argc, char ** argv)
{
  if(argc != 2)
  {
    printf("specify filename!\n");
    return 1;
  }
  std::string fileName(argv[1]);  

  printf(" { ALEMBIC } archive: %s loading....\n",fileName.c_str());

  {
    Alembic::Abc::IArchive localArchive(
      Alembic::AbcCoreHDF5::ReadArchive(),
      fileName.c_str(),
      Alembic::Abc::ErrorHandler::kThrowPolicy);
    printf(" { ALEMBIC } archive: %s loaded.\n",localArchive.getName().c_str());
  }

  return 0;
}
*/