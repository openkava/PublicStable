/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_UTIL_LITTLE_ENDIAN_BUFFER_DECODER_H
#define _FABRIC_UTIL_LITTLE_ENDIAN_BUFFER_DECODER_H

#include <Fabric/Core/Util/Decoder.h>
#include <Fabric/Core/Util/Endian.h>
#include <stdint.h>
#include <vector>

namespace Fabric
{
  namespace Util
  {
    class LittleEndianBufferDecoder : public Decoder
    {
    public:
    
      LittleEndianBufferDecoder( std::vector< uint8_t > const &buffer )
        : m_buffer( buffer )
        , m_offset( 0 )
      {
        FABRIC_UTIL_TRACE( "Util::LittleEndianBufferDecoder::LittleEndianBufferDecoder()" );
      }
      
      virtual Decoder &skip( size_t count )
      {
        FABRIC_UTIL_TRACE( "Util::LittleEndianBufferDecoder::skip( %u )", (unsigned)count );
        uint64_t newOffset = m_offset + count;
        if ( newOffset > uint64_t( m_buffer.size() ) )
          throw Exception( "data underrun" );
        m_offset = newOffset;
        return *this;
      }
    
    protected:
    
      Decoder &_get( void *data, size_t count, bool endian )
      {
        FABRIC_UTIL_TRACE( "Util::LittleEndianBufferDecoder::_get( %p, %u, %s )", data, (unsigned)count, endian?"true":"false" );
        uint64_t newOffset = m_offset + count;
        if ( newOffset > uint64_t( m_buffer.size() ) )
          throw Exception( "data underrun" );
        if ( !endian || isLittleEndian() )
          memcpy( data, &m_buffer[size_t(m_offset)], count );
        else
          reverseMemcpy( data, &m_buffer[size_t(m_offset)], count );
        m_offset = newOffset;
        FABRIC_UTIL_TRACE_DUMP_BUFFER( count, data );
        return *this;
      }  
    
    private:
    
      std::vector< uint8_t > const &m_buffer;
      uint64_t m_offset;
    };
  };
};

#endif //_FABRIC_UTIL_LITTLE_ENDIAN_BUFFER_DECODER_H
