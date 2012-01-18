/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>
#include "objparser.h"

#include <streambuf>

using namespace Fabric::EDK;
IMPLEMENT_FABRIC_EDK_ENTRIES

struct istreambuf : public std::basic_streambuf<char, std::char_traits<char> >
{
  istreambuf(char* buffer, std::streamsize bufferLength)
  {
    setg(buffer, buffer, buffer + bufferLength);
  }
};

void EDKExceptionThrow(const char* msg) {
  Fabric::EDK::throwException( msg );
}

FABRIC_EXT_EXPORT void FabricOBJDecode(
  KL::Data objData,
  KL::Size objDataSize,
  KL::Boolean splitByObjects,
  KL::Boolean splitByGroups,
  KL::Boolean splitByMaterials,
  KL::Data &parsedDataHandle
  )
{
  istreambuf memstreambuff( (char*)objData,objDataSize );
  std::istream memstream( &memstreambuff );

  if( parsedDataHandle != NULL )
  {
    //Dangerous, but ok for current tests...
    ObjParser *parser = (ObjParser*)parsedDataHandle;
    delete parser;
    parsedDataHandle = NULL;
  }
  parsedDataHandle = new ObjParser(memstream, splitByObjects, splitByGroups, splitByMaterials, EDKExceptionThrow);
}

FABRIC_EXT_EXPORT void FabricOBJFreeParsedData(
  KL::Data& parsedDataHandle
  )
{
  if( parsedDataHandle != NULL )
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;
    delete parser;
    parsedDataHandle = NULL;
  }
}

FABRIC_EXT_EXPORT void FabricOBJIsHandleValid(
  KL::Data parsedDataHandle,
  KL::Boolean& valid
  )
{
  valid = parsedDataHandle != NULL;
}

FABRIC_EXT_EXPORT void FabricOBJHadErrors(
  KL::Data parsedDataHandle,
  KL::Boolean& hadErrors
  )
{
  if( parsedDataHandle == NULL )
    hadErrors = false;
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;
    hadErrors = parser->GetNbErrors() > 0;
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetErrors(
  KL::Data parsedDataHandle,
  KL::VariableArray<KL::String>& errors
  )
{
  if( parsedDataHandle == NULL )
    errors.resize( 0 );
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;
    size_t nb = parser->GetNbErrors();
    errors.resize( nb );
    for( size_t i = 0; i < nb; ++i )
      errors[i] = parser->GetError(i).c_str();
  }
}

FABRIC_EXT_EXPORT void FabricOBJHasTextureCoords(
  KL::Data parsedDataHandle,
  KL::Boolean& hasTextureCoords
  )
{
  if( parsedDataHandle == NULL )
    hasTextureCoords = false;
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;
    hasTextureCoords = parser->HasTextureCoords();
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetMaterialLibraries(
  KL::Data parsedDataHandle,
  KL::VariableArray<KL::String>& materialLibraries
  )
{
  if( parsedDataHandle == NULL )
    materialLibraries.resize(0);
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;
    size_t nb = parser->GetNbMaterialLibraries();
    materialLibraries.resize( nb );
    for( size_t i = 0; i < nb; ++i )
      materialLibraries[i] = parser->GetMaterialLibrary(i).c_str();
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetMaterialNames(
  KL::Data parsedDataHandle,
  KL::VariableArray<KL::String>& materialNames
  )
{
  if( parsedDataHandle == NULL )
    materialNames.resize(0);
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;
    size_t nb = parser->GetNbMaterials();
    materialNames.resize( nb );
    for( size_t i = 0; i < nb; ++i )
      materialNames[i] = parser->GetMaterial(i).c_str();
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetNbEntities(
  KL::Data parsedDataHandle,
  KL::Size& nbEntities
  )
{
  if( parsedDataHandle == NULL )
    nbEntities = 0;
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;
    nbEntities = parser->GetNbEntities();
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetEntityObjectName(
  KL::Data parsedDataHandle,
  KL::Integer entityIndex,
  KL::String& objectName
  )
{
  if( parsedDataHandle == NULL )
    objectName = "";
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;
    objectName = parser->GetEntityObjectName(entityIndex).c_str();
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetEntityGroupName(
  KL::Data parsedDataHandle,
  KL::Integer entityIndex,
  KL::String& objectName
  )
{
  if( parsedDataHandle == NULL )
    objectName = "";
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;
    objectName = parser->GetEntityGroupName(entityIndex).c_str();
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetEntityMaterialName(
  KL::Data parsedDataHandle,
  KL::Integer entityIndex,
  KL::String& objectName
  )
{
  if( parsedDataHandle == NULL )
    objectName = "";
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;
    objectName = parser->GetEntityMaterialName(entityIndex).c_str();
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetNbEntityPoints(
  KL::Data parsedDataHandle,
  KL::Integer entityIndex,
  KL::Size& nbPoints
  )
{
  if( parsedDataHandle == NULL )
    nbPoints = 0;
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;
    nbPoints = parser->GetNbEntityPoints(entityIndex);
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetEntityPoints(
  KL::Data parsedDataHandle,
  KL::Integer entityIndex,
  KL::VariableArray<KL::Vec3>& points
  )
{
  if( parsedDataHandle == NULL )
    points.resize( 0 );
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbPoints = parser->GetNbEntityPoints(entityIndex);
    points.resize( nbPoints );

    for( size_t i = 0; i < nbPoints; ++i )
    {
      V3 point = parser->GetEntityPoint( entityIndex, i );
      KL::Vec3 klVec = { point.v[0], point.v[1], point.v[2] };
      points[i] = klVec;
    }
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetEntityPointsSliced(
  KL::Data parsedDataHandle,
  KL::Integer entityIndex,
  KL::SlicedArray<KL::Vec3>& points
  )
{
  if ( parsedDataHandle != NULL )
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbPoints = parser->GetNbEntityPoints(entityIndex);
    if ( points.size() != nbPoints )
      throwException( "FabricOBJGetEntityPointsSliced: sliced array size mismatch" );

    for( size_t i = 0; i < nbPoints; ++i )
    {
      V3 point = parser->GetEntityPoint( entityIndex, i );
      KL::Vec3 klVec = { point.v[0], point.v[1], point.v[2] };
      points[i] = klVec;
    }
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetEntityNormals(
  KL::Data parsedDataHandle,
  KL::Integer entityIndex,
  KL::VariableArray<KL::Vec3>& normals
  )
{
  if( parsedDataHandle == NULL )
    normals.resize( 0 );
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbPoints = parser->GetNbEntityPoints(entityIndex);
    normals.resize( nbPoints );

    for( size_t i = 0; i < nbPoints; ++i )
    {
      V3 normal = parser->GetEntityNormal( entityIndex, i );
      KL::Vec3 klVec = { normal.v[0], normal.v[1], normal.v[2] };
      normals[i] = klVec;
    }
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetEntityNormalsSliced(
  KL::Data parsedDataHandle,
  KL::Integer entityIndex,
  KL::SlicedArray<KL::Vec3>& normals
  )
{
  if ( parsedDataHandle )
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbPoints = parser->GetNbEntityPoints(entityIndex);
    if ( normals.size() != nbPoints )
      throwException( "FabricOBJGetEntityNormalsSliced: sliced array size mismatch" );

    for ( size_t i = 0; i < nbPoints; ++i )
    {
      V3 normal = parser->GetEntityNormal( entityIndex, i );
      KL::Vec3 klVec = { normal.v[0], normal.v[1], normal.v[2] };
      normals[i] = klVec;
    }
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetEntityTextureCoords(
  KL::Data parsedDataHandle,
  KL::Integer entityIndex,
  KL::VariableArray<KL::Vec2>& textureCoords
  )
{
  if( parsedDataHandle == NULL )
    textureCoords.resize( 0 );
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbPoints = parser->GetNbEntityPoints(entityIndex);
    textureCoords.resize( nbPoints );

    for( size_t i = 0; i < nbPoints; ++i )
    {
      V2 texCoord = parser->GetEntityTextureCoord( entityIndex, i );
      KL::Vec2 klVec = { texCoord.v[0], texCoord.v[1] };
      textureCoords[i] = klVec;
    }
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetEntityTextureCoordsSliced(
  KL::Data parsedDataHandle,
  KL::Integer entityIndex,
  KL::SlicedArray<KL::Vec2>& textureCoords
  )
{
  if ( parsedDataHandle )
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;


    size_t nbPoints = parser->GetNbEntityPoints(entityIndex);
    if ( textureCoords.size() != nbPoints )
      throwException( "FabricOBJGetEntityTextureCoordsSliced: sliced array size mismatch" );

    for ( size_t i = 0; i < nbPoints; ++i )
    {
      V2 texCoord = parser->GetEntityTextureCoord( entityIndex, i );
      KL::Vec2 klVec = { texCoord.v[0], 1.0-texCoord.v[1] };//OBJ considers (0, 0) to be the top left of a texture, OpenGL considers it to be the bottom left
      textureCoords[i] = klVec;
    }
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetNbEntityTriangles(
  KL::Data parsedDataHandle,
  KL::Integer entityIndex,
  KL::Size& nbTriangles
  )
{
  if( parsedDataHandle == NULL )
    nbTriangles = 0;
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;
    nbTriangles = parser->GetNbEntityTriangles(entityIndex);
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetEntityTriangleIndices(
  KL::Data parsedDataHandle,
  KL::Integer entityIndex,
  KL::VariableArray<KL::Integer>& indices
  )
{
  if( parsedDataHandle == NULL )
    indices.resize( 0 );
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbTriangles = parser->GetNbEntityTriangles(entityIndex);
    indices.resize( nbTriangles * 3 );

    for( size_t i = 0; i < nbTriangles; ++i )
    {
      int t1, t2, t3;
      parser->GetEntityTriangleIndices( entityIndex, i, t1, t2, t3 );
      indices[i*3] = t1;
      indices[i*3+1] = t2;
      indices[i*3+2] = t3;
    }
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetEntityTriangleIndicesSliced(
  KL::Data parsedDataHandle,
  KL::Integer entityIndex,
  KL::SlicedArray<KL::Integer>& indices
  )
{
  if ( parsedDataHandle )
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbTriangles = parser->GetNbEntityTriangles(entityIndex);
    if ( indices.size() != nbTriangles )
      throwException( "FabricOBJGetEntityTriangleIndicesSliced: sliced array size mismatch" );

    for( size_t i = 0; i < nbTriangles; ++i )
    {
      int t1, t2, t3;
      parser->GetEntityTriangleIndices( entityIndex, i, t1, t2, t3 );
      indices[i*3] = t1;
      indices[i*3+1] = t2;
      indices[i*3+2] = t3;
    }
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetEntityTriangleMaterialIndices(
  KL::Data parsedDataHandle,
  KL::Integer entityIndex,
  KL::VariableArray<KL::Integer>& indices
  )
{
  if( parsedDataHandle == NULL )
    indices.resize( 0 );
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbTriangles = parser->GetNbEntityTriangles(entityIndex);
    indices.resize( nbTriangles );

    for( size_t i = 0; i < nbTriangles; ++i )
      indices[i] = parser->GetEntityTriangleMaterialIndex( entityIndex, i );
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetEntityTriangleMaterialIndicesSliced(
  KL::Data parsedDataHandle,
  KL::Integer entityIndex,
  KL::SlicedArray<KL::Integer>& indices
  )
{
  if ( parsedDataHandle )
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbTriangles = parser->GetNbEntityTriangles(entityIndex);
    if ( indices.size() != nbTriangles )
      throwException( "FabricOBJGetEntityTriangleIndicesSliced: sliced array size mismatch" );

    for( size_t i = 0; i < nbTriangles; ++i )
      indices[i] = parser->GetEntityTriangleMaterialIndex( entityIndex, i );
  }
}
