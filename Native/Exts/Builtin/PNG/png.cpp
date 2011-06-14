/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>

#include <libpng/png.h>

using namespace Fabric::EDK;

class DataBuffer
{
public:

  DataBuffer( unsigned size, void const *data )
    : m_size( size )
    , m_data( (uint8_t const *)data )
  {
  }
  
  void read( void *dst, unsigned count )
  {
    if ( count > m_size )
      throw Exception("short read");
    memcpy( dst, m_data, count );
    m_size -= count;
    m_data += count;
  }
  
  static void Read( png_structp png_ptr, png_bytep dst, png_size_t count )
  {
    static_cast<DataBuffer *>( png_get_io_ptr( png_ptr ) )->read( dst, count );
  }
  
private:

  unsigned m_size;
  uint8_t const *m_data;
};

FABRIC_EXT_EXPORT void FabricPNGDecode(
  KL::Data pngData,
  KL::Size pngDataSize,
  KL::Size &imageWidth,
  KL::Size &imageHeight,
  KL::VariableArray<KL::RGBA> &imagePixels
  )
{
  DataBuffer dataBuffer( pngDataSize, pngData );

  png_structp png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
  if ( !png_ptr )
    throw Exception("png_create_read_struct failed");

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if ( !info_ptr )
    throw Exception("png_create_info_struct failed");

  if ( setjmp( png_jmpbuf(png_ptr) ) )
    throw Exception("decode error");

  png_set_read_fn( png_ptr, &dataBuffer, &DataBuffer::Read );
  
  png_read_info( png_ptr, info_ptr );

  png_uint_32 width = 0;
  png_uint_32 height = 0;
  int bitDepth = 0;
  int colorType = -1;
  png_uint_32 retval = png_get_IHDR(
    png_ptr, info_ptr,
    &width, &height, &bitDepth, &colorType,
    NULL, NULL, NULL
    );
  if ( retval != 1 )
    throw Exception( "png_get_IHDR failed" );
  if ( colorType != PNG_COLOR_TYPE_RGB && colorType != PNG_COLOR_TYPE_RGB_ALPHA )
    throw Exception( "unsupported color type" );
  
  imageWidth = width;
  imageHeight = height;
  imagePixels.resize( width * height );
  
  const png_uint_32 bytesPerRow = png_get_rowbytes( png_ptr, info_ptr );
  uint8_t *rowData = (uint8_t *)alloca( bytesPerRow );

  size_t index = 0;
  for ( png_uint_32 rowIdx = 0; rowIdx < height; ++rowIdx )
  {
    png_read_row( png_ptr, (png_bytep)rowData, NULL );

    png_uint_32 byteIndex = 0;
    for ( png_uint_32 colIdx = 0; colIdx < width; ++colIdx )
    {
      KL::RGBA klRGBA;
      klRGBA.r = klRGBA.g = klRGBA.b = klRGBA.a = 255;
      if ( colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_RGB_ALPHA )
      {
        klRGBA.r = rowData[byteIndex++];
        klRGBA.g = rowData[byteIndex++];
        klRGBA.b = rowData[byteIndex++];
      }
      if ( colorType == PNG_COLOR_TYPE_RGB_ALPHA )
        klRGBA.a = rowData[byteIndex++];
      imagePixels[index++] = klRGBA;
    }
  }

  png_destroy_read_struct( &png_ptr, &info_ptr, NULL );
}
