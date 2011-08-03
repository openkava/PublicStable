/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <iostream>
#include <vector>
#include "simplevector.h"

class ObjParser
{
public:
  ObjParser( std::istream& stream );

  bool HadErrors()const{ return !m_errors.empty(); }
  const std::vector<std::string>& GetErrors()const{ return m_errors; }

  bool HasTextureCoords()const{ return !m_texCoords.empty(); }

  size_t NbPoints()const{ return m_points.size(); }

  V3 GetPoint(int ptIndex)const;
  V3 GetNormal(int ptIndex)const;
  V2 GetTextureCoord(int ptIndex)const;

  size_t NbTriangles()const{ return m_triangleIndices.size()/3; }

  void GetTriangleIndices(int triIndex, int& i1, int& i2, int& i3)const;

private:
  void ParseV( std::istream& stream );
  void ParseN( std::istream& stream );
  void ParseT( std::istream& stream );
  void ParseF( std::istream& stream );

  void ComputeMissingNormals();

  std::vector<std::string> m_errors;

  std::vector<V3> m_points, m_normals;
  std::vector<V2> m_texCoords;

  struct PointIndices
  {
    PointIndices()
      : m_point(-1)
      , m_normal(-1)
      , m_texCoord(-1)
    {}

    bool isInitialized()
    {
      return m_point != -1;
    }

    bool operator==( const PointIndices& other )const
    {
      return m_point == other.m_point && m_normal == other.m_normal && m_texCoord == other.m_texCoord;
    }
    bool operator!=( const PointIndices& other )const
    {
      return !(*this == other);
    }

    int m_point;
    int m_normal;
    int m_texCoord;
  };

  std::vector<PointIndices> m_triangleIndices;
  std::vector<PointIndices> m_initialPointIndices;//To try maintaining trivial sharing
};
