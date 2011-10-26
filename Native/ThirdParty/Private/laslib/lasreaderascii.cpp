/*
===============================================================================

  FILE:  lasreaderascii.cpp
  
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
#include "lasreaderascii.hpp"

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

extern "C" FILE* fopen_compressed(const char* filename, const char* mode, bool* piped);

BOOL LASreaderASCII::open(const char* file_name, const char* parse_string, I32 skip_lines, BOOL populate_header)
{
  int i;

  if (file_name == 0)
  {
    fprintf(stderr,"ERROR: fine name pointer is zero\n");
    return FALSE;
  }

  if (parse_string && !check_parse_string(parse_string))
  {
    return FALSE;
  }

  clean();
  header.clean();

  file = fopen_compressed(file_name, "r", &piped);
  if (file == 0)
  {
    fprintf(stderr, "ERROR: cannot open file '%s'\n", file_name);
    return FALSE;
  }

  // populate the header as much as it makes sense

  strncpy(header.system_identifier, "LAStools (c) by Martin Isenburg", 32);
  header.system_identifier[31] = '\0';
  strncpy(header.generating_software, "created by LASreaderASCII", 32);
  header.generating_software[31] = '\0';
#ifdef _WIN32
  WIN32_FILE_ATTRIBUTE_DATA attr;
	SYSTEMTIME creation;
  GetFileAttributesEx(file_name, GetFileExInfoStandard, &attr);
	FileTimeToSystemTime(&attr.ftCreationTime, &creation);
  int startday[13] = {-1, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
  header.file_creation_day = startday[creation.wMonth] + creation.wDay;
  header.file_creation_year = creation.wYear;
  // leap year handling
  if ((((creation.wYear)%4) == 0) && (creation.wMonth > 2)) header.file_creation_day++;
#else
  header.file_creation_day = 111;
  header.file_creation_year = 2011;
#endif
  if (parse_string)
  {
    if (strstr(parse_string,"t"))
    {
      if (strstr(parse_string,"R") || strstr(parse_string,"G") || strstr(parse_string,"B"))
      {
        header.point_data_format = 3;
        header.point_data_record_length = 34;
      }
      else
      {
        header.point_data_format = 1;
        header.point_data_record_length = 28;
      }
    }
    else
    {
      if (strstr(parse_string,"R") || strstr(parse_string,"G") || strstr(parse_string,"B"))
      {
        header.point_data_format = 2;
        header.point_data_record_length = 26;
      }
      else
      {
        header.point_data_format = 0;
        header.point_data_record_length = 20;
      }
    }
  }
  else
  {
    header.point_data_format = 0;
    header.point_data_record_length = 20;
  }

  // initialize point

  point.init(&header, header.point_data_format, header.point_data_record_length);

  // we do not know yet how many points to expect
  
  npoints = 0;

  // should we perform an extra pass to fully populate the header

  if (populate_header)
  {
    // create a cheaper parse string that only looks for 'x' 'y' 'z' and 'r'

    char* parse_less;
    if (parse_string == 0)
    {
      parse_less = strdup("xyz");
    }
    else
    {
      parse_less = strdup(parse_string);
      for (i = 0; i < (int)strlen(parse_string); i++)
      {
        if (parse_less[i] != 'x' && parse_less[i] != 'y' && parse_less[i] != 'z' && parse_less[i] != 'r') 
        {
          parse_less[i] = 's';
        }
      }
      do
      {
        parse_less[i] = '\0';
        i--;
      } while (parse_less[i] == 's');
    }

    // skip lines if we have to

    for (i = 0; i < skip_lines; i++) fgets(line, 512, file);

    // read the first line

    while (fgets(line, 512, file))
    {
      if (parse(parse_less))
      {
        // mark that we found the first point
        npoints++;
        // we can stop this loop
        break;
      }
      else
      {
        line[strlen(line)-1] = '\0';
        fprintf(stderr, "WARNING: cannot parse '%s' with '%s'. skipping ...\n", line, parse_less);
      }
    }

    // did we manage to parse a line

    if (npoints == 0)
    {
      fprintf(stderr, "ERROR: could not parse any lines with '%s'\n", parse_less);
      fclose(file);
      file = 0;
      free(parse_less);    
      return FALSE;
    }

    // init the bounding box
    header.min_x = header.max_x = point.coordinates[0];
    header.min_y = header.max_y = point.coordinates[1];
    header.min_z = header.max_z = point.coordinates[2];
    // create return histogram
    if (point.return_number >= 1 && point.return_number <= 5) header.number_of_points_by_return[point.return_number-1]++;

    // loop over the remaining lines

    while (fgets(line, 512, file))
    {
      if (parse(parse_less))
      {
        // count points
        npoints++;
        // create return histogram
        if (point.return_number >= 1 && point.return_number <= 5) header.number_of_points_by_return[point.return_number-1]++;
        // update bounding box
        if (point.coordinates[0] < header.min_x) header.min_x = point.coordinates[0];
        else if (point.coordinates[0] > header.max_x) header.max_x = point.coordinates[0];
        if (point.coordinates[1] < header.min_y) header.min_y = point.coordinates[1];
        else if (point.coordinates[1] > header.max_y) header.max_y = point.coordinates[1];
        if (point.coordinates[2] < header.min_z) header.min_z = point.coordinates[2];
        else if (point.coordinates[2] > header.max_z) header.max_z = point.coordinates[2];
      }
      else
      {
        line[strlen(line)-1] = '\0';
        fprintf(stderr, "WARNING: cannot parse '%s' with '%s'. skipping ...\n", line, parse_less);
      }
    }
    header.number_of_point_records = (U32)npoints;
    free(parse_less);

    // close the input file
    
    fclose(file);
    
    // populate scale and offset

    populate_scale_and_offset();

    // populate bounding box

    populate_bounding_box();

    // mark that header is already populated

    populated_header = TRUE;

    // reopen input file for the second pass

    file = fopen_compressed(file_name, "r", &piped);
    if (file == 0)
    {
      fprintf(stderr, "ERROR: could not open '%s' for second pass\n", file_name);
      return FALSE;
    }
  }

  if (parse_string == 0)
  {
    this->parse_string = strdup("xyz");
  }
  else
  {
    this->parse_string = strdup(parse_string);
  }

  // skip lines if we have to

  for (i = 0; i < skip_lines; i++) fgets(line, 512, file);
  this->skip_lines = skip_lines;

  // read the first line with full parse_string

  i = 0;
  while (fgets(line, 512, file))
  {
    if (parse(this->parse_string))
    {
      // mark that we found the first point
      i = 1;
      break;
    }
    else
    {
      line[strlen(line)-1] = '\0';
      fprintf(stderr, "WARNING: cannot parse '%s' with '%s'. skipping ...\n", line, this->parse_string);
    }
  }

  // did we manage to parse a line

  if (i != 1)
  {
    fprintf(stderr, "ERROR: could not parse any lines with '%s'\n", this->parse_string);
    fclose(file);
    file = 0;
    free(this->parse_string);
    this->parse_string = 0;
    return FALSE;
  }
  
  if (!populated_header)
  {
    // init the bounding box that we will incrementally compute

    header.min_x = header.max_x = point.coordinates[0];
    header.min_y = header.max_y = point.coordinates[1];
    header.min_z = header.max_z = point.coordinates[2];

    // set scale and offset

    populate_scale_and_offset();
  }

  p_count = 0;

  return TRUE;
}

void LASreaderASCII::set_translate_intensity(F32 translate_intensity)
{
  this->translate_intensity = translate_intensity;
}

void LASreaderASCII::set_scale_intensity(F32 scale_intensity)
{
  this->scale_intensity = scale_intensity;
}

void LASreaderASCII::set_translate_scan_angle(F32 translate_scan_angle)
{
  this->translate_scan_angle = translate_scan_angle;
}

void LASreaderASCII::set_scale_scan_angle(F32 scale_scan_angle)
{
  this->scale_scan_angle = scale_scan_angle;
}

void LASreaderASCII::set_scale_factor(const F64* scale_factor)
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

void LASreaderASCII::set_offset(const F64* offset)
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

BOOL LASreaderASCII::seek(const I64 p_index)
{
  U32 delta = 0;
  if (p_index > p_count)
  {
    delta = (U32)(p_index - p_count);
  }
  else if (p_index < p_count)
  {
    if (piped) return FALSE;
    fseek(file, 0, SEEK_SET);
    // skip lines if we have to
    int i;
    for (i = 0; i < skip_lines; i++) fgets(line, 512, file);
    this->skip_lines = skip_lines;
    // read the first line with full parse_string
    i = 0;
    while (fgets(line, 512, file))
    {
      if (parse(this->parse_string))
      {
        // mark that we found the first point
        i = 1;
        break;
      }
      else
      {
        line[strlen(line)-1] = '\0';
        fprintf(stderr, "WARNING: cannot parse '%s' with '%s'. skipping ...\n", line, this->parse_string);
      }
    }
    // did we manage to parse a line
    if (i != 1)
    {
      fprintf(stderr, "ERROR: could not parse any lines with '%s'\n", this->parse_string);
      fclose(file);
      file = 0;
      free(this->parse_string);
      this->parse_string = 0;
      return FALSE;
    }
    delta = (U32)p_index;
  }
  while (delta)
  {
    read_point_default();
    delta--;
  }
  p_count = p_index;
  return TRUE;
}

BOOL LASreaderASCII::read_point_default()
{
  if (p_count)
  {
    while (true)
    {
      if (fgets(line, 512, file))
      {
        if (parse(parse_string))
        {
          break;
        }
        else
        {
          line[strlen(line)-1] = '\0';
          fprintf(stderr, "WARNING: cannot parse '%s' with '%s'. skipping ...\n", line, this->parse_string);
        }
      }
      else
      {
        if (npoints)
        {
          if (p_count != npoints)
          {
#ifdef _WIN32
            fprintf(stderr,"WARNING: end-of-file after %I64d of %I64d points\n", p_count, npoints);
#else
            fprintf(stderr,"WARNING: end-of-file after %lld of %lld points\n", p_count, npoints);
#endif
          }
        }
        else
        {
          npoints = p_count;
          populate_bounding_box();
        }
        return FALSE;
      }
    }
  }
  // compute the quantized x, y, and z values
  point.x = header.get_x(point.coordinates[0]);
  point.y = header.get_y(point.coordinates[1]);
  point.z = header.get_z(point.coordinates[2]);
  p_count++;
  if (!populated_header)
  {
    // update number of point records
    header.number_of_point_records++;
    // create return histogram
    if (point.return_number >= 1 && point.return_number <= 5) header.number_of_points_by_return[point.return_number-1]++;
    // update bounding box
    if (point.coordinates[0] < header.min_x) header.min_x = point.coordinates[0];
    else if (point.coordinates[0] > header.max_x) header.max_x = point.coordinates[0];
    if (point.coordinates[1] < header.min_y) header.min_y = point.coordinates[1];
    else if (point.coordinates[1] > header.max_y) header.max_y = point.coordinates[1];
    if (point.coordinates[2] < header.min_z) header.min_z = point.coordinates[2];
    else if (point.coordinates[2] > header.max_z) header.max_z = point.coordinates[2];
  }
  return TRUE;
}

void LASreaderASCII::close()
{
  if (file)
  {
    if (piped) while(fgets(line, 512, file));
    fclose(file);
    file = 0;
  }
}

BOOL LASreaderASCII::reopen(const char* file_name)
{
  int i;

  if (file_name == 0)
  {
    fprintf(stderr,"ERROR: fine name pointer is zero\n");
    return FALSE;
  }

  file = fopen_compressed(file_name, "r", &piped);
  if (file == 0)
  {
    fprintf(stderr, "ERROR: cannot reopen file '%s'\n", file_name);
    return FALSE;
  }

  // skip lines if we have to

  for (i = 0; i < skip_lines; i++) fgets(line, 512, file);

  // read the first line with full parse_string

  i = 0;
  while (fgets(line, 512, file))
  {
    if (parse(parse_string))
    {
      // mark that we found the first point
      i = 1;
      break;
    }
    else
    {
      line[strlen(line)-1] = '\0';
      fprintf(stderr, "WARNING: cannot parse '%s' with '%s'. skipping ...\n", line, parse_string);
    }
  }

  // did we manage to parse a line

  if (i != 1)
  {
    fprintf(stderr, "ERROR: could not parse any lines with '%s'\n", parse_string);
    fclose(file);
    file = 0;
    return FALSE;
  }

  p_count = 0;

  return TRUE;
}

void LASreaderASCII::clean()
{
  if (file)
  {
    fclose(file);
    file = 0;
  }
  if (parse_string)
  {
    free(parse_string);
    parse_string = 0;
  }
  skip_lines = 0;
  populated_header = FALSE;
}

LASreaderASCII::LASreaderASCII()
{
  file = 0;
  piped = false;
  parse_string = 0;
  scale_factor = 0;
  offset = 0;
  translate_intensity = 0.0f;
  scale_intensity = 1.0f;
  translate_scan_angle = 0.0f;
  scale_scan_angle = 1.0f;
  clean();
}

LASreaderASCII::~LASreaderASCII()
{
  clean();
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
}

BOOL LASreaderASCII::parse(const char* parse_string)
{
  I32 temp_i;
  F32 temp_f;
  const char* p = parse_string;
  const char* l = line;

  while (p[0])
  {
    if (p[0] == 'x') // we expect the x coordinate
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      if (sscanf(l, "%lf", &(point.coordinates[0])) != 1) return FALSE;
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else if (p[0] == 'y') // we expect the y coordinate
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      if (sscanf(l, "%lf", &(point.coordinates[1])) != 1) return FALSE;
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else if (p[0] == 'z') // we expect the x coordinate
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      if (sscanf(l, "%lf", &(point.coordinates[2])) != 1) return FALSE;
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else if (p[0] == 't') // we expect the gps time
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      if (sscanf(l, "%lf", &(point.gps_time)) != 1) return FALSE;
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else if (p[0] == 'R') // we expect the red channel of the RGB field
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
      point.rgb[0] = (short)temp_i;
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else if (p[0] == 'G') // we expect the green channel of the RGB field
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
      point.rgb[1] = (short)temp_i;
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else if (p[0] == 'B') // we expect the blue channel of the RGB field
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
      point.rgb[2] = (short)temp_i;
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else if (p[0] == 's') // we expect a string or a number that we don't care about
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else if (p[0] == 'i') // we expect the intensity
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      if (sscanf(l, "%f", &temp_f) != 1) return FALSE;
      if (translate_intensity != 0.0f) temp_f = temp_f+translate_intensity;
      if (scale_intensity != 1.0f) temp_f = temp_f*scale_intensity;
      if (temp_f < 0.0f || temp_f >= 65535.5f) fprintf(stderr, "WARNING: intensity %g is out of range of unsigned short\n", temp_f);
      point.intensity = (unsigned short)(temp_f+0.5f);
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else if (p[0] == 'a') // we expect the scan angle
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      if (sscanf(l, "%f", &temp_f) != 1) return FALSE;
      if (translate_scan_angle != 0.0f) temp_f = temp_f+translate_scan_angle;
      if (scale_scan_angle != 1.0f) temp_f = temp_f*scale_scan_angle;
      if (temp_f < -128.0f || temp_f > 127.0f) fprintf(stderr, "WARNING: scan angle %g is out of range of char\n", temp_f);
      point.scan_angle_rank = (char)temp_f;
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else if (p[0] == 'n') // we expect the number of returns of given pulse
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
      if (temp_i < 0 || temp_i > 7) fprintf(stderr, "WARNING: return number %d is out of range of three bits\n", temp_i);
      point.number_of_returns_of_given_pulse = temp_i & 7;
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else if (p[0] == 'r') // we expect the number of the return
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
      if (temp_i < 0 || temp_i > 7) fprintf(stderr, "WARNING: return number %d is out of range of three bits\n", temp_i);
      point.return_number = temp_i & 7;
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else if (p[0] == 'c') // we expect the classification
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
      if (temp_i < 0 || temp_i > 255) fprintf(stderr, "WARNING: classification %d is out of range of unsigned char\n", temp_i);
      point.classification = (unsigned char)temp_i;
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else if (p[0] == 'u') // we expect the user data
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
      if (temp_i < 0 || temp_i > 255) fprintf(stderr, "WARNING: user data %d is out of range of unsigned char\n", temp_i);
      point.user_data = temp_i & 255;
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else if (p[0] == 'p') // we expect the point source ID
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
      if (temp_i < 0 || temp_i > 65535) fprintf(stderr, "WARNING: point source ID %d is out of range of unsigned short\n", temp_i);
      point.point_source_ID = temp_i & 65535;
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else if (p[0] == 'e') // we expect the edge of flight line flag
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
      if (temp_i < 0 || temp_i > 1) fprintf(stderr, "WARNING: edge of flight line flag %d is out of range of boolean flag\n", temp_i);
      point.edge_of_flight_line = (temp_i ? 1 : 0);
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else if (p[0] == 'd') // we expect the direction of scan flag
    {
      while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; // first skip white spaces
      if (l[0] == 0) return FALSE;
      if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
      if (temp_i < 0 || temp_i > 1) fprintf(stderr, "WARNING: direction of scan flag %d is out of range of boolean flag\n", temp_i);
      point.scan_direction_flag = (temp_i ? 1 : 0);
      while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; // then advance to next white space
    }
    else
    {
      fprintf(stderr, "ERROR: unknown symbol '%c' in parse string\n", p[0]);
    }
    p++;
  }
  return TRUE;
}

BOOL LASreaderASCII::check_parse_string(const char* parse_string)
{
  const char* p = parse_string;
  while (p[0])
  {
    if ((p[0] != 'x') && // we expect the x coordinate
        (p[0] != 'y') && // we expect the y coordinate
        (p[0] != 'z') && // we expect the x coordinate
        (p[0] != 't') && // we expect the gps time
        (p[0] != 'R') && // we expect the red channel of the RGB field
        (p[0] != 'G') && // we expect the green channel of the RGB field
        (p[0] != 'B') && // we expect the blue channel of the RGB field
        (p[0] != 's') && // we expect a string or a number that we don't care about
        (p[0] != 'i') && // we expect the intensity
        (p[0] != 'a') && // we expect the scan angle
        (p[0] != 'n') && // we expect the number of returns of given pulse
        (p[0] != 'r') && // we expect the number of the return
        (p[0] != 'c') && // we expect the classification
        (p[0] != 'u') && // we expect the user data
        (p[0] != 'p') && // we expect the point source ID
        (p[0] != 'e') && // we expect the edge of flight line flag
        (p[0] != 'd'))   // we expect the direction of scan flag
    {
      fprintf(stderr, "ERROR: unknown symbol '%c' in parse string. valid are\n", p[0]);
      fprintf(stderr, "       'x' : the x coordinate\n");
      fprintf(stderr, "       'y' : the y coordinate\n");
      fprintf(stderr, "       'z' : the x coordinate\n");
      fprintf(stderr, "       't' : the gps time\n");
      fprintf(stderr, "       'R' : the red channel of the RGB field\n");
      fprintf(stderr, "       'G' : the green channel of the RGB field\n");
      fprintf(stderr, "       'B' : the blue channel of the RGB field\n");
      fprintf(stderr, "       's' : a string or a number that we don't care about\n");
      fprintf(stderr, "       'i' : the intensity\n");
      fprintf(stderr, "       'a' : the scan angle\n");
      fprintf(stderr, "       'n' : the number of returns of that given pulse\n");
      fprintf(stderr, "       'r' : the number of the return\n");
      fprintf(stderr, "       'c' : the classification\n");
      fprintf(stderr, "       'u' : the user data\n");
      fprintf(stderr, "       'p' : the point source ID\n");
      fprintf(stderr, "       'e' : the edge of flight line flag\n");
      fprintf(stderr, "       'd' : the direction of scan flag\n");
      return FALSE;
    }
    p++;
  }
  return TRUE;
}

void LASreaderASCII::populate_scale_and_offset()
{
  // if not specified in the command line, set a reasonable scale_factor
  if (scale_factor)
  {
    header.x_scale_factor = scale_factor[0];
    header.y_scale_factor = scale_factor[1];
    header.z_scale_factor = scale_factor[2];
  }
  else
  {
    if (-360 < header.min_x  && -360 < header.min_y && header.max_x < 360 && header.max_y < 360) // do we have longitude / latitude coordinates
    {
      header.x_scale_factor = 1e-7;
      header.y_scale_factor = 1e-7;
    }
    else // then we assume utm or mercator / lambertian projections
    {
      header.x_scale_factor = 0.01;
      header.y_scale_factor = 0.01;
    }
    header.z_scale_factor = 0.01;
  }

  // if not specified in the command line, set a reasonable offset
  if (offset)
  {
    header.x_offset = offset[0];
    header.y_offset = offset[1];
    header.z_offset = offset[2];
  }
  else
  {
    if (-360 < header.min_x  && -360 < header.min_y && header.max_x < 360 && header.max_y < 360) // do we have longitude / latitude coordinates
    {
      header.x_offset = 0;
      header.y_offset = 0;
      header.z_offset = 0;
    }
    else // then we assume utm or mercator / lambertian projections
    {
      header.x_offset = ((I32)((header.min_x + header.max_x)/200000))*100000;
      header.y_offset = ((I32)((header.min_y + header.max_y)/200000))*100000;
      header.z_offset = ((I32)((header.min_z + header.max_z)/200000))*100000;
    }
  }
}

void LASreaderASCII::populate_bounding_box()
{
  // compute quantized and then unquantized bounding box

  F64 dequant_min_x = header.get_x(header.get_x(header.min_x));
  F64 dequant_max_x = header.get_x(header.get_x(header.max_x));
  F64 dequant_min_y = header.get_y(header.get_y(header.min_y));
  F64 dequant_max_y = header.get_y(header.get_y(header.max_y));
  F64 dequant_min_z = header.get_z(header.get_z(header.min_z));
  F64 dequant_max_z = header.get_z(header.get_z(header.max_z));

  // make sure there is not sign flip

  if ((header.min_x > 0) != (dequant_min_x > 0))
  {
    fprintf(stderr, "WARNING: quantization sign flip for min_x from %g to %g.\n", header.min_x, dequant_min_x);
    fprintf(stderr, "         set scale factor for x coarser than %g with '-scale'\n", header.x_scale_factor);
  }
  else
  {
    header.min_x = dequant_min_x;
  }
  if ((header.max_x > 0) != (dequant_max_x > 0))
  {
    fprintf(stderr, "WARNING: quantization sign flip for max_x from %g to %g.\n", header.max_x, dequant_max_x);
    fprintf(stderr, "         set scale factor for x coarser than %g with '-scale'\n", header.x_scale_factor);
  }
  else
  {
    header.max_x = dequant_max_x;
  }
  if ((header.min_y > 0) != (dequant_min_y > 0))
  {
    fprintf(stderr, "WARNING: quantization sign flip for min_y from %g to %g.\n", header.min_y, dequant_min_y);
    fprintf(stderr, "         set scale factor for y coarser than %g with '-scale'\n", header.y_scale_factor);
  }
  else
  {
    header.min_y = dequant_min_y;
  }
  if ((header.max_y > 0) != (dequant_max_y > 0))
  {
    fprintf(stderr, "WARNING: quantization sign flip for max_y from %g to %g.\n", header.max_y, dequant_max_y);
    fprintf(stderr, "         set scale factor for y coarser than %g with '-scale'\n", header.y_scale_factor);
  }
  else
  {
    header.max_y = dequant_max_y;
  }
  if ((header.min_z > 0) != (dequant_min_z > 0))
  {
    fprintf(stderr, "WARNING: quantization sign flip for min_z from %g to %g.\n", header.min_z, dequant_min_z);
    fprintf(stderr, "         set scale factor for z coarser than %g with '-scale'\n", header.z_scale_factor);
  }
  else
  {
    header.min_z = dequant_min_z;
  }
  if ((header.max_z > 0) != (dequant_max_z > 0))
  {
    fprintf(stderr, "WARNING: quantization sign flip for max_z from %g to %g.\n", header.max_z, dequant_max_z);
    fprintf(stderr, "         set scale factor for z coarser than %g with '-scale'\n", header.z_scale_factor);
  }
  else
  {
    header.max_z = dequant_max_z;
  }
}

LASreaderASCIIRescale::LASreaderASCIIRescale(F64 x_scale_factor, F64 y_scale_factor, F64 z_scale_factor) : LASreaderASCII()
{
  scale_factor[0] = x_scale_factor;
  scale_factor[1] = y_scale_factor;
  scale_factor[2] = z_scale_factor;
}

BOOL LASreaderASCIIRescale::open(const char* file_name, const char* parse_string, I32 skip_lines, BOOL populate_header)
{
  if (!LASreaderASCII::open(file_name, parse_string, skip_lines, populate_header)) return FALSE;
  // do we need to change anything
  if (header.x_scale_factor != scale_factor[0])
  {
    header.x_scale_factor = scale_factor[0];
  }
  if (header.y_scale_factor != scale_factor[1])
  {
    header.y_scale_factor = scale_factor[1];
  }
  if (header.z_scale_factor != scale_factor[2])
  {
    header.z_scale_factor = scale_factor[2];
  }
  return TRUE;
}

LASreaderASCIIReoffset::LASreaderASCIIReoffset(F64 x_offset, F64 y_offset, F64 z_offset) : LASreaderASCII()
{
  this->offset[0] = x_offset;
  this->offset[1] = y_offset;
  this->offset[2] = z_offset;
}

BOOL LASreaderASCIIReoffset::open(const char* file_name, const char* parse_string, I32 skip_lines, BOOL populate_header)
{
  if (!LASreaderASCII::open(file_name, parse_string, skip_lines, populate_header)) return FALSE;
  // do we need to change anything
  if (header.x_offset != offset[0])
  {
    header.x_offset = offset[0];
  }
  if (header.y_offset != offset[1])
  {
    header.y_offset = offset[1];
  }
  if (header.z_offset != offset[2])
  {
    header.z_offset = offset[2];
  }
  return TRUE;
}

LASreaderASCIIRescaleReoffset::LASreaderASCIIRescaleReoffset(F64 x_scale_factor, F64 y_scale_factor, F64 z_scale_factor, F64 x_offset, F64 y_offset, F64 z_offset) : LASreaderASCIIRescale(x_scale_factor, y_scale_factor, z_scale_factor), LASreaderASCIIReoffset(x_offset, y_offset, z_offset)
{
}

BOOL LASreaderASCIIRescaleReoffset::open(const char* file_name, const char* parse_string, I32 skip_lines, BOOL populate_header)
{
  if (!LASreaderASCII::open(file_name, parse_string, skip_lines, populate_header)) return FALSE;
  // do we need to change anything
  if (header.x_scale_factor != scale_factor[0])
  {
    header.x_scale_factor = scale_factor[0];
  }
  if (header.y_scale_factor != scale_factor[1])
  {
    header.y_scale_factor = scale_factor[1];
  }
  if (header.z_scale_factor != scale_factor[2])
  {
    header.z_scale_factor = scale_factor[2];
  }
  if (header.x_offset != offset[0])
  {
    header.x_offset = offset[0];
  }
  if (header.y_offset != offset[1])
  {
    header.y_offset = offset[1];
  }
  if (header.z_offset != offset[2])
  {
    header.z_offset = offset[2];
  }
  return TRUE;
}
