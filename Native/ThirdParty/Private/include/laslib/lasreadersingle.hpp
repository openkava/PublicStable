/*
===============================================================================

  FILE:  lasreadersingle.hpp
  
  CONTENTS:
  
    Reads LIDAR points from the LAS format (Version 1.x , April 29, 2008).

  PROGRAMMERS:

    martin.isenburg@gmail.com

  COPYRIGHT:

    (c) 2007-2011, Martin Isenburg, LASSO - tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    21 January 2011 -- adapted from lasreader to create abstract reader class
    3 December 2010 -- updated to (somewhat) support LAS format 1.3
    7 September 2008 -- updated to support LAS format 1.2 
    18 February 2007 -- created after repairing 2 vacuum cleaners in the garden
  
===============================================================================
*/
#ifndef LAS_READER_SINGLE_HPP
#define LAS_READER_SINGLE_HPP

#include "lasreader.hpp"

#include <stdio.h>

#ifdef LZ_WIN32_VC6
#include <fstream.h>
#else
#include <istream>
#include <fstream>
using namespace std;
#endif

class ByteStreamIn;
class LASreadPoint;

class LASreaderSingle : public LASreader
{
public:

  BOOL open(const char* file_name);
  BOOL open(FILE* file);
  BOOL open(istream& stream);

  BOOL seek(const I64 p_index);

  void close();

  LASreaderSingle();
  virtual ~LASreaderSingle();

protected:
  virtual BOOL open(ByteStreamIn* stream);
  virtual BOOL read_point_default();

private:
  FILE* file;
  ByteStreamIn* stream;
  LASreadPoint* reader;
};

class LASreaderSingleRescale : public virtual LASreaderSingle
{
public:
  LASreaderSingleRescale(F64 x_scale_factor, F64 y_scale_factor, F64 z_scale_factor);

protected:
  virtual BOOL open(ByteStreamIn* stream);
  virtual BOOL read_point_default();
  BOOL rescale_x, rescale_y, rescale_z;
  F64 scale_factor[3];
  F64 orig_x_scale_factor, orig_y_scale_factor, orig_z_scale_factor;
};

class LASreaderSingleReoffset : public virtual LASreaderSingle
{
public:
  LASreaderSingleReoffset(F64 x_offset, F64 y_offset, F64 z_offset);

protected:
  virtual BOOL open(ByteStreamIn* stream);
  virtual BOOL read_point_default();
  BOOL reoffset_x, reoffset_y, reoffset_z;
  F64 offset[3];
  F64 orig_x_offset, orig_y_offset, orig_z_offset;
};

class LASreaderSingleRescaleReoffset : public LASreaderSingleRescale, LASreaderSingleReoffset
{
public:
  LASreaderSingleRescaleReoffset(F64 x_scale_factor, F64 y_scale_factor, F64 z_scale_factor, F64 x_offset, F64 y_offset, F64 z_offset);

protected:
  BOOL open(ByteStreamIn* stream);
  BOOL read_point_default();
};

#endif
