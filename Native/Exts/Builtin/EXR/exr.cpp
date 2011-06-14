/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>

#include <ImfIO.h>
#include <ImfRgbaFile.h>
#include <ImfArray.h>
#include <ImathBox.h>

using namespace Fabric::EDK;

class MemIStream : public Imf::IStream
{
public:

  MemIStream( void const *data, size_t size )
    : IStream( "<in memory stream>" )
    , m_data( static_cast<uint8_t const *>( data ) )
    , m_size( size )
    , m_pos( 0 )
  {
  }
  
  virtual bool read( char c[], int n )
  {
    if ( m_pos + n <= m_size )
    {
      memcpy( c, &m_data[m_pos], n );
      m_pos += n;
      return true;
    }
    else return false;
  }
  
  virtual Imf::Int64 tellg()
  {
    return m_pos;
  }
  
  virtual void seekg( Imf::Int64 pos )
  {
    m_pos = pos;
  }
  
  virtual void clear()
  {
  }
  
private:

  uint8_t const *m_data;
  size_t m_size;
  size_t m_pos;
};
  

FABRIC_EXT_EXPORT void FabricEXRDecode(
  KL::Data exrData,
  KL::Size exrDataSize,
  KL::Size &imageWidth,
  KL::Size &imageHeight,
  KL::VariableArray<KL::Color> &imagePixels
  )
{
  MemIStream memIStream( exrData, exrDataSize );
  Imf::RgbaInputFile rgbaInputFile( memIStream );
  Imath::Box2i dw = rgbaInputFile.dataWindow();
  imageWidth = dw.max.x - dw.min.x + 1;
  imageHeight = dw.max.y - dw.min.y + 1;
  Imf::Array2D<Imf::Rgba> rgbaPixels;
  rgbaPixels.resizeErase( imageHeight, imageWidth );
  rgbaInputFile.setFrameBuffer( &rgbaPixels[0][0] - dw.min.x - dw.min.y * imageWidth, 1, imageWidth );
  rgbaInputFile.readPixels( dw.min.y, dw.max.y );
  
  imagePixels.resize( imageWidth*imageHeight );
  size_t index = 0;
  for ( size_t y=0; y<imageHeight; ++y )
  {
    for ( size_t x=0; x<imageWidth; ++x )
    {
      Imf::Rgba const &rgba = rgbaPixels[y][x];
      KL::Color &klColor = imagePixels[index++];
      klColor.r = rgba.r;
      klColor.g = rgba.g;
      klColor.b = rgba.b;
      klColor.a = rgba.a;
    }
  }
}
