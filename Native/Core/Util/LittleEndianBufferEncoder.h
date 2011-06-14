/*
 *
 *  Created by Peter Zion on 10-10-28.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */
 
#ifndef _FABRIC_UTIL_LITTLE_ENDIAN_BUFFER_ENCODER_H
#define _FABRIC_UTIL_LITTLE_ENDIAN_BUFFER_ENCODER_H

#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Core/Util/Endian.h>
#include <stdint.h>
#include <vector>

#if defined(FABRIC_OS_LINUX)
	#include <string.h>
#endif

namespace Fabric
{
  namespace Util
  {
    class LittleEndianBufferEncoder : public Encoder
    {
    public:
    
      LittleEndianBufferEncoder( std::vector< uint8_t > &buffer )
        : m_buffer( buffer )
      {
        FABRIC_UTIL_TRACE( "Util::LittleEndianBufferEncoder::LittleEndianBufferEncoder()" );
      }
      
    protected:
    
      virtual Encoder &_put( void const *data, size_t count, bool endian )
      {
        FABRIC_UTIL_TRACE( "Util::LittleEndianBufferEncoder::_put( %p, %u, %s )", data, (unsigned)count, endian?"true":"false" );
        FABRIC_UTIL_TRACE_DUMP_BUFFER( count, data );
        size_t oldSize = m_buffer.size();
        m_buffer.resize( oldSize + count );
        if ( !endian || isLittleEndian() )
          memcpy( &m_buffer[oldSize], data, count );
        else
          reverseMemcpy( &m_buffer[oldSize], data, count );
        return *this;
      }

    private:
    
      std::vector< uint8_t > &m_buffer;
    };
  };
};

#endif //_FABRIC_UTIL_LITTLE_ENDIAN_BUFFER_ENCODER_H
