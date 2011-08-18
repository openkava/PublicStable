/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>
#include "objparser.h"

#include <streambuf>

using namespace Fabric::EDK;

struct istreambuf : public std::basic_streambuf<char, std::char_traits<char> >
{
  istreambuf(char* buffer, std::streamsize bufferLength)
  {
    setg(buffer, buffer, buffer + bufferLength);
  }
};

FABRIC_EXT_EXPORT void FabricOBJDecode(
  KL::Data objData,
  KL::Size objDataSize,
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
  parsedDataHandle = new ObjParser(memstream);
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
    hadErrors = parser->HadErrors();
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
    const std::vector<std::string>& parseErrors = parser->GetErrors();
    errors.resize( parseErrors.size() );
    for( size_t i = 0; i < parseErrors.size(); ++i )
      errors[i] = parseErrors[i].c_str();
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

FABRIC_EXT_EXPORT void FabricOBJGetNbPoints(
  KL::Data parsedDataHandle,
  KL::Size& nbPoints
  )
{
  if( parsedDataHandle == NULL )
    nbPoints = 0;
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;
    nbPoints = parser->NbPoints();
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetPoints(
  KL::Data parsedDataHandle,
  KL::VariableArray<KL::Vec3>& points
  )
{
  if( parsedDataHandle == NULL )
    points.resize( 0 );
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbPoints = parser->NbPoints();
    points.resize( nbPoints );

    for( size_t i = 0; i < nbPoints; ++i )
    {
      V3 point = parser->GetPoint( i );
      KL::Vec3 klVec = { point.v[0], point.v[1], point.v[2] };
      points[i] = klVec;
    }
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetPointsSliced(
  KL::Data parsedDataHandle,
  KL::SlicedArray<KL::Vec3>& points
  )
{
  if ( parsedDataHandle != NULL )
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbPoints = parser->NbPoints();
    if ( points.size() != nbPoints )
      throw Exception( "FabricOBJGetPointsSliced: sliced array size mismatch" );

    for( size_t i = 0; i < nbPoints; ++i )
    {
      V3 point = parser->GetPoint( i );
      KL::Vec3 klVec = { point.v[0], point.v[1], point.v[2] };
      points[i] = klVec;
    }
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetNormals(
  KL::Data parsedDataHandle,
  KL::VariableArray<KL::Vec3>& normals
  )
{
  if( parsedDataHandle == NULL )
    normals.resize( 0 );
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbPoints = parser->NbPoints();
    normals.resize( nbPoints );

    for( size_t i = 0; i < nbPoints; ++i )
    {
      V3 normal = parser->GetNormal( i );
      KL::Vec3 klVec = { normal.v[0], normal.v[1], normal.v[2] };
      normals[i] = klVec;
    }
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetNormalsSliced(
  KL::Data parsedDataHandle,
  KL::SlicedArray<KL::Vec3>& normals
  )
{
  if ( parsedDataHandle )
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbPoints = parser->NbPoints();
    if ( normals.size() != nbPoints )
      throw Exception( "FabricOBJGetNormalsSliced: sliced array size mismatch" );

    for ( size_t i = 0; i < nbPoints; ++i )
    {
      V3 normal = parser->GetNormal( i );
      KL::Vec3 klVec = { normal.v[0], normal.v[1], normal.v[2] };
      normals[i] = klVec;
    }
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetTextureCoords(
  KL::Data parsedDataHandle,
  KL::VariableArray<KL::Vec2>& textureCoords
  )
{
  if( parsedDataHandle == NULL )
    textureCoords.resize( 0 );
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbPoints = parser->NbPoints();
    textureCoords.resize( nbPoints );

    for( size_t i = 0; i < nbPoints; ++i )
    {
      V2 texCoord = parser->GetTextureCoord( i );
      KL::Vec2 klVec = { texCoord.v[0], texCoord.v[1] };
      textureCoords[i] = klVec;
    }
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetTextureCoordsSliced(
  KL::Data parsedDataHandle,
  KL::SlicedArray<KL::Vec2>& textureCoords
  )
{
  if ( parsedDataHandle )
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbPoints = parser->NbPoints();
    if ( textureCoords.size() != nbPoints )
      throw Exception( "FabricOBJGetTextureCoordsSliced: sliced array size mismatch" );

    for ( size_t i = 0; i < nbPoints; ++i )
    {
      V2 texCoord = parser->GetTextureCoord( i );
      KL::Vec2 klVec = { texCoord.v[0], texCoord.v[1] };
      textureCoords[i] = klVec;
    }
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetNbTriangles(
  KL::Data parsedDataHandle,
  KL::Size& nbTriangles
  )
{
  if( parsedDataHandle == NULL )
    nbTriangles = 0;
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;
    nbTriangles = parser->NbTriangles();
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetTriangleIndices(
  KL::Data parsedDataHandle,
  KL::VariableArray<KL::Integer>& indices
  )
{
  if( parsedDataHandle == NULL )
    indices.resize( 0 );
  else
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbTriangles = parser->NbTriangles();
    indices.resize( nbTriangles * 3 );

    for( size_t i = 0; i < nbTriangles; ++i )
    {
      int t1, t2, t3;
      parser->GetTriangleIndices( i, t1, t2, t3 );
      indices[i*3] = t1;
      indices[i*3+1] = t2;
      indices[i*3+2] = t3;
    }
  }
}

FABRIC_EXT_EXPORT void FabricOBJGetTriangleIndicesSliced(
  KL::Data parsedDataHandle,
  KL::SlicedArray<KL::Integer>& indices
  )
{
  if ( parsedDataHandle )
  {
    ObjParser *parser = (ObjParser*)parsedDataHandle;

    size_t nbTriangles = parser->NbTriangles();
    if ( indices.size() != nbTriangles )
      throw Exception( "FabricOBJGetTriangleIndicesSliced: sliced array size mismatch" );

    for( size_t i = 0; i < nbTriangles; ++i )
    {
      int t1, t2, t3;
      parser->GetTriangleIndices( i, t1, t2, t3 );
      indices[i*3] = t1;
      indices[i*3+1] = t2;
      indices[i*3+2] = t3;
    }
  }
}
