/*
===============================================================================

  FILE:  bytestreamout_file.hpp
  
  CONTENTS:
      
  PROGRAMMERS:
  
    martin.isenburg@gmail.com
  
  COPYRIGHT:

    (c) 2010-2011, Martin Isenburg, LASSO - tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    10 January 2011 -- licensing change for LGPL release and liblas integration
    12 December 2010 -- created from ByteStreamOutFile after Howard got pushy (-;
  
===============================================================================
*/
#ifndef BYTE_STREAM_OUT_NIL_H
#define BYTE_STREAM_OUT_NIL_H

#include "bytestreamout.hpp"

#include <stdio.h>

class ByteStreamOutNil : public ByteStreamOut
{
public:
  ByteStreamOutNil();
/* write a single byte                                       */
  bool putByte(unsigned char byte);
/* write an array of bytes                                   */
  bool putBytes(const unsigned char* bytes, unsigned int num_bytes);
/* write 16 bit low-endian field                             */
  bool put16bitsLE(const unsigned char* bytes);
/* write 32 bit low-endian field                             */
  bool put32bitsLE(const unsigned char* bytes);
/* write 64 bit low-endian field                             */
  bool put64bitsLE(const unsigned char* bytes);
/* write 16 bit big-endian field                             */
  bool put16bitsBE(const unsigned char* bytes);
/* write 32 bit big-endian field                             */
  bool put32bitsBE(const unsigned char* bytes);
/* write 64 bit big-endian field                             */
  bool put64bitsBE(const unsigned char* bytes);
/* is the stream seekable (e.g. standard out is not)         */
  bool isSeekable() const;
/* get current position of stream                            */
  long position() const;
/* seek to this position in the stream                       */
  bool seek(const long position);
/* seek to the end of the file                               */
  bool seekEnd();
/* returns how many bytes were written                       */
  unsigned int byteCount() const;
/* returns how many bytes were written                       */
  void resetCount();
/* destructor                                                */
  ~ByteStreamOutNil(){};
private:
  U32 num_bytes;
};

inline ByteStreamOutNil::ByteStreamOutNil()
{
  num_bytes = 0;
}

inline bool ByteStreamOutNil::putByte(unsigned char byte)
{
  num_bytes++;
  return true;
}

inline bool ByteStreamOutNil::putBytes(const unsigned char* bytes, unsigned int num_bytes)
{
  this->num_bytes += num_bytes;
  return true;
}

inline bool ByteStreamOutNil::put16bitsLE(const unsigned char* bytes)
{
  return putBytes(bytes, 2);
}

inline bool ByteStreamOutNil::put32bitsLE(const unsigned char* bytes)
{
  return putBytes(bytes, 4);
}

inline bool ByteStreamOutNil::put64bitsLE(const unsigned char* bytes)
{
  return putBytes(bytes, 8);
}

inline bool ByteStreamOutNil::put16bitsBE(const unsigned char* bytes)
{
  return putBytes(bytes, 2);
}

inline bool ByteStreamOutNil::put32bitsBE(const unsigned char* bytes)
{
  return putBytes(bytes, 4);
}

inline bool ByteStreamOutNil::put64bitsBE(const unsigned char* bytes)
{
  return putBytes(bytes, 8);
}

inline bool ByteStreamOutNil::isSeekable() const
{
  return true;
}

inline long ByteStreamOutNil::position() const
{
  return 0;
}

inline bool ByteStreamOutNil::seek(long position)
{
  return true;
}

inline bool ByteStreamOutNil::seekEnd()
{
  return true;
}

inline unsigned int ByteStreamOutNil::byteCount() const
{
  return num_bytes;
}

inline void ByteStreamOutNil::resetCount()
{
  num_bytes = 0;
}

#endif
