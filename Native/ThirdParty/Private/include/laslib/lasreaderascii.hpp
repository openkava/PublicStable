/*
===============================================================================

  FILE:  lasreaderascii.hpp
  
  CONTENTS:
  
    Reads LIDAR points in LAS format through on-the-fly conversion from ASCII.

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
  
    8 April 2011 -- created after starting a google group for LAStools users
  
===============================================================================
*/
#ifndef LAS_READER_ASCII_HPP
#define LAS_READER_ASCII_HPP

#include "lasreader.hpp"

#include <stdio.h>

class LASreaderASCII : public LASreader
{
public:

  void set_translate_intensity(F32 translate_intensity);
  void set_scale_intensity(F32 scale_intensity);
  void set_translate_scan_angle(F32 translate_scan_angle);
  void set_scale_scan_angle(F32 scale_scan_angle);
  void set_scale_factor(const F64* scale_factor);
  void set_offset(const F64* offset);
  virtual BOOL open(const char* file_name, const char* parse_string=0, I32 skip_lines=0, BOOL populate_header=FALSE);

  BOOL seek(const I64 p_index);

  void close();
  BOOL reopen(const char* file_name);

  LASreaderASCII();
  virtual ~LASreaderASCII();

protected:
  BOOL read_point_default();

private:
  char* parse_string;
  F32 translate_intensity;
  F32 scale_intensity;
  F32 translate_scan_angle;
  F32 scale_scan_angle;
  F64* scale_factor;
  F64* offset;
  I32 skip_lines;
  BOOL populated_header;
  FILE* file;
  bool piped;
  char line[512];
  BOOL parse(const char* parse_string);
  BOOL check_parse_string(const char* parse_string);
  void populate_scale_and_offset();
  void populate_bounding_box();
  void clean();
};

class LASreaderASCIIRescale : public virtual LASreaderASCII
{
public:
  virtual BOOL open(const char* file_name, const char* parse_string=0, I32 skip_lines=0, BOOL populate_header=FALSE);
  LASreaderASCIIRescale(F64 x_scale_factor, F64 y_scale_factor, F64 z_scale_factor);

protected:
  F64 scale_factor[3];
};

class LASreaderASCIIReoffset : public virtual LASreaderASCII
{
public:
  virtual BOOL open(const char* file_name, const char* parse_string=0, I32 skip_lines=0, BOOL populate_header=FALSE);
  LASreaderASCIIReoffset(F64 x_offset, F64 y_offset, F64 z_offset);
protected:
  F64 offset[3];
};

class LASreaderASCIIRescaleReoffset : public LASreaderASCIIRescale, LASreaderASCIIReoffset
{
public:
  BOOL open(const char* file_name, const char* parse_string=0, I32 skip_lines=0, BOOL populate_header=FALSE);
  LASreaderASCIIRescaleReoffset(F64 x_scale_factor, F64 y_scale_factor, F64 z_scale_factor, F64 x_offset, F64 y_offset, F64 z_offset);
};

#endif
