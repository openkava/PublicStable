/*
===============================================================================

  FILE:  lasreadermulti.cpp
  
  CONTENTS:
  
    see corresponding header file
  
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
  
    see corresponding header file
  
===============================================================================
*/
#include "lasreadermulti.hpp"

#include "lasindex.hpp"
#include "lasfilter.hpp"

#include <stdlib.h>
#include <string.h>

BOOL LASreaderMulti::add_file_name(const char* file_name)
{
  // do we have a file name
  if (file_name == 0)
  {
    fprintf(stderr, "ERROR: file name pointer is NULL\n");
    return FALSE;
  }
  // does the file exist
  FILE* file = fopen(file_name, "r");
  if (file == 0)
  {
    fprintf(stderr, "ERROR: file '%s' cannot be opened\n", file_name);
    return FALSE;
  }
  fclose(file);
  // check file extension
  if (strstr(file_name, ".las") || strstr(file_name, ".laz") || strstr(file_name, ".LAS") || strstr(file_name, ".LAZ"))
  {
    if (lasreaderascii)
    {
      fprintf(stderr, "ERROR: cannot mix LAS & ASCII. skipping '%s' ...\n", file_name);
      return FALSE;
    }
    if (lasreadersingle == 0)
    {
      lasreader = lasreadersingle = new LASreaderSingle();
    }
  }
  else
  {
    if (lasreadersingle)
    {
      fprintf(stderr, "ERROR: cannot mix LAS & ASCII. skipping '%s' ...\n", file_name);
      return FALSE;
    }
    if (lasreaderascii == 0)
    {
      lasreader = lasreaderascii = new LASreaderASCII();
    }
  }
  // add the file
  if (file_name_number == file_name_allocated)
  {
    file_name_allocated += 1024;
    if (file_names)
    {
      file_names = (char**)realloc(file_names, sizeof(char*)*file_name_allocated);
    }
    else
    {
      file_names = (char**)malloc(sizeof(char*)*file_name_allocated);
    }
    if (file_names == 0)
    {
      fprintf(stderr, "ERROR: alloc for file_names pointer array failed at %d\n", file_name_allocated);
      return FALSE;
    }
  }
  file_names[file_name_number] = strdup(file_name);
  file_name_number++;
  return TRUE;
}

void LASreaderMulti::set_scale_factor(const F64* scale_factor)
{
  if (scale_factor)
  {
    if (this->scale_factor == 0) this->scale_factor = new F64[3];
    this->scale_factor[0] = scale_factor[0];
    this->scale_factor[1] = scale_factor[1];
    this->scale_factor[2] = scale_factor[2];
  }
  else if (this->scale_factor)
  {
    delete [] this->scale_factor;
    this->scale_factor = 0;
  }
}

void LASreaderMulti::set_offset(const F64* offset)
{
  if (offset)
  {
    if (this->offset == 0) this->offset = new F64[3];
    this->offset[0] = offset[0];
    this->offset[1] = offset[1];
    this->offset[2] = offset[2];
  }
  else if (this->offset)
  {
    delete [] this->offset;
    this->offset = 0;
  }
}

void LASreaderMulti::set_translate_intensity(F32 translate_intensity)
{
  this->translate_intensity = translate_intensity;
}

void LASreaderMulti::set_scale_intensity(F32 scale_intensity)
{
  this->scale_intensity = scale_intensity;
}

void LASreaderMulti::set_translate_scan_angle(F32 translate_scan_angle)
{
  this->translate_scan_angle = translate_scan_angle;
}

void LASreaderMulti::set_scale_scan_angle(F32 scale_scan_angle)
{
  this->scale_scan_angle = scale_scan_angle;
}

void LASreaderMulti::set_parse_string(const char* parse_string)
{
  if (this->parse_string) free(this->parse_string);
  if (parse_string)
  {
    this->parse_string = strdup(parse_string);
  }
  else
  {
    this->parse_string = 0;
  }
}

void LASreaderMulti::set_skip_lines(I32 skip_lines)
{
  this->skip_lines = skip_lines;
}

void LASreaderMulti::set_populate_header(BOOL populate_header)
{
  this->populate_header = populate_header;
}

BOOL LASreaderMulti::open()
{
  if (file_name_number == 0)
  {
    fprintf(stderr, "ERROR: no valid file names\n");
    return FALSE;
  }

  // allocate space for the individual bounding_boxes
  if (bounding_boxes) delete [] bounding_boxes;
  bounding_boxes = new F64[file_name_number*4];

  // clean  header
  header.clean();

  // combine all headers

  U32 i;
  for (i = 0; i < file_name_number; i++)
  {
    // open the lasreader with the next file name
    if (lasreadersingle)
    {
      if (!lasreadersingle->open(file_names[i]))
      {
        fprintf(stderr, "ERROR: could not open lasreadersingle for file '%s'\n", file_names[i]);
        return FALSE;
      }
    }
    else
    {
      lasreaderascii->set_translate_intensity(translate_intensity);
      lasreaderascii->set_scale_intensity(scale_intensity);
      lasreaderascii->set_translate_scan_angle(translate_scan_angle);
      lasreaderascii->set_scale_scan_angle(scale_scan_angle);
      lasreaderascii->set_scale_factor(scale_factor);
      lasreaderascii->set_offset(offset);
      if (!lasreaderascii->open(file_names[i], parse_string, skip_lines, populate_header))
      {
        fprintf(stderr, "ERROR: could not open lasreaderascii for file '%s'\n", file_names[i]);
        return FALSE;
      }
    }
    // record individual bounding box info
    bounding_boxes[4*i+0] = lasreader->header.min_x;
    bounding_boxes[4*i+1] = lasreader->header.min_y;
    bounding_boxes[4*i+2] = lasreader->header.max_x;
    bounding_boxes[4*i+3] = lasreader->header.max_y;
    // populate the merged header
    if (i == 0)
    {
      // use the header info from the first file
      header = lasreader->header;
      // zero the pointers of the other header so they don't get deallocated twice
      lasreader->header.user_data_in_header = 0;
      lasreader->header.vlrs = 0;
      lasreader->header.laszip = 0;
      lasreader->header.vlr_lastiling = 0;
      lasreader->header.user_data_after_header = 0;
      // count the points up to 64 bits
      npoints = header.number_of_point_records;
    }
    else
    {
      // count the points up to 64 bits
      npoints += lasreader->header.number_of_point_records;
      // but increment point counters and widen the bounding box
      header.number_of_point_records += lasreader->header.number_of_point_records;
      header.number_of_points_by_return[0] += lasreader->header.number_of_points_by_return[0];
      header.number_of_points_by_return[1] += lasreader->header.number_of_points_by_return[1];
      header.number_of_points_by_return[2] += lasreader->header.number_of_points_by_return[2];
      header.number_of_points_by_return[3] += lasreader->header.number_of_points_by_return[3];
      header.number_of_points_by_return[4] += lasreader->header.number_of_points_by_return[4];
      if (header.max_x < lasreader->header.max_x) header.max_x = lasreader->header.max_x;
      if (header.max_y < lasreader->header.max_y) header.max_y = lasreader->header.max_y;
      if (header.max_z < lasreader->header.max_z) header.max_z = lasreader->header.max_z;
      if (header.min_x > lasreader->header.min_x) header.min_x = lasreader->header.min_x;
      if (header.min_y > lasreader->header.min_y) header.min_y = lasreader->header.min_y;
      if (header.min_z > lasreader->header.min_z) header.min_z = lasreader->header.min_z;
      // a point type change could be problematic
      if (header.point_data_format != lasreader->header.point_data_format)
      {
        if (!point_type_change) fprintf(stderr, "WARNING: files have different point types: %d vs %d\n", header.point_data_format, lasreader->header.point_data_format);
        point_type_change = TRUE;
      }
      // and check if we need to resample points because scalefactor of offsets change
      if (header.x_scale_factor != lasreader->header.x_scale_factor ||
          header.y_scale_factor != lasreader->header.y_scale_factor ||
          header.z_scale_factor != lasreader->header.z_scale_factor)
      {
//        if (!rescale) fprintf(stderr, "WARNING: files have different scale factors: %g %g %g vs %g %g %g\n", header.x_scale_factor, header.y_scale_factor, header.z_scale_factor, lasreader->header.x_scale_factor, lasreader->header.y_scale_factor, lasreader->header.z_scale_factor);
        rescale = TRUE;
      }
      if (header.x_offset != lasreader->header.x_offset ||
          header.y_offset != lasreader->header.y_offset ||
          header.z_offset != lasreader->header.z_offset)
      {
//        if (!reoffset) fprintf(stderr, "WARNING: files have different offsets: %g %g %g vs %g %g %g\n", header.x_offset, header.y_offset, header.z_offset, lasreader->header.x_offset, lasreader->header.y_offset, lasreader->header.z_offset);
        reoffset = TRUE;
      }
    }
    lasreader->close();
  }

  // was it requested to rescale or reoffset

  if (scale_factor)
  {
    if (header.x_scale_factor != scale_factor[0])
    {
      header.x_scale_factor = scale_factor[0];
      rescale = TRUE;
    }
    if (header.y_scale_factor != scale_factor[1])
    {
      header.y_scale_factor = scale_factor[1];
      rescale = TRUE;
    }
    if (header.z_scale_factor != scale_factor[2])
    {
      header.z_scale_factor = scale_factor[2];
      rescale = TRUE;
    }
  }
  if (offset)
  {
    if (header.x_offset != offset[0])
    {
      header.x_offset = offset[0];
      reoffset = TRUE;
    }
    if (header.y_offset != offset[1])
    {
      header.y_offset = offset[1];
      reoffset = TRUE;
    }
    if (header.z_offset != offset[2])
    {
      header.z_offset = offset[2];
      reoffset = TRUE;
    }
  }

  // initialize the point with the header info

  if (header.laszip)
  {
    if (!point.init(&header, header.laszip->num_items, header.laszip->items)) return FALSE;
  }
  else
  {
    if (!point.init(&header, header.point_data_format, header.point_data_record_length)) return FALSE;
  }

  // check if the header can support the enlarged bounding box

  // check x

  if ((((header.max_x - header.x_offset) / header.x_scale_factor) > I32_MAX) || (((header.min_x - header.x_offset) / header.x_scale_factor) < I32_MIN))
  {
    // maybe we can fix it by adjusting the offset (and if needed by lowering the resolution via the scale factor)
    F64 x_offset = (F64)I64_QUANTIZE((header.min_x + header.max_x)/2);
    F64 x_scale_factor = header.x_scale_factor;
    while ((((header.max_x - x_offset) / x_scale_factor) > I32_MAX) || (((header.min_x - x_offset) / x_scale_factor) < I32_MIN))
    {
      x_scale_factor *= 10;
    }
    if (header.x_scale_factor != x_scale_factor)
    {
      fprintf(stderr, "WARNING: i changed x_scale_factor from %g to %g to accommodate enlarged bounding box\n", header.x_scale_factor, x_scale_factor);
      header.x_scale_factor = x_scale_factor;
      rescale = TRUE;
    }
    // maybe we changed the resolution ... so do we really need to adjuste the offset
    if ((((header.max_x - header.x_offset) / x_scale_factor) > I32_MAX) || (((header.min_x - header.x_offset) / x_scale_factor) < I32_MIN))
    {
      fprintf(stderr, "WARNING: i changed x_offset from %g to %g to accommodate enlarged bounding box\n", header.x_offset, x_offset);
      header.x_offset = x_offset;
      reoffset = TRUE;
    }
  }
    
  // check y

  if ((((header.max_y - header.y_offset) / header.y_scale_factor) > I32_MAX) || (((header.min_y - header.y_offset) / header.y_scale_factor) < I32_MIN))
  {
    // maybe we can fix it by adjusting the offset (and if needed by lowering the resolution via the scale factor)
    F64 y_offset = (F64)I64_QUANTIZE((header.min_y + header.max_y)/2);
    F64 y_scale_factor = header.y_scale_factor;
    while ((((header.max_y - y_offset) / y_scale_factor) > I32_MAX) || (((header.min_y - y_offset) / y_scale_factor) < I32_MIN))
    {
      y_scale_factor *= 10;
    }
    if (header.y_scale_factor != y_scale_factor)
    {
      fprintf(stderr, "WARNING: i changed y_scale_factor from %g to %g to accommodate enlarged bounding box\n", header.y_scale_factor, y_scale_factor);
      header.y_scale_factor = y_scale_factor;
      rescale = TRUE;
    }
    // maybe we changed the resolution ... so do we really need to adjuste the offset
    if ((((header.max_y - header.y_offset) / y_scale_factor) > I32_MAX) || (((header.min_y - header.y_offset) / y_scale_factor) < I32_MIN))
    {
      fprintf(stderr, "WARNING: i changed y_offset from %g to %g to accommodate enlarged bounding box\n", header.y_offset, y_offset);
      header.y_offset = y_offset;
      reoffset = TRUE;
    }
  }
    
  // check z

  if ((((header.max_z - header.z_offset) / header.z_scale_factor) > I32_MAX) || (((header.min_z - header.z_offset) / header.z_scale_factor) < I32_MIN))
  {
    // maybe we can fix it by adjusting the offset (and if needed by lowering the resolution via the scale factor)
    F64 z_offset = (F64)I64_QUANTIZE((header.min_z + header.max_z)/2);
    F64 z_scale_factor = header.z_scale_factor;
    while ((((header.max_z - z_offset) / z_scale_factor) > I32_MAX) || (((header.min_z - z_offset) / z_scale_factor) < I32_MIN))
    {
      z_scale_factor *= 10;
    }
    if (header.z_scale_factor != z_scale_factor)
    {
      fprintf(stderr, "WARNING: i changed  z_scale_factor from %g to %g to accommodate enlarged bounding box\n", header.z_scale_factor, z_scale_factor);
      header.z_scale_factor = z_scale_factor;
      rescale = TRUE;
    }
    // maybe we changed the resolution ... so do we really need to adjuste the offset
    if ((((header.max_z - header.z_offset) / z_scale_factor) > I32_MAX) || (((header.min_z - header.z_offset) / z_scale_factor) < I32_MIN))
    {
      fprintf(stderr, "WARNING: i changed z_offset from %g to %g to accommodate enlarged bounding box\n", header.z_offset, z_offset);
      header.z_offset = z_offset;
      reoffset = TRUE;
    }
  }

  if (rescale || reoffset)
  {
    if (lasreadersingle)
    {
      delete lasreadersingle;
      if (rescale && reoffset)
        lasreadersingle = new LASreaderSingleRescaleReoffset(header.x_scale_factor, header.y_scale_factor, header.z_scale_factor, header.x_offset, header.y_offset, header.z_offset);
      else if (rescale)
        lasreadersingle = new LASreaderSingleRescale(header.x_scale_factor, header.y_scale_factor, header.z_scale_factor);
      else
        lasreadersingle = new LASreaderSingleReoffset(header.x_offset, header.y_offset, header.z_offset);
      lasreader = lasreadersingle;
    }
    else
    {
      delete lasreaderascii;
      if (rescale && reoffset)
        lasreaderascii = new LASreaderASCIIRescaleReoffset(header.x_scale_factor, header.y_scale_factor, header.z_scale_factor, header.x_offset, header.y_offset, header.z_offset);
      else if (rescale)
        lasreaderascii = new LASreaderASCIIRescale(header.x_scale_factor, header.y_scale_factor, header.z_scale_factor);
      else
        lasreaderascii = new LASreaderASCIIReoffset(header.x_offset, header.y_offset, header.z_offset);
      lasreaderascii->set_translate_intensity(translate_intensity);
      lasreaderascii->set_scale_intensity(scale_intensity);
      lasreaderascii->set_translate_scan_angle(translate_scan_angle);
      lasreaderascii->set_scale_scan_angle(scale_scan_angle);
      lasreaderascii->set_scale_factor(scale_factor);
      lasreaderascii->set_offset(offset);
      lasreader = lasreaderascii;
    }
  }

  p_count = 0;
  file_name_current = 0;

  return TRUE;
}

void LASreaderMulti::set_filter(LASfilter* filter)
{
  this->filter = filter;
}

void LASreaderMulti::set_transform(LAStransform* transform)
{
  this->transform = transform;
}

BOOL LASreaderMulti::inside_tile(const F32 ll_x, const F32 ll_y, const F32 size)
{
  inside = 1;
  t_ll_x = ll_x;
  t_ll_y = ll_y;
  t_size = size;
  t_ur_x = ll_x + size;
  t_ur_y = ll_y + size;
  header.min_x = ll_x;
  header.min_y = ll_y;
  header.max_x = ll_x + size - 0.001f * header.x_scale_factor;
  header.max_y = ll_y + size - 0.001f * header.y_scale_factor;
  return TRUE;
}

BOOL LASreaderMulti::inside_circle(const F64 center_x, const F64 center_y, const F64 radius)
{
  inside = 2;
  c_center_x = center_x;
  c_center_y = center_y;
  c_radius = radius;
  c_radius_squared = radius*radius;
  header.min_x = center_x - radius;
  header.min_y = center_y - radius;
  header.max_x = center_x + radius;
  header.max_y = center_y + radius;
  return TRUE;
}

BOOL LASreaderMulti::inside_rectangle(const F64 min_x, const F64 min_y, const F64 max_x, const F64 max_y)
{
  inside = 3;
  r_min_x = min_x;
  r_min_y = min_y;
  r_max_x = max_x;
  r_max_y = max_y;
  header.min_x = min_x;
  header.min_y = min_y;
  header.max_x = max_x;
  header.max_y = max_y;
  return TRUE;
}
  
BOOL LASreaderMulti::read_point_default()
{
  if (file_name_current == 0)
  {
    if (!open_next_file()) return FALSE;
  }

  while (true)
  {
    if (lasreader->read_point())
    {
      point = lasreader->point;
      p_count++;
      return TRUE;
    }
    if (lasreaderascii)
    {
      if (!populate_header)
      {
        header.number_of_point_records += lasreader->header.number_of_point_records;
        header.number_of_points_by_return[0] += lasreader->header.number_of_points_by_return[0];
        header.number_of_points_by_return[1] += lasreader->header.number_of_points_by_return[1];
        header.number_of_points_by_return[2] += lasreader->header.number_of_points_by_return[2];
        header.number_of_points_by_return[3] += lasreader->header.number_of_points_by_return[3];
        header.number_of_points_by_return[4] += lasreader->header.number_of_points_by_return[4];
        if (header.max_x < lasreader->header.max_x) header.max_x = lasreader->header.max_x;
        if (header.max_y < lasreader->header.max_y) header.max_y = lasreader->header.max_y;
        if (header.max_z < lasreader->header.max_z) header.max_z = lasreader->header.max_z;
        if (header.min_x > lasreader->header.min_x) header.min_x = lasreader->header.min_x;
        if (header.min_y > lasreader->header.min_y) header.min_y = lasreader->header.min_y;
        if (header.min_z > lasreader->header.min_z) header.min_z = lasreader->header.min_z;
      }
    }
    lasreader->close();
    point.zero();
    if (!open_next_file()) return FALSE;
  }
  return FALSE;
}

void LASreaderMulti::close()
{
  if (lasreader) 
  {
    lasreader->close();
  }
}

BOOL LASreaderMulti::reopen()
{
  p_count = 0;
  file_name_current = 0;
  if (filter) filter->reset();
  return TRUE;
}

void LASreaderMulti::clean()
{
  if (lasreader) 
  {
    delete lasreader;
    lasreader = 0;
    lasreaderascii = 0;
    lasreadersingle = 0;
  }
  point_type_change = FALSE;
  rescale = FALSE;
  reoffset = FALSE;
  if (scale_factor)
  {
    delete [] scale_factor;
    scale_factor = 0;
  }
  if (offset)
  {
    delete [] offset;
    offset = 0;
  }
  if (parse_string)
  {
    free(parse_string);
    parse_string = 0;
  }
  skip_lines = 0;
  translate_intensity = 0.0f;
  scale_intensity = 1.0f;
  translate_scan_angle = 0.0f;
  scale_scan_angle = 1.0f;
  populate_header = FALSE;
  if (file_names)
  {
    U32 i;
    for (i = 0; i < file_name_number; i++)
    {
      free(file_names[i]);
    }
    delete [] file_names;
    file_names = 0;
  }
  if (bounding_boxes)
  {
    delete [] bounding_boxes;
    bounding_boxes = 0;
  }
  file_name_current = 0;
  file_name_number = 0;
  file_name_allocated = 0;
  inside = 0;
}

LASreaderMulti::LASreaderMulti()
{
  lasreader = 0;
  lasreaderascii = 0;
  lasreadersingle = 0;
  scale_factor = 0;
  offset = 0;
  parse_string = 0;
  file_names = 0;
  bounding_boxes = 0;
  clean();
}

LASreaderMulti::~LASreaderMulti()
{
  if (lasreader) close();
  clean();
}

BOOL LASreaderMulti::open_next_file()
{
  while (file_name_current < file_name_number)
  {
    if (inside)
    {
      // check if bounding box overlaps requested bounding box
      if (inside < 3) // tile or circle
      {
        if (bounding_boxes[4*file_name_current+0] >= header.max_x) { file_name_current++; continue; }
        if (bounding_boxes[4*file_name_current+1] >= header.max_y) { file_name_current++; continue; }
      }
      else // rectangle
      {
        if (bounding_boxes[4*file_name_current+0] > header.max_x) { file_name_current++; continue; }
        if (bounding_boxes[4*file_name_current+1] > header.max_y) { file_name_current++; continue; }
      }
      if (bounding_boxes[4*file_name_current+2] < header.min_x) { file_name_current++; continue; }
      if (bounding_boxes[4*file_name_current+3] < header.min_y) { file_name_current++; continue; }
    }
    // open the leasreadersingle with the next file name
    if (lasreadersingle)
    {
      if (!lasreadersingle->open(file_names[file_name_current]))
      {
        fprintf(stderr, "ERROR: could not open lasreadersingle for file '%s'\n", file_names[file_name_current]);
        return FALSE;
      }
      LASindex* index = new LASindex;
      if (index->read(file_names[file_name_current]))
        lasreadersingle->set_index(index);
      else
        delete index;
    }
    else
    {
      if (!lasreaderascii->open(file_names[file_name_current], parse_string, skip_lines))
      {
        fprintf(stderr, "ERROR: could not open lasreaderascii for file '%s'\n", file_names[file_name_current]);
        return FALSE;
      }
    }
    if (filter) lasreader->set_filter(filter);
    if (transform) lasreader->set_transform(transform);
    if (inside)
    {
      if (inside == 1) lasreader->inside_tile(t_ll_x, t_ll_y, t_size);
      else if (inside == 2) lasreader->inside_circle(c_center_x, c_center_y, c_radius);
      else lasreader->inside_rectangle(r_min_x, r_min_y, r_max_x, r_max_y);
    }
    file_name_current++;
    return TRUE;
  }
  return FALSE;
}
