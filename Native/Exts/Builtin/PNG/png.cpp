/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>

#include <libpng/png.h>

using namespace Fabric::EDK;
IMPLEMENT_FABRIC_EDK_ENTRIES

class ReadDataBuffer
{
public:

  ReadDataBuffer( unsigned size, void const *data )
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
    static_cast<ReadDataBuffer *>( png_get_io_ptr( png_ptr ) )->read( dst, count );
  }

private:

  unsigned m_size;
  uint8_t const *m_data;
};

class WriteDataBuffer
{
public:

  WriteDataBuffer( KL::VariableArray<KL::Byte> &data )
    : m_data( data )
  {
  }
  
  void write( const void *src, unsigned count )
  {
    size_t prevSize = m_data.size();
    m_data.resize( prevSize + count );
    memcpy( &m_data[prevSize], src, count );
  }
  
  static void Write( png_structp png_ptr, png_bytep src, png_size_t count )
  {
    static_cast<WriteDataBuffer *>( png_get_io_ptr( png_ptr ) )->write( src, count );
  }

  static void Flush( png_structp png_ptr ){}

private:

  KL::VariableArray<KL::Byte>& m_data;
};

FABRIC_EXT_EXPORT void FabricPNGDecode(
  KL::Data pngData,
  KL::Size pngDataSize,
  KL::Size &imageWidth,
  KL::Size &imageHeight,
  KL::VariableArray<KL::RGBA> &imagePixels
  )
{
  ReadDataBuffer dataBuffer( pngDataSize, pngData );

  png_structp png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
  if ( !png_ptr )
    throw Exception("png_create_read_struct failed");

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if ( !info_ptr )
    throw Exception("png_create_info_struct failed");

  if ( setjmp( png_jmpbuf(png_ptr) ) )
    throw Exception("decode error");

  png_set_read_fn( png_ptr, &dataBuffer, &ReadDataBuffer::Read );
  
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

FABRIC_EXT_EXPORT void FabricPNGEncode(
  KL::Size imageWidth,
  KL::Size imageHeight,
  KL::VariableArray<KL::RGBA> imagePixels,
  KL::VariableArray<KL::Byte> &pngData
  )
{
  WriteDataBuffer dataBuffer( pngData );

  png_structp png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
  if ( !png_ptr )
    throw Exception("png_create_write_struct failed");

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if ( !info_ptr )
    throw Exception("png_create_info_struct failed");
  

  png_set_write_fn( png_ptr, &dataBuffer, &WriteDataBuffer::Write, &WriteDataBuffer::Flush );

  if( setjmp( png_jmpbuf(png_ptr) ) )
    throw Exception("init io error");

  png_set_IHDR(png_ptr, info_ptr, imageWidth, imageHeight,
                8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_bytep *rowPointers = (png_bytep *)alloca( sizeof(png_bytep)*imageHeight );
  for( size_t i = 0; i < imageHeight; ++i )
    rowPointers[i] = (png_bytep)&imagePixels[ i*imageWidth ];

  png_set_rows(png_ptr, info_ptr, rowPointers);

  png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

  png_destroy_write_struct( &png_ptr, &info_ptr );

  KL::Size readImageWidth;
  KL::Size readImageHeight;
  KL::VariableArray<KL::RGBA> readImagePixels;

  FabricPNGDecode(
    &pngData[0],
    pngData.size(),
    readImageWidth,
    readImageHeight,
    readImagePixels);
}
