/*
===============================================================================

  FILE:  LASdefinitions.hpp
  
  CONTENTS:
  
    Contains the Header and Point classes for reading and writing LIDAR points
    in the LAS format

      Version 1.3,   Oct 24, 2010.
      Version 1.2, April 29, 2008.
      Version 1.1, March 07, 2005.
      Version 1.0,   May 09, 2003

  PROGRAMMERS:
  
    martin.isenburg@gmail.com
  
  COPYRIGHT:

    (c) 2005-2011, Martin Isenburg, LASSO - tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    10 January 2011 -- licensing change for LGPL release and liblas integration
    16 December 2010 -- updated to support generic LASitem point formats
    3 December 2010 -- updated to (somewhat) support LAS format 1.3
    7 September 2008 -- updated to support LAS format 1.2 
    11 June 2007 -- number of return / scan direction bitfield order was wrong
    18 February 2007 -- created after repairing 2 vacuum cleaners in the garden
  
===============================================================================
*/
#ifndef LAS_DEFINITIONS_HPP
#define LAS_DEFINITIONS_HPP

#define LAS_TOOLS_VERSION 110815

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "mydefs.hpp"
#include "laszip.hpp"

class LASwavepacket
{
public:
  LASwavepacket() {zero();};
  void zero() {memset(data, 0, 29);};
  inline U8 getIndex() const {return data[0];};
  inline U64 getOffset() const {return ((U64*)&(data[1]))[0];};
  inline U32 getSize() const {return ((U32*)&(data[9]))[0];};
  inline F32 getLocation() const {return ((F32*)&(data[13]))[0];};
  inline F32 getXt() const {return ((F32*)&(data[17]))[0];};
  inline F32 getYt() const {return ((F32*)&(data[21]))[0];};
  inline F32 getZt() const {return ((F32*)&(data[25]))[0];};
  inline void setIndex(U8 index) {data[0] = index;};
  inline void setOffset(U64 offset) {((U64*)&(data[1]))[0] = offset;};
  inline void setSize(U32 size) {((U32*)&(data[9]))[0] = size;};
  inline void setLocation(F32 location) { ((F32*)&(data[13]))[0] = location;};
  inline void setXt(F32 xt) {((F32*)&(data[17]))[0] = xt;};
  inline void setYt(F32 yt) {((F32*)&(data[21]))[0] = yt;};
  inline void setZt(F32 zt) {((F32*)&(data[25]))[0] = zt;};
private:
  U8 data[29];
};

class LASquantizer
{
public:
  F64 x_scale_factor;
  F64 y_scale_factor;
  F64 z_scale_factor;
  F64 x_offset;
  F64 y_offset;
  F64 z_offset;

  inline F64 get_x(const I32 x) const { return x_scale_factor*x+x_offset; };
  inline F64 get_y(const I32 y) const { return y_scale_factor*y+y_offset; };
  inline F64 get_z(const I32 z) const { return z_scale_factor*z+z_offset; };

  inline I32 get_x(const F64 x) const { if (x >= x_offset) return (I32)((x-x_offset)/x_scale_factor+0.5); else return (I32)((x-x_offset)/x_scale_factor-0.5); };
  inline I32 get_y(const F64 y) const { if (y >= y_offset) return (I32)((y-y_offset)/y_scale_factor+0.5); else return (I32)((y-y_offset)/y_scale_factor-0.5); };
  inline I32 get_z(const F64 z) const { if (z >= z_offset) return (I32)((z-z_offset)/z_scale_factor+0.5); else return (I32)((z-z_offset)/z_scale_factor-0.5); };

  LASquantizer()
  {
    x_scale_factor = 0.01;
    y_scale_factor = 0.01;
    z_scale_factor = 0.01;
    x_offset = 0.0;
    y_offset = 0.0;
    z_offset = 0.0;
  };

  LASquantizer & operator=(const LASquantizer & quantizer)
  {
    this->x_scale_factor = quantizer.x_scale_factor;
    this->y_scale_factor = quantizer.y_scale_factor;
    this->z_scale_factor = quantizer.z_scale_factor;
    this->x_offset = quantizer.x_offset;
    this->y_offset = quantizer.y_offset;
    this->z_offset = quantizer.z_offset;
    return *this;
  };
};

class LASpoint
{
public:

// these fields contain the data that describe each point

  I32 x;
  I32 y;
  I32 z;
  U16 intensity;
  U8 return_number : 3;
  U8 number_of_returns_of_given_pulse : 3;
  U8 scan_direction_flag : 1;
  U8 edge_of_flight_line : 1;
  U8 classification;
  I8 scan_angle_rank;
  U8 user_data;
  U16 point_source_ID;

  F64 gps_time;
  U16 rgb[3];
  LASwavepacket wavepacket;
  U8* extra_bytes;

// for converting between x,y,z integers and scaled/translated coordinates

  const LASquantizer* quantizer;
  F64 coordinates[3];

// this field provides generic access to the point data

  U8** point;

// these fields describe the point format LAS specific

  BOOL have_gps_time;
  BOOL have_rgb;
  BOOL have_wavepacket;
  I32 extra_bytes_number;
  U32 total_point_size;

// these fields describe the point format terms of generic items

  U16 num_items;
  LASitem* items;

// copy functions

  LASpoint(const LASpoint & other)
  {
    *this = other;
  }

  LASpoint & operator=(const LASpoint & other)
  {
    x = other.x;
    y = other.y;
    z = other.z;
    intensity = other.intensity;
    return_number = other.return_number;
    number_of_returns_of_given_pulse = other.number_of_returns_of_given_pulse;
    scan_direction_flag = other.scan_direction_flag;
    edge_of_flight_line = other.edge_of_flight_line;
    classification = other.classification;
    scan_angle_rank = other.scan_angle_rank;
    user_data = other.user_data;
    point_source_ID = other.point_source_ID;

    if (other.have_gps_time)
    {
      gps_time = other.gps_time;
    }
    if (other.have_rgb)
    {
      rgb[0] = other.rgb[0];
      rgb[1] = other.rgb[1];
      rgb[2] = other.rgb[2];
    }
    if (other.have_wavepacket)
    {
      wavepacket = other.wavepacket;
    }
    if (other.extra_bytes && extra_bytes)
    {
      memcpy(extra_bytes, other.extra_bytes, extra_bytes_number);
    }
    return *this;
  };

  void copy_to(U8* buffer) const
  {
    U32 i;
    U32 b = 0;
    for (i = 0; i < num_items; i++)
    {
      memcpy(&buffer[b], point[i], items[i].size);
      b += items[i].size;
    }
  };

  void copy_from(const U8* buffer)
  {
    U32 i;
    U32 b = 0;
    for (i = 0; i < num_items; i++)
    {
      memcpy(point[i], &buffer[b], items[i].size);
      b += items[i].size;
    }
  };

// these functions set the desired point format

  BOOL init(const LASquantizer* quantizer, const U8 point_type, const U16 point_size)
  {
    // clean the point

    clean();

    // switch over the point types we know

    if (!LASzip().setup(&num_items, &items, point_type, point_size))
    {
      fprintf(stderr,"ERROR: unknown point type %d with point size %d\n", (I32)point_type, (I32)point_size);
      return FALSE;
    }

    // create point's item pointers

    point = new U8*[num_items];

    U16 i;
    for (i = 0; i < num_items; i++)
    {
      total_point_size += items[i].size;
      switch (items[i].type)
      {
      case LASitem::POINT10:
        this->point[i] = (U8*)&(this->x);
        break;
      case LASitem::GPSTIME11:
        have_gps_time = true;
        this->point[i] = (U8*)&(this->gps_time);
        break;
      case LASitem::RGB12:
        have_rgb = true;
        this->point[i] = (U8*)(this->rgb);
        break;
      case LASitem::WAVEPACKET13:
        have_wavepacket = true;
        this->point[i] = (U8*)&(this->wavepacket);
        break;
      case LASitem::BYTE:
        extra_bytes_number = items[i].size;
        extra_bytes = new U8[extra_bytes_number];
        this->point[i] = extra_bytes;
        break;
      default:
        return FALSE;
      }
    }
    this->quantizer = quantizer;
    return TRUE;
  };

  BOOL init(const LASquantizer* quantizer, const U32 num_items, const LASitem* items)
  {
    U32 i,e;

    // clean the point

    clean();

    // create item description

    this->num_items = num_items;
    if (this->items) delete [] this->items;
    this->items = new LASitem[num_items];
    if (this->point) delete [] this->point;
    this->point = new U8*[num_items];

    for (i = 0, e = 0; i < num_items; i++)
    {
      this->items[i] = items[i];
      total_point_size += items[i].size;

      switch (items[i].type)
      {
      case LASitem::POINT10:
        this->point[i] = (U8*)&(this->x);
        break;
      case LASitem::GPSTIME11:
        have_gps_time = true;
        this->point[i] = (U8*)&(this->gps_time);
        break;
      case LASitem::RGB12:
        have_rgb = true;
        this->point[i] = (U8*)(this->rgb);
        break;
      case LASitem::WAVEPACKET13:
        have_wavepacket = true;
        this->point[i] = (U8*)&(this->wavepacket);
        break;
      case LASitem::BYTE:
        extra_bytes_number = items[i].size;
        extra_bytes = new U8[extra_bytes_number];
        this->point[i] = extra_bytes;
        break;
      default:
        return FALSE;
      }
    }
    this->quantizer = quantizer;
    return TRUE;
  };

  BOOL inside_rectangle(const F64 r_min_x, const F64 r_min_y, const F64 r_max_x, const F64 r_max_y) const
  {
    F64 xy;
    xy = get_x();
    if (xy < r_min_x || xy > r_max_x) return FALSE;
    xy = get_y();
    if (xy < r_min_y || xy > r_max_y) return FALSE;
    return TRUE;
  }

  BOOL inside_tile(const F32 ll_x, const F32 ll_y, const F32 ur_x, const F32 ur_y) const
  {
    F64 xy;
    xy = get_x();
    if (xy < ll_x || xy >= ur_x) return FALSE;
    xy = get_y();
    if (xy < ll_y || xy >= ur_y) return FALSE;
    return TRUE;
  }

  BOOL inside_circle(const F64 center_x, const F64 center_y, F64 squared_radius) const
  {
    F64 dx = center_x - get_x();
    F64 dy = center_y - get_y();
    return ((dx*dx+dy*dy) < squared_radius);
  }

  BOOL is_zero() const
  {
    if (((U32*)&(this->x))[0] || ((U32*)&(this->x))[1] || ((U32*)&(this->x))[2] || ((U32*)&(this->x))[3] || ((U32*)&(this->x))[4])
    {
      return FALSE;
    }
    if (have_gps_time)
    {
      if (this->gps_time)
      {
        return FALSE;
      }
    }
    if (have_rgb)
    {
      if (this->rgb[0] || this->rgb[1] || this->rgb[2])
      {
        return FALSE;
      }
    }
    return TRUE;
  }

  void zero()
  {
    x=0;
    y=0;
    z=0;
    intensity=0;
    edge_of_flight_line=0;
    scan_direction_flag=0;
    number_of_returns_of_given_pulse = 1;
    return_number = 1;
    classification = 0;
    scan_angle_rank = 0;
    user_data = 0;
    point_source_ID = 0;

    gps_time = 0.0;
    rgb[0] = rgb[1] = rgb[2] = 0;
    wavepacket.zero();
  };

  void clean()
  {
    zero();

    if (extra_bytes)
    {
      delete [] extra_bytes;
      extra_bytes = 0;
    };

    if (point) delete [] point;
    point = 0;

    have_gps_time = FALSE;
    have_rgb = FALSE;
    have_wavepacket = FALSE;
    extra_bytes_number = 0;
    total_point_size = 0;
    
    num_items = 0;
    if (items) delete [] items;
    items = 0;
  };

  LASpoint()
  {
    extra_bytes = 0;
    point = 0;
    items = 0;
    clean();
  };

  inline F64 get_x() const { return quantizer->get_x(x); };
  inline F64 get_y() const { return quantizer->get_y(y); };
  inline F64 get_z() const { return quantizer->get_z(z); };

  inline void set_x(const F64 x) { this->x = quantizer->get_x(x); };
  inline void set_y(const F64 y) { this->y = quantizer->get_y(y); };
  inline void set_z(const F64 z) { this->z = quantizer->get_z(z); };

  inline void compute_coordinates()
  {
    coordinates[0] = get_x();
    coordinates[1] = get_y();
    coordinates[2] = get_z();
  };

  inline void compute_xyz()
  {
    set_x(coordinates[0]);
    set_y(coordinates[1]);
    set_z(coordinates[2]);
  };

  inline void compute_xyz(const LASquantizer* quantizer)
  {
    x = quantizer->get_x(coordinates[0]);
    y = quantizer->get_y(coordinates[1]);
    z = quantizer->get_z(coordinates[2]);
  };

  ~LASpoint()
  {
    clean();
  };
};

class LASvlr
{
public:
  U16 reserved;
  I8 user_id[16]; 
  U16 record_id;
  U16 record_length_after_header;
  I8 description[32];
  I8* data;
};

class LASvlr_geo_keys
{
public:
  U16 key_directory_version;
  U16 key_revision;
  U16 minor_revision;
  U16 number_of_keys;
};

class LASvlr_key_entry
{
public:
  U16 key_id;
  U16 tiff_tag_location;
  U16 count;
  U16 value_offset;
};

class LASvlr_classification
{
public:
  U8 class_number;
  I8 description[15];
};

class LASvlr_wave_packet_descr
{
public:
  LASvlr_wave_packet_descr() {clean();};
  void clean() {memset(data, 0, 29);};
  inline U8 getBitsPerSample() const {return data[0];};
  inline U8 getCompressionType() const {return data[1];};
  inline U32 getNumberOfSamples() const {return ((U32*)&(data[2]))[0];};
  inline U32 getTemporalSpacing() const {return ((U32*)&(data[6]))[0];};
  inline F64 getDigitizerGain() const {return ((F64*)&(data[10]))[0];};
  inline F64 getDigitizerOffset() const {return ((F64*)&(data[18]))[0];};
  inline void setBitsPerSample(U8 bps) {data[0] = bps;};
  inline void setCompressionType(U8 compression) {data[1] = compression;};
  inline void setNumberOfSamples(U32 samples) {((U32*)&(data[2]))[0] = samples;};
  inline void setTemporalSpacing(U32 spacing) {((U32*)&(data[6]))[0] = spacing;};
  inline void setDigitizerGain(F64 gain) {((F64*)&(data[10]))[0] = gain;};
  inline void setDigitizerOffset(F64 offset) {((F64*)&(data[18]))[0] = offset;};
private:
  U8 data[26];
};

class LASvlr_lastiling
{
public:
  U32 level;
  U32 level_index;
  U32 implicit_levels;
  F32 min_x;
  F32 max_x;
  F32 min_y;
  F32 max_y;
};

class LASheader : public LASquantizer
{
public:
  I8 file_signature[4];
  U16 file_source_id;
  U16 global_encoding;
  U32 project_ID_GUID_data_1;
  U16 project_ID_GUID_data_2;
  U16 project_ID_GUID_data_3;
  U8 project_ID_GUID_data_4[8];
  U8 version_major;
  U8 version_minor;
  I8 system_identifier[32];
  I8 generating_software[32];
  U16 file_creation_day;
  U16 file_creation_year;
  U16 header_size;
  U32 offset_to_point_data;
  U32 number_of_variable_length_records;
  U8 point_data_format;
  U16 point_data_record_length;
  U32 number_of_point_records;
  U32 number_of_points_by_return[5];
  F64 max_x;
  F64 min_x;
  F64 max_y;
  F64 min_y;
  F64 max_z;
  F64 min_z;

  U64 start_of_waveform_data_packet_record; // LAS 1.3 only

  U32 user_data_in_header_size;
  I8* user_data_in_header;

  LASvlr* vlrs;
  LASvlr_geo_keys* vlr_geo_keys;
  LASvlr_key_entry* vlr_geo_key_entries;
  F64* vlr_geo_double_params;
  I8* vlr_geo_ascii_params;
  LASvlr_classification* vlr_classification;
  LASvlr_wave_packet_descr** vlr_wave_packet_descr;

  LASzip* laszip;
  LASvlr_lastiling* vlr_lastiling;

  U32 user_data_after_header_size;
  I8* user_data_after_header;

  LASheader()
  {
    clean_las_header();
  };

  // clean functions

  void clean_las_header()
  {
    memset((void*)this, 0, sizeof(LASheader));
    file_signature[0] = 'L'; file_signature[1] = 'A'; file_signature[2] = 'S'; file_signature[3] = 'F';
    version_major = 1;
    version_minor = 2;
    header_size = 227;
    offset_to_point_data = 227;
    point_data_record_length = 20;
    x_scale_factor = 0.01;
    y_scale_factor = 0.01;
    z_scale_factor = 0.01;
  };

  void clean_user_data_in_header()
  {
    if (user_data_in_header)
    {
      header_size -= user_data_in_header_size;
      delete [] user_data_in_header;
      user_data_in_header = 0;
      user_data_in_header_size = 0;
    }
  };

  void clean_vlrs()
  {
    if (vlrs)
    {
      U32 i;
      for (i = 0; i < number_of_variable_length_records; i++)
      {
        offset_to_point_data -= (54 + vlrs[i].record_length_after_header);
        delete [] vlrs[i].data;
      }
      free(vlrs);
      vlrs = 0;
      vlr_geo_keys = 0;
      vlr_geo_key_entries = 0;
      vlr_geo_double_params = 0;
      vlr_geo_ascii_params = 0;
      vlr_classification = 0;
      if (vlr_wave_packet_descr) delete [] vlr_wave_packet_descr;
      vlr_wave_packet_descr = 0;
      number_of_variable_length_records = 0;
    }
  };

  void clean_vlrs(U32 i)
  {
    if (vlrs)
    {
      if (i < number_of_variable_length_records)
      {
        offset_to_point_data -= (54 + vlrs[i].record_length_after_header);
        delete [] vlrs[i].data;
        number_of_variable_length_records--;
        if (number_of_variable_length_records)
        {
          vlrs[i] = vlrs[number_of_variable_length_records];
        }
      }
    }
  };

  void clean_laszip()
  {
    if (laszip)
    {
      delete laszip;
    }
    laszip = 0;
  };

  void clean_lastiling()
  {
    if (vlr_lastiling)
    {
      delete vlr_lastiling;
    }
    vlr_lastiling = 0;
  };

  void clean_user_data_after_header()
  {
    if (user_data_after_header)
    {
      offset_to_point_data -= user_data_after_header_size;
      delete [] user_data_after_header;
      user_data_after_header = 0;
      user_data_after_header_size = 0;
    }
  };

  void clean()
  {
    clean_user_data_in_header();
    clean_vlrs();
    clean_laszip();
    clean_lastiling();
    clean_user_data_after_header();
    clean_las_header();
  };

  LASheader & operator=(const LASquantizer & quantizer)
  {
    this->x_scale_factor = quantizer.x_scale_factor;
    this->y_scale_factor = quantizer.y_scale_factor;
    this->z_scale_factor = quantizer.z_scale_factor;
    this->x_offset = quantizer.x_offset;
    this->y_offset = quantizer.y_offset;
    this->z_offset = quantizer.z_offset;
    return *this;
  };

  BOOL check() const
  {
    if (strncmp(file_signature, "LASF", 4) != 0)
    {
      fprintf(stderr,"ERROR: wrong file signature '%s'\n", file_signature);
      return FALSE;
    }
    if ((version_major != 1) || ((version_minor != 0) && (version_minor != 1) && (version_minor != 2) && (version_minor != 3)))
    {
      fprintf(stderr,"WARNING: unknown version %d.%d (should be 1.0 or 1.1 or 1.2 or 1.3)\n", version_major, version_minor);
    }
    if (header_size < 227)
    {
      fprintf(stderr,"ERROR: header size is %d but should be at least 227\n", header_size);
      return FALSE;
    }
    if (offset_to_point_data < header_size)
    {
      fprintf(stderr,"ERROR: offset to point data %d is smaller than header size %d\n", offset_to_point_data, header_size);
      return FALSE;
    }
    if (x_scale_factor == 0)
    {
      fprintf(stderr,"ERROR: x scale factor is zero.\n");
      return FALSE;
    }
    if (y_scale_factor == 0)
    {
      fprintf(stderr,"ERROR: y scale factor is zero.\n");
      return FALSE;
    }
    if (z_scale_factor == 0)
    {
      fprintf(stderr,"ERROR: z scale factor is zero.\n");
      return FALSE;
    }
    if (max_x < min_x || max_y < min_y || max_z < min_z)
    {
      fprintf(stderr,"WARNING: invalid bounding box [ %g %g %g / %g %g %g ]\n", min_x, min_y, min_z, max_x, max_y, max_z);
    }
    return TRUE;
  };

  void set_lastiling(U32 level, U32 level_index, U32 implicit_levels, F32 min_x, F32 max_x, F32 min_y, F32 max_y)
  {
    clean_lastiling();
    vlr_lastiling = new LASvlr_lastiling();
    vlr_lastiling->level = level;
    vlr_lastiling->level_index = level_index;
    vlr_lastiling->implicit_levels = implicit_levels;
    vlr_lastiling->min_x = min_x;
    vlr_lastiling->max_x = max_x;
    vlr_lastiling->min_y = min_y;
    vlr_lastiling->max_y = max_y;
  };

  void set_geo_keys(const I32 number_of_keys, const LASvlr_key_entry* geo_keys)
  {
    U32 i = 0;
    vlr_geo_keys = new LASvlr_geo_keys[number_of_keys+1];
    vlr_geo_keys->key_directory_version = 1;
    vlr_geo_keys->key_revision = 1;
    vlr_geo_keys->minor_revision = 0;
    vlr_geo_keys->number_of_keys = number_of_keys;
    vlr_geo_key_entries = (LASvlr_key_entry*)&vlr_geo_keys[1];
    memcpy(vlr_geo_key_entries, geo_keys, sizeof(LASvlr_key_entry)*number_of_keys);
    if (vlrs)
    {
      for (i = 0; i < number_of_variable_length_records; i++)
      {
        if ((strcmp(vlrs[i].user_id, "LASF_Projection") == 0) && (vlrs[i].record_id == 34735)) // GeoKeyDirectoryTag
        {
          offset_to_point_data -= vlrs[i].record_length_after_header;
          delete [] vlrs[i].data;
          break;
        }
      }
      if (i == number_of_variable_length_records)
      {
        number_of_variable_length_records++;
        offset_to_point_data += 54;
        vlrs = (LASvlr*)realloc(vlrs, sizeof(LASvlr)*number_of_variable_length_records);
      }
    }
    else
    {
      number_of_variable_length_records = 1;
      offset_to_point_data += 54;
      vlrs = (LASvlr*)malloc(sizeof(LASvlr)*number_of_variable_length_records);
    }
    vlrs[i].reserved = 0xAABB;
    sprintf(vlrs[i].user_id, "LASF_Projection");
    vlrs[i].record_id = 34735;
    vlrs[i].record_length_after_header = sizeof(LASvlr_geo_keys)*(number_of_keys+1);
    offset_to_point_data += vlrs[i].record_length_after_header;
    sprintf(vlrs[i].description, "by LAStools of Martin Isenburg");
    vlrs[i].data = (char*)vlr_geo_keys;
  }

  void set_geo_double_params(const I32 num_geo_double_params, const F64* geo_double_params)
  {
    U32 i = 0;
    vlr_geo_double_params = new F64[num_geo_double_params];
    memcpy(vlr_geo_double_params, geo_double_params, sizeof(F64)*num_geo_double_params);
    if (vlrs)
    {
      for (i = 0; i < number_of_variable_length_records; i++)
      {
        if ((strcmp(vlrs[i].user_id, "LASF_Projection") == 0) && (vlrs[i].record_id == 34736)) // GeoDoubleParamsTag
        {
          offset_to_point_data -= vlrs[i].record_length_after_header;
          delete [] vlrs[i].data;
          break;
        }
      }
      if (i == number_of_variable_length_records)
      {
        number_of_variable_length_records++;
        offset_to_point_data += 54;
        vlrs = (LASvlr*)realloc(vlrs, sizeof(LASvlr)*number_of_variable_length_records);
      }
    }
    else
    {
      number_of_variable_length_records = 1;
      offset_to_point_data += 54;
      vlrs = (LASvlr*)malloc(sizeof(LASvlr)*number_of_variable_length_records);
    }
    vlrs[i].reserved = 0xAABB;
    sprintf(vlrs[i].user_id, "LASF_Projection");
    vlrs[i].record_id = 34736;
    vlrs[i].record_length_after_header = sizeof(F64)*num_geo_double_params;
    offset_to_point_data += vlrs[i].record_length_after_header;
    sprintf(vlrs[i].description, "by LAStools of Martin Isenburg");
    vlrs[i].data = (char*)vlr_geo_double_params;
  }

  void del_geo_ascii_params()
  {
    if (vlr_geo_ascii_params)
    {
      U32 i;
      for (i = 0; i < number_of_variable_length_records; i++)
      {
        if ((strcmp(vlrs[i].user_id, "LASF_Projection") == 0) && (vlrs[i].record_id == 34737)) // GeoAsciiParamsTag
        {
          if (vlrs[i].record_length_after_header)
          {
            offset_to_point_data -= vlrs[i].record_length_after_header;
            delete [] vlrs[i].data;
          }
          break;
        }
      }
      if (i != number_of_variable_length_records)
      {
        number_of_variable_length_records--;
        offset_to_point_data -= 54;
        vlrs[i] = vlrs[number_of_variable_length_records];
      }
      vlr_geo_ascii_params = 0;
    }
  }

  ~LASheader()
  {
    clean();
  };
};

#endif
