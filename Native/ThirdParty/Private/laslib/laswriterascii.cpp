/*
===============================================================================

  FILE:  laswriterascii.cpp
  
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
#include "laswriterascii.hpp"

#include <stdlib.h>
#include <string.h>

BOOL LASwriterASCII::refile(FILE* file)
{
  this->file = file;
  return TRUE;
}

BOOL LASwriterASCII::open(const char* file_name, const LASheader* header, const char* parse_string, const char* separator)
{
  if (file_name == 0)
  {
    fprintf(stderr,"ERROR: file name pointer is zero\n");
    return FALSE;
  }

  file = fopen(file_name, "w");

  if (file == 0)
  {
    fprintf(stderr, "ERROR: cannot open file '%s'\n", file_name);
    return FALSE;
  }

  close_file = TRUE;

  return open(file, header, parse_string, separator);
}

BOOL LASwriterASCII::open(FILE* file, const LASheader* header, const char* parse_string, const char* separator)
{
  if (file == 0)
  {
    fprintf(stderr,"ERROR: file pointer is zero\n");
    return FALSE;
  }

  this->file = file;
  this->header = header;

  if (parse_string)
  {
    this->parse_string = strdup(parse_string);
  }
  else
  {
    if (header->point_data_format == 1 || header->point_data_format == 4)
    {
      this->parse_string = strdup("xyzt");
    }
    else if (header->point_data_format == 2)
    {
      this->parse_string = strdup("xyzRGB");
    }
    else if (header->point_data_format == 3 || header->point_data_format == 5)
    {
      this->parse_string = strdup("xyztRGB");
    }
    else
    {
      this->parse_string = strdup("xyz");
    }
  }

  if (!check_parse_string(this->parse_string))
  {
    return FALSE;
  }

  if (separator)
  {
    if (strcmp(separator, "comma") == 0)
    {
      separator_sign = ',';
    }
    else if (strcmp(separator, "tab") == 0)
    {
      separator_sign = '\t';
    }
    else if (strcmp(separator, "dot") == 0 || strcmp(separator, "period") == 0)
    {
      separator_sign = '.';
    }
    else if (strcmp(separator, "colon") == 0)
    {
      separator_sign = ':';
    }
    else if (strcmp(separator, "semicolon") == 0)
    {
      separator_sign = ';';
    }
    else if (strcmp(separator, "hyphen") == 0 || strcmp(separator, "minus") == 0)
    {
      separator_sign = '-';
    }
    else if (strcmp(separator, "space") == 0)
    {
      separator_sign = ' ';
    }
    else
    {
      fprintf(stderr, "ERROR: unknown seperator '%s'\n", separator);
      return FALSE;
    }
  }

  return TRUE;
}

static void lidardouble2string(char* string, double value)
{
  int len;
  len = sprintf(string, "%.15f", value) - 1;
  while (string[len] == '0') len--;
  if (string[len] != '.') len++;
  string[len] = '\0';
}

static void lidardouble2string(char* string, double value, double precision)
{
  if (precision == 0.1)
    sprintf(string, "%.1f", value);
  else if (precision == 0.01)
    sprintf(string, "%.2f", value);
  else if (precision == 0.001)
    sprintf(string, "%.3f", value);
  else if (precision == 0.0001)
    sprintf(string, "%.4f", value);
  else if (precision == 0.00001)
    sprintf(string, "%.5f", value);
  else if (precision == 0.000001)
    sprintf(string, "%.6f", value);
  else if (precision == 0.0000001)
    sprintf(string, "%.7f", value);
  else if (precision == 0.00000001)
    sprintf(string, "%.8f", value);
  else if (precision == 0.000000001)
    sprintf(string, "%.9f", value);
  else
    lidardouble2string(string, value);
}

BOOL LASwriterASCII::write_point(const LASpoint* point)
{
  p_count++;
  int i = 0;
  while (true)
  {
    switch (parse_string[i])
    {
    case 'x': // the x coordinate
      lidardouble2string(printstring, header->get_x(point->x), header->x_scale_factor); fprintf(file, "%s", printstring);
      break;
    case 'y': // the y coordinate
      lidardouble2string(printstring, header->get_y(point->y), header->y_scale_factor); fprintf(file, "%s", printstring);
      break;
    case 'z': // the z coordinate
      lidardouble2string(printstring, header->get_z(point->z), header->z_scale_factor); fprintf(file, "%s", printstring);
      break;
    case 'X': // the unscaled raw integer X coordinate
      fprintf(file, "%d", point->x);
      break;
    case 'Y': // the unscaled raw integer Y coordinate
      fprintf(file, "%d", point->y);
      break;
    case 'Z': // the unscaled raw integer Z coordinate
      fprintf(file, "%d", point->z);
      break;
    case 't': // the gps-time
      lidardouble2string(printstring,point->gps_time); fprintf(file, "%s", printstring);
      break;
    case 'i': // the intensity
      fprintf(file, "%d", point->intensity);
      break;
    case 'a': // the scan angle
      fprintf(file, "%d", point->scan_angle_rank);
      break;
    case 'r': // the number of the return
      fprintf(file, "%d", point->return_number);
      break;
    case 'c': // the classification
      fprintf(file, "%d", point->classification);
      break;
    case 'u': // the user data
      fprintf(file, "%d", point->user_data);
      break;
    case 'n': // the number of returns of given pulse
      fprintf(file, "%d", point->number_of_returns_of_given_pulse);
      break;
    case 'p': // the point source ID
      fprintf(file, "%d", point->point_source_ID);
      break;
    case 'e': // the edge of flight line flag
      fprintf(file, "%d", point->edge_of_flight_line);
      break;
    case 'd': // the direction of scan flag
      fprintf(file, "%d", point->scan_direction_flag);
      break;
    case 'R': // the red channel of the RGB field
      fprintf(file, "%d", point->rgb[0]);
      break;
    case 'G': // the green channel of the RGB field
      fprintf(file, "%d", point->rgb[1]);
      break;
    case 'B': // the blue channel of the RGB field
      fprintf(file, "%d", point->rgb[2]);
      break;
    case 'M': // the index of the point
#ifdef _WIN32
      fprintf(file, "%I64d", p_count);
#else
      fprintf(file, "%lld", p_count);
#endif
      break;
    case 'w': // the wavepacket index
      fprintf(file, "%d", point->wavepacket.getIndex());
      break;
    case 'W': // all wavepacket attributes
      fprintf(file, "%d%c%d%c%d%c%g%c%g%c%g%c%g", point->wavepacket.getIndex(), separator_sign, (U32)point->wavepacket.getOffset(), separator_sign, point->wavepacket.getSize(), separator_sign, point->wavepacket.getLocation(), separator_sign, point->wavepacket.getXt(), separator_sign, point->wavepacket.getYt(), separator_sign, point->wavepacket.getZt());
      break;
    }
    i++;
    if (parse_string[i])
    {
      fprintf(file, "%c", separator_sign);
    }
    else
    {
      fprintf(file, "\012");
      break;
    }
  }
  return TRUE;
}

BOOL LASwriterASCII::update_header(const LASheader* header, BOOL use_inventory)
{
  return TRUE;
}

U32 LASwriterASCII::close(BOOL update_header)
{
  U32 bytes = (U32)ftell(file);

  if (file)
  {
    if (close_file)
    {
      fclose(file);
      close_file = FALSE;
    }
    file = 0;
  }
  if (parse_string)
  {
    free(parse_string);
    parse_string = 0;
  }

  npoints = p_count;
  p_count = 0;

  return bytes;
}

LASwriterASCII::LASwriterASCII()
{
  close_file = FALSE;
  file = 0;
  parse_string = 0;
  separator_sign = ' ';
}

LASwriterASCII::~LASwriterASCII()
{
  if (file) close();
}

BOOL LASwriterASCII::check_parse_string(const char* parse_string)
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
