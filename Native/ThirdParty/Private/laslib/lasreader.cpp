/*
===============================================================================

  FILE:  LASreader.cpp
  
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
#include "lasreader.hpp"

#include "lasindex.hpp"
#include "lasfilter.hpp"
#include "lastransform.hpp"

#include "lasreadersingle.hpp"
#include "lasreaderascii.hpp"
#include "lasreadermulti.hpp"

#include <stdlib.h>
#include <string.h>

LASreader::LASreader()
{
  npoints = 0;
  p_count = 0;
  read_simple = &LASreader::read_point_default;
  read_complex = 0;
  index = 0;
  filter = 0;
  transform = 0;
  r_min_x = 0;
  r_min_y = 0;
  r_max_x = 0;
  r_max_y = 0;
  t_ll_x = 0;
  t_ll_y = 0;
  t_size = 0;
  t_ur_x = 0;
  t_ur_y = 0;
  c_center_x = 0;
  c_center_y = 0;
  c_radius = 0;
  c_radius_squared = 0;
}
  
LASreader::~LASreader()
{
  if (index) delete index;
}

void LASreader::set_index(LASindex* index)
{
  if (this->index) delete this->index;
  this->index = index;
}

void LASreader::set_filter(LASfilter* filter)
{
  this->filter = filter;
  if (filter && transform)
  {
    read_simple = &LASreader::read_point_filtered_and_transformed;
  }
  else if (filter)
  {
    read_simple = &LASreader::read_point_filtered;
  }
  else if (transform)
  {
    read_simple = &LASreader::read_point_transformed;
  }
  read_complex = &LASreader::read_point_default;
}

void LASreader::set_transform(LAStransform* transform)
{
  this->transform = transform;
  if (filter && transform)
  {
    read_simple = &LASreader::read_point_filtered_and_transformed;
  }
  else if (filter)
  {
    read_simple = &LASreader::read_point_filtered;
  }
  else if (transform)
  {
    read_simple = &LASreader::read_point_transformed;
  }
  read_complex = &LASreader::read_point_default;
}

void LASreader::reset_filter()
{
  if (filter) filter->reset();
}

BOOL LASreader::inside_tile(const F32 ll_x, const F32 ll_y, const F32 size)
{
  t_ll_x = ll_x;
  t_ll_y = ll_y;
  t_size = size;
  t_ur_x = ll_x + size;
  t_ur_y = ll_y + size;
  header.min_x = ll_x;
  header.min_y = ll_y;
  header.max_x = ll_x + size - 0.001f * header.x_scale_factor;
  header.max_y = ll_y + size - 0.001f * header.y_scale_factor;
  if (index) index->intersect_tile(ll_x, ll_y, size);
  if (filter || transform)
  {
    if (index)
      read_complex = &LASreader::read_point_inside_tile_indexed;
    else
      read_complex = &LASreader::read_point_inside_tile;
  }
  else
  {
    if (index)
      read_simple = &LASreader::read_point_inside_tile_indexed;
    else
      read_simple = &LASreader::read_point_inside_tile;
  }
  return TRUE;
}

BOOL LASreader::inside_circle(const F64 center_x, const F64 center_y, const F64 radius)
{
  c_center_x = center_x;
  c_center_y = center_y;
  c_radius = radius;
  c_radius_squared = radius*radius;
  header.min_x = center_x - radius;
  header.min_y = center_y - radius;
  header.max_x = center_x + radius;
  header.max_y = center_y + radius;
  if (index) index->intersect_circle(center_x, center_y, radius);
  if (filter || transform)
  {
    if (index)
      read_complex = &LASreader::read_point_inside_circle_indexed;
    else
      read_complex = &LASreader::read_point_inside_circle;
  }
  else
  {
    if (index)
      read_simple = &LASreader::read_point_inside_circle_indexed;
    else
      read_simple = &LASreader::read_point_inside_circle;
  }
  return TRUE;
}

BOOL LASreader::inside_rectangle(const F64 min_x, const F64 min_y, const F64 max_x, const F64 max_y)
{
  r_min_x = min_x;
  r_min_y = min_y;
  r_max_x = max_x;
  r_max_y = max_y;
  header.min_x = min_x;
  header.min_y = min_y;
  header.max_x = max_x;
  header.max_y = max_y;
  if (index) index->intersect_rectangle(min_x, min_y, max_x, max_y);
  if (filter || transform)
  {
    if (index)
      read_complex = &LASreader::read_point_inside_rectangle_indexed;
    else
      read_complex = &LASreader::read_point_inside_rectangle;
  }
  else
  {
    if (index)
      read_simple = &LASreader::read_point_inside_rectangle_indexed;
    else
      read_simple = &LASreader::read_point_inside_rectangle;
  }
  return TRUE;
}

BOOL LASreader::read_point_inside_tile()
{
  while (read_point_default())
  {
    if (point.inside_tile(t_ll_x, t_ll_y, t_ur_x, t_ur_y)) return TRUE;
  }
  return FALSE;
}

BOOL LASreader::read_point_inside_tile_indexed()
{
  while (index->seek_next((LASreader*)this))
  {
    if (read_point_default() && point.inside_tile(t_ll_x, t_ll_y, t_ur_x, t_ur_y)) return TRUE;
  }
  return FALSE;
}

BOOL LASreader::read_point_inside_circle()
{
  while (read_point_default())
  {
    if (point.inside_circle(c_center_x, c_center_y, c_radius_squared)) return TRUE;
  }
  return FALSE;
}

BOOL LASreader::read_point_inside_circle_indexed()
{
  while (index->seek_next((LASreader*)this))
  {
    if (read_point_default() && point.inside_circle(c_center_x, c_center_y, c_radius_squared)) return TRUE;
  }
  return FALSE;
}

BOOL LASreader::read_point_inside_rectangle()
{
  while (read_point_default())
  {
    if (point.inside_rectangle(r_min_x, r_min_y, r_max_x, r_max_y)) return TRUE;
  }
  return FALSE;
}

BOOL LASreader::read_point_inside_rectangle_indexed()
{
  while (index->seek_next((LASreader*)this))
  {
    if (read_point_default() && point.inside_rectangle(r_min_x, r_min_y, r_max_x, r_max_y)) return TRUE;
  }
  return FALSE;
}

BOOL LASreader::read_point_filtered()
{
  while ((this->*read_complex)())
  {
    if (!filter->filter(&point)) return TRUE;
  }
  return FALSE;
}

BOOL LASreader::read_point_transformed()
{
  if ((this->*read_complex)())
  {
    transform->transform(&point);
    return TRUE;
  }
  return FALSE;
}

BOOL LASreader::read_point_filtered_and_transformed()
{
  if (read_point_filtered())
  {
    transform->transform(&point);
    return TRUE;
  }
  return FALSE;
}

BOOL LASreadOpener::piped() const
{
  return (!file_names && use_stdin);
}

BOOL LASreadOpener::has_populated_header() const
{
  return (populate_header || (file_name && (strstr(file_name, ".las") || strstr(file_name, ".laz") || strstr(file_name, ".LAS") || strstr(file_name, ".LAZ"))));
}

void LASreadOpener::reset()
{
  file_name_current = 0;
  file_name = 0;
}

LASreader* LASreadOpener::open()
{
  if (file_names)
  {
    use_stdin = FALSE;
    if (file_name_current == file_name_number) return 0;
    if ((file_name_number > 1) && merged)
    {
      LASreaderMulti* lasreadermulti = new LASreaderMulti();
      lasreadermulti->set_scale_factor(scale_factor);
      lasreadermulti->set_offset(offset);
      lasreadermulti->set_parse_string(parse_string);
      lasreadermulti->set_skip_lines(skip_lines);
      lasreadermulti->set_populate_header(populate_header);
      lasreadermulti->set_translate_intensity(translate_intensity);
      lasreadermulti->set_scale_intensity(scale_intensity);
      lasreadermulti->set_translate_scan_angle(translate_scan_angle);
      lasreadermulti->set_scale_scan_angle(scale_scan_angle);
      for (file_name_current = 0; file_name_current < file_name_number; file_name_current++) lasreadermulti->add_file_name(file_names[file_name_current]);
      if (!lasreadermulti->open())
      {
        fprintf(stderr,"ERROR: cannot open lasreadermulti with %d file names\n", file_name_number);
        delete lasreadermulti;
        return 0;
      }
      if (filter) lasreadermulti->set_filter(filter);
      if (transform) lasreadermulti->set_transform(transform);
      if (inside_tile) lasreadermulti->inside_tile(inside_tile[0], inside_tile[1], inside_tile[2]);
      if (inside_circle) lasreadermulti->inside_circle(inside_circle[0], inside_circle[1], inside_circle[2]);
      if (inside_rectangle) lasreadermulti->inside_rectangle(inside_rectangle[0], inside_rectangle[1], inside_rectangle[2], inside_rectangle[3]);
      return lasreadermulti;
    }
    else
    {
      file_name = file_names[file_name_current];
      file_name_current++;
      if (strstr(file_name, ".las") || strstr(file_name, ".laz") || strstr(file_name, ".LAS") || strstr(file_name, ".LAZ"))
      {
        LASreaderSingle* lasreadersingle;
        if (scale_factor == 0 && offset == 0)
          lasreadersingle = new LASreaderSingle();
        else if (scale_factor != 0 && offset == 0)
          lasreadersingle = new LASreaderSingleRescale(scale_factor[0], scale_factor[1], scale_factor[2]);
        else if (scale_factor == 0 && offset != 0)
          lasreadersingle = new LASreaderSingleReoffset(offset[0], offset[1], offset[2]);
        else
          lasreadersingle = new LASreaderSingleRescaleReoffset(scale_factor[0], scale_factor[1], scale_factor[2], offset[0], offset[1], offset[2]);
        if (!lasreadersingle->open(file_name))
        {
          fprintf(stderr,"ERROR: cannot open lasreadersingle with file name '%s'\n", file_name);
          delete lasreadersingle;
          return 0;
        }
        LASindex* index = new LASindex();
        if (index->read(file_name))
          lasreadersingle->set_index(index);
        else
          delete index;
        if (filter) lasreadersingle->set_filter(filter);
        if (transform) lasreadersingle->set_transform(transform);
        if (inside_tile) lasreadersingle->inside_tile(inside_tile[0], inside_tile[1], inside_tile[2]);
        if (inside_circle) lasreadersingle->inside_circle(inside_circle[0], inside_circle[1], inside_circle[2]);
        if (inside_rectangle) lasreadersingle->inside_rectangle(inside_rectangle[0], inside_rectangle[1], inside_rectangle[2], inside_rectangle[3]);
        return lasreadersingle;
      }
      else
      {
        LASreaderASCII* lasreaderascii = new LASreaderASCII();
        lasreaderascii->set_translate_intensity(translate_intensity);
        lasreaderascii->set_scale_intensity(scale_intensity);
        lasreaderascii->set_translate_scan_angle(translate_scan_angle);
        lasreaderascii->set_scale_scan_angle(scale_scan_angle);
        lasreaderascii->set_scale_factor(scale_factor);
        lasreaderascii->set_offset(offset);
        if (!lasreaderascii->open(file_name, parse_string, skip_lines, populate_header))
        {
          fprintf(stderr,"ERROR: cannot open lasreaderascii with file name '%s'\n", file_name);
          delete lasreaderascii;
          return 0;
        }
        if (filter) lasreaderascii->set_filter(filter);
        if (transform) lasreaderascii->set_transform(transform);
        if (inside_tile) lasreaderascii->inside_tile(inside_tile[0], inside_tile[1], inside_tile[2]);
        if (inside_circle) lasreaderascii->inside_circle(inside_circle[0], inside_circle[1], inside_circle[2]);
        if (inside_rectangle) lasreaderascii->inside_rectangle(inside_rectangle[0], inside_rectangle[1], inside_rectangle[2], inside_rectangle[3]);
        return lasreaderascii;
      }
    }
  }
  else if (use_stdin)
  {
    use_stdin = FALSE; populate_header = TRUE;
    LASreaderSingle* lasreadersingle;
    if (scale_factor == 0 && offset == 0)
      lasreadersingle = new LASreaderSingle();
    else if (scale_factor != 0 && offset == 0)
      lasreadersingle = new LASreaderSingleRescale(scale_factor[0], scale_factor[1], scale_factor[2]);
    else if (scale_factor == 0 && offset != 0)
      lasreadersingle = new LASreaderSingleReoffset(offset[0], offset[1], offset[2]);
    else
      lasreadersingle = new LASreaderSingleRescaleReoffset(scale_factor[0], scale_factor[1], scale_factor[2], offset[0], offset[1], offset[2]);
    if (!lasreadersingle->open(stdin))
    {
      fprintf(stderr,"ERROR: cannot open lasreadersingle from stdin \n");
      delete lasreadersingle;
      return 0;
    }
    if (filter) lasreadersingle->set_filter(filter);
    if (transform) lasreadersingle->set_transform(transform);
    if (inside_tile) lasreadersingle->inside_tile(inside_tile[0], inside_tile[1], inside_tile[2]);
    if (inside_circle) lasreadersingle->inside_circle(inside_circle[0], inside_circle[1], inside_circle[2]);
    if (inside_rectangle) lasreadersingle->inside_rectangle(inside_rectangle[0], inside_rectangle[1], inside_rectangle[2], inside_rectangle[3]);
    return lasreadersingle;
  }
  else
  {
    return 0;
  }
}

BOOL LASreadOpener::reopen(LASreader* lasreader)
{
  if (file_names)
  {
    if ((file_name_number > 1) && merged)
    {
      LASreaderMulti* lasreadermulti = (LASreaderMulti*)lasreader;
      if (!lasreadermulti->reopen())
      {
        fprintf(stderr,"ERROR: cannot reopen lasreadermulti\n");
        return FALSE;
      }
      lasreadermulti->reset_filter();
      return TRUE;
    }
    else
    {
      if (!file_name) return FALSE;
      if (strstr(file_name, ".las") || strstr(file_name, ".laz") || strstr(file_name, ".LAS") || strstr(file_name, ".LAZ"))
      {
        LASreaderSingle* lasreadersingle = (LASreaderSingle*)lasreader;
        if (!lasreadersingle->open(file_name))
        {
          fprintf(stderr,"ERROR: cannot reopen lasreadersingle with file name '%s'\n", file_name);
          return FALSE;
        }
        lasreadersingle->reset_filter();
        return TRUE;
      }
      else
      {
        LASreaderASCII* lasreaderascii = (LASreaderASCII*)lasreader;
        if (!lasreaderascii->reopen(file_name))
        {
          fprintf(stderr,"ERROR: cannot reopen lasreaderascii with file name '%s'\n", file_name);
          return FALSE;
        }
        lasreaderascii->reset_filter();
        return TRUE;
      }
    }
  }
  else
  {
    fprintf(stderr,"ERROR: no lasreader input specified\n");
    return FALSE;
  }
}

void LASreadOpener::usage() const
{
  fprintf(stderr,"Supported LAS Inputs\n");
  fprintf(stderr,"  -i lidar.las\n");
  fprintf(stderr,"  -i lidar.laz\n");
  fprintf(stderr,"  -i lidar1.las lidar2.las lidar3.las -merged\n");
  fprintf(stderr,"  -i *.las\n");
  fprintf(stderr,"  -i flight0??.laz flight1??.laz -single\n");
  fprintf(stderr,"  -i lidar.txt -iparse xyzti (on-the-fly from ASCII)\n");
  fprintf(stderr,"  -i lidar.txt -iparse xyzi -itranslate_intensity 1024\n");
  fprintf(stderr,"  -lof file_list.txt\n");
  fprintf(stderr,"  -stdin (pipe from stdin)\n");
  fprintf(stderr,"  -rescale 0.1 0.1 0.1\n");
  fprintf(stderr,"  -reoffset 600000 4000000 0\n");
}

BOOL LASreadOpener::parse(int argc, char* argv[])
{
  int i;
  for (i = 1; i < argc; i++)
  {
    if (argv[i][0] == '\0')
    {
      continue;
    }
    else if (strcmp(argv[i],"-h") == 0 || strcmp(argv[i],"-help") == 0)
    {
      LASfilter().usage();
      LAStransform().usage();
      usage();
      return TRUE;
    }
    else if (strcmp(argv[i],"-i") == 0)
    {
      if ((i+1) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs at least 1 argument: file_name or wild_card\n", argv[i]);
        return FALSE;
      }
      *argv[i]='\0';
      i+=1;
      do
      {
#ifdef _WIN32
        add_file_name_windows(argv[i]);
#else
        add_file_name(argv[i]);
#endif
        *argv[i]='\0';
        i+=1;
      } while (i < argc && *argv[i] != '-');
      i-=1;
    }
    else if (strcmp(argv[i],"-inside_tile") == 0)
    {
      if ((i+3) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs 3 arguments: ll_x, ll_y, size\n", argv[i]);
        return FALSE;
      }
      if (inside_tile == 0) inside_tile = new F32[3];
      inside_tile[0] = (F32)atof(argv[i+1]);
      inside_tile[1] = (F32)atof(argv[i+2]);
      inside_tile[2] = (F32)atof(argv[i+3]);
      *argv[i]='\0'; *argv[i+1]='\0'; *argv[i+2]='\0'; *argv[i+3]='\0'; i+=3; 
    }
    else if (strcmp(argv[i],"-inside_circle") == 0)
    {
      if ((i+3) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs 3 arguments: center_x, center_y, radius\n", argv[i]);
        return FALSE;
      }
      if (inside_circle == 0) inside_circle = new F64[3];
      inside_circle[0] = atof(argv[i+1]);
      inside_circle[1] = atof(argv[i+2]);
      inside_circle[2] = atof(argv[i+3]);
      *argv[i]='\0'; *argv[i+1]='\0'; *argv[i+2]='\0'; *argv[i+3]='\0'; i+=3;
    }
    else if (strcmp(argv[i],"-inside") == 0 || strcmp(argv[i],"-inside_rectangle") == 0)
    {
      if ((i+4) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs 4 arguments: min_x, min_y, max_x, max_y\n", argv[i]);
        return FALSE;
      }
      if (inside_rectangle == 0) inside_rectangle = new F64[4];
      inside_rectangle[0] = atof(argv[i+1]);
      inside_rectangle[1] = atof(argv[i+2]);
      inside_rectangle[2] = atof(argv[i+3]);
      inside_rectangle[3] = atof(argv[i+4]);
      *argv[i]='\0'; *argv[i+1]='\0'; *argv[i+2]='\0'; *argv[i+3]='\0'; *argv[i+4]='\0'; i+=4; 
    }
    else if (strcmp(argv[i],"-stdin") == 0)
    {
      use_stdin = TRUE;
      *argv[i]='\0';
    }
    else if (strcmp(argv[i],"-lof") == 0)
    {
      if ((i+1) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs 1 argument: list_of_files\n", argv[i]);
        return FALSE;
      }
      FILE* file = fopen(argv[i+1], "r");
      if (file == 0)
      {
        fprintf(stderr, "ERROR: cannot open '%s'\n", argv[i+1]);
        return FALSE;
      }
      char line[1024];
      while (fgets(line, 1024, file))
      {
        // find end of line
        int len = strlen(line) - 1;
        // remove extra white spaces and line return at the end 
        while (len > 0 && ((line[len] == '\n') || (line[len] == ' ') || (line[len] == '\t') || (line[len] == '\012')))
        {
          line[len] = '\0';
          len--;
        }
        add_file_name(line);
      }
      *argv[i]='\0'; *argv[i+1]='\0'; i+=1;
    }
    else if (strcmp(argv[i],"-rescale") == 0)
    {
      if ((i+3) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs 3 arguments: rescale_x, rescale_y, rescale_z\n", argv[i]);
        return FALSE;
      }
      F64 scale_factor[3];
      scale_factor[0] = atof(argv[i+1]);
      scale_factor[1] = atof(argv[i+2]);
      scale_factor[2] = atof(argv[i+3]);
      set_scale_factor(scale_factor);
      *argv[i]='\0'; *argv[i+1]='\0'; *argv[i+2]='\0'; *argv[i+3]='\0'; i+=3;
    }
    else if (strcmp(argv[i],"-reoffset") == 0)
    {
      if ((i+3) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs 3 arguments: reoffset_x, reoffset_y, reoffset_z\n", argv[i]);
        return FALSE;
      }
      F64 offset[3];
			offset[0] = atof(argv[i+1]);
			offset[1] = atof(argv[i+2]);
			offset[2] = atof(argv[i+3]);
      set_offset(offset);
      *argv[i]='\0'; *argv[i+1]='\0'; *argv[i+2]='\0'; *argv[i+3]='\0'; i+=3;
    }
    else if (strcmp(argv[i],"-itranslate_intensity") == 0)
    {
      if ((i+1) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs 1 argument: offset\n", argv[i]);
        return FALSE;
      }
      set_translate_intensity((F32)atof(argv[i+1]));
      *argv[i]='\0'; *argv[i+1]='\0'; i+=1;
    }
    else if (strcmp(argv[i],"-iscale_intensity") == 0)
    {
      if ((i+1) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs 1 argument: scale\n", argv[i]);
        return FALSE;
      }
      set_scale_intensity((F32)atof(argv[i+1]));
      *argv[i]='\0'; *argv[i+1]='\0'; i+=1;
    }
    else if (strcmp(argv[i],"-itranslate_scan_angle") == 0)
    {
      if ((i+1) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs 1 argument: offset\n", argv[i]);
        return FALSE;
      }
      set_translate_scan_angle((F32)atof(argv[i+1]));
      *argv[i]='\0'; *argv[i+1]='\0'; i+=1;
    }
    else if (strcmp(argv[i],"-iscale_scan_angle") == 0)
    {
      if ((i+1) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs 1 argument: scale\n", argv[i]);
        return FALSE;
      }
      set_scale_scan_angle((F32)atof(argv[i+1]));
      *argv[i]='\0'; *argv[i+1]='\0'; i+=1;
    }
    else if (strcmp(argv[i],"-iparse") == 0)
    {
      if ((i+1) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs 1 argument: string\n", argv[i]);
        return FALSE;
      }
      set_parse_string(argv[i+1]);
      *argv[i]='\0'; *argv[i+1]='\0'; i+=1;
    }
    else if (strcmp(argv[i],"-skip") == 0)
    {
      if ((i+1) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs 1 argument: number_of_lines\n", argv[i]);
        return FALSE;
      }
      set_skip_lines(atoi(argv[i+1]));
      *argv[i]='\0'; *argv[i+1]='\0'; i+=1;
    }
    else if (strcmp(argv[i],"-single") == 0)
    {
      set_merged(FALSE);
      *argv[i]='\0';
    }
    else if (strcmp(argv[i],"-merged") == 0)
    {
      set_merged(TRUE);
      *argv[i]='\0';
    }
    else if (strcmp(argv[i],"-populate") == 0)
    {
      set_populate_header(TRUE);
      *argv[i]='\0';
    }
    else if (strcmp(argv[i],"-do_not_populate") == 0)
    {
      set_populate_header(FALSE);
      *argv[i]='\0';
    }
  }

  if (filter) filter->clean();
  else filter = new LASfilter();
  if (!filter->parse(argc, argv))
  {
    delete filter;
    return FALSE;
  }
  if (!filter->active())
  {
    delete filter;
    filter = 0;
  }

  if (transform) transform->clean();
  else transform = new LAStransform();
  if (!transform->parse(argc, argv))
  {
    delete transform;
    return FALSE;
  }
  if (!transform->active())
  {
    delete transform;
    transform = 0;
  }

  return TRUE;
}

const char* LASreadOpener::get_file_name() const
{
  if (file_name)
    return file_name;
  if (file_name_number)
    return file_names[0];
  return 0;
}

const char* LASreadOpener::get_file_name(U32 number) const
{
  return file_names[number];
}

void LASreadOpener::set_merged(const BOOL merged)
{
  this->merged = merged;
}

BOOL LASreadOpener::get_merged() const
{
  return merged;
}

void LASreadOpener::set_file_name(const char* file_name, BOOL unique)
{
#ifdef _WIN32
  add_file_name_windows(file_name, unique);
#else
  add_file_name(file_name, unique);
#endif
}

BOOL LASreadOpener::add_file_name(const char* file_name, BOOL unique)
{
  if (unique)
  {
    U32 i;
    for (i = 0; i < file_name_number; i++)
    {
      if (strcmp(file_names[i], file_name) == 0)
      {
        return FALSE;
      }
    }
  }
  if (file_name_number == file_name_allocated)
  {
    if (file_names)
    {
      file_name_allocated *= 2;
      file_names = (char**)realloc(file_names, sizeof(char*)*file_name_allocated);
    }
    else
    {
      file_name_allocated = 16;
      file_names = (char**)malloc(sizeof(char*)*file_name_allocated);
    }
    if (file_names == 0)
    {
      fprintf(stderr, "ERROR: alloc for file_names pointer array failed at %d\n", file_name_allocated);
    }
  }
  file_names[file_name_number] = strdup(file_name);
  file_name_number++;
  return TRUE;
}

void LASreadOpener::delete_file_name(U32 file_name_id)
{
  if (file_name_id < file_name_number)
  {
    U32 i;
    free(file_names[file_name_id]);
    for (i = file_name_id+1; i < file_name_number; i++)
    {
      file_names[i-i] = file_names[i];
    }
  }
  file_name_number--;
}

BOOL LASreadOpener::set_file_name_current(U32 file_name_id)
{
  if (file_name_id < file_name_number)
  {
    file_name_current = file_name_id;
    return TRUE;
  }
  return FALSE;
}

#ifdef _WIN32
#include <windows.h>
void LASreadOpener::add_file_name_windows(const char* file_name, BOOL unique)
{
  HANDLE h;
  WIN32_FIND_DATA info;
  h = FindFirstFile(file_name, &info);
  if (h != INVALID_HANDLE_VALUE)
  {
    // find the path
    int len = strlen(file_name);
    while (len && file_name[len] != '\\') len--;
    if (len)
    {
      len++;
      char full_file_name[512];
      strncpy(full_file_name, file_name, len);
	    do
	    {
        sprintf(&full_file_name[len], "%s", info.cFileName);
        add_file_name(full_file_name, unique);
  	  } while (FindNextFile(h, &info));
    }
    else
    {
      do
      {
        add_file_name(info.cFileName);
  	  } while (FindNextFile(h, &info));
    }
	  FindClose(h);
  }
}
#endif

U32 LASreadOpener::get_file_name_number() const
{
  return file_name_number;
}

void LASreadOpener::set_parse_string(const char* parse_string)
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

const char* LASreadOpener::get_parse_string() const
{
  return parse_string;
}

void LASreadOpener::set_scale_factor(const F64* scale_factor)
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

void LASreadOpener::set_offset(const F64* offset)
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

void LASreadOpener::set_translate_intensity(F32 translate_intensity)
{
  this->translate_intensity = translate_intensity;
}

void LASreadOpener::set_scale_intensity(F32 scale_intensity)
{
  this->scale_intensity = scale_intensity;
}

void LASreadOpener::set_translate_scan_angle(F32 translate_scan_angle)
{
  this->translate_scan_angle = translate_scan_angle;
}

void LASreadOpener::set_scale_scan_angle(F32 scale_scan_angle)
{
  this->scale_scan_angle = scale_scan_angle;
}

void LASreadOpener::set_skip_lines(I32 skip_lines)
{
  this->skip_lines = skip_lines;
}

void LASreadOpener::set_populate_header(BOOL populate_header)
{
  this->populate_header = populate_header;
}

BOOL LASreadOpener::active() const
{
  return ((file_name_current < file_name_number) || use_stdin);
}

LASreadOpener::LASreadOpener()
{
  file_names = 0;
  file_name = 0;
  merged = TRUE;
  use_stdin = FALSE;
  scale_factor = 0;
  offset = 0;
  translate_intensity = 0.0f;
  scale_intensity = 1.0f;
  translate_scan_angle = 0.0f;
  scale_scan_angle = 1.0f;
  parse_string = 0;
  skip_lines = 0;
  populate_header = TRUE;
  file_name_number = 0;
  file_name_allocated = 0;
  file_name_current = 0;
  inside_tile = 0;
  inside_circle = 0;
  inside_rectangle = 0;
  filter = 0;
  transform = 0;
}

LASreadOpener::~LASreadOpener()
{
  if (file_names)
  {
    U32 i;
    for (i = 0; i < file_name_number; i++) free(file_names[i]);
    free(file_names);
  }
  if (parse_string) free(parse_string);
  if (scale_factor) delete [] scale_factor;
  if (offset) delete [] offset;
  if (inside_tile) delete [] inside_tile;
  if (inside_circle) delete [] inside_circle;
  if (inside_rectangle) delete [] inside_rectangle;
  if (filter) delete filter;
  if (transform) delete transform;
}
