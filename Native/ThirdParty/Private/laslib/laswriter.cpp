/*
===============================================================================

  FILE:  laswriter.cpp
  
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
#include "laswriter.hpp"

#include "laswritersingle.hpp"
#include "laswriterascii.hpp"

#include <stdlib.h>
#include <string.h>

BOOL LASwriteOpener::piped() const
{
  return ((file_name == 0) && use_stdout);
}

LASwriter* LASwriteOpener::open(LASheader* header)
{
  if (use_nil)
  {
    LASwriterSingle* laswritersingle = new LASwriterSingle();
    if (!laswritersingle->open(header, (use_compression ? (use_chunking ?  LASZIP_COMPRESSOR_CHUNKED : LASZIP_COMPRESSOR_NOT_CHUNKED) : LASZIP_COMPRESSOR_NONE), (use_v2 ? 2 : 1), chunk_size))
    {
      fprintf(stderr,"ERROR: cannot open laswritersingle to NULL\n");
      delete laswritersingle;
      return 0;
    }
    return laswritersingle;
  }
  else if (file_name)
  {
    if (use_txt)
    {
      LASwriterASCII* laswriterascii = new LASwriterASCII();
      if (!laswriterascii->open(file_name, header, parse_string, separator))
      {
        fprintf(stderr,"ERROR: cannot open laswriterascii with file name '%s'\n", file_name);
        delete laswriterascii;
        return 0;
      }
      return laswriterascii;
    }
    else
    {
      LASwriterSingle* laswritersingle = new LASwriterSingle();
      if (!laswritersingle->open(file_name, header, (use_compression ? (use_chunking ? LASZIP_COMPRESSOR_CHUNKED : LASZIP_COMPRESSOR_NOT_CHUNKED) : LASZIP_COMPRESSOR_NONE), (use_v2 ? 2 : 1), chunk_size))
      {
        fprintf(stderr,"ERROR: cannot open laswritersingle with file name '%s'\n", file_name);
        delete laswritersingle;
        return 0;
      }
      return laswritersingle;
    }
  }
  else if (use_stdout)
  {
    if (use_txt)
    {
      LASwriterASCII* laswriterascii = new LASwriterASCII();
      if (!laswriterascii->open(stdout, header, parse_string, separator))
      {
        fprintf(stderr,"ERROR: cannot open laswriterascii to stdout\n");
        delete laswriterascii;
        return 0;
      }
      return laswriterascii;
    }
    else
    {
      LASwriterSingle* laswritersingle = new LASwriterSingle();
      if (!laswritersingle->open(stdout, header, (use_compression ? (use_chunking ? LASZIP_COMPRESSOR_CHUNKED : LASZIP_COMPRESSOR_NOT_CHUNKED) : LASZIP_COMPRESSOR_NONE), (use_v2 ? 2 : 1), chunk_size))
      {
        fprintf(stderr,"ERROR: cannot open laswritersingle to stdout\n");
        delete laswritersingle;
        return 0;
      }
      return laswritersingle;
    }
  }
  else
  {
    fprintf(stderr,"ERROR: no laswriter output specified\n");
    return 0;
  }
}

void LASwriteOpener::usage() const
{
  fprintf(stderr,"Supported LAS Outputs\n");
  fprintf(stderr,"  -o lidar.las\n");
  fprintf(stderr,"  -o lidar.laz\n");
  fprintf(stderr,"  -o xyzta.txt -oparse xyzta (on-the-fly to ASCII)\n");
  fprintf(stderr,"  -olas -olaz -otxt (specify format)\n");
  fprintf(stderr,"  -stdout (pipe to stdout)\n");
  fprintf(stderr,"  -nil    (pipe to NULL)\n");
}

BOOL LASwriteOpener::parse(int argc, char* argv[])
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
      usage();
      return TRUE;
    }
    else if (strcmp(argv[i],"-o") == 0)
    {
      if ((i+1) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs 1 argument: file_name\n", argv[i]);
        return FALSE;
      }
      set_file_name(argv[i+1]);
      *argv[i]='\0'; *argv[i+1]='\0'; i+=1;
    }
    else if (strcmp(argv[i],"-olas") == 0)
    {
      use_compression = FALSE;
      use_txt = FALSE;
      *argv[i]='\0';
    }
    else if (strcmp(argv[i],"-olaz") == 0)
    {
      use_compression = TRUE;
      use_txt = FALSE;
      *argv[i]='\0';
    }
    else if (strcmp(argv[i],"-otxt") == 0)
    {
      use_txt = TRUE;
      *argv[i]='\0';
    }
    else if (strcmp(argv[i],"-stdout") == 0)
    {
      use_stdout = TRUE;
      use_nil = FALSE;
      *argv[i]='\0';
    }
    else if (strcmp(argv[i],"-nil") == 0)
    {
      use_nil = TRUE;
      use_stdout = FALSE;
      *argv[i]='\0';
    }
    else if (strcmp(argv[i],"-v2") == 0)
    {
      use_v2 = TRUE;
      use_chunking = TRUE;
      *argv[i]='\0';
    }
    else if (strcmp(argv[i],"-v1") == 0)
    {
      use_v2 = FALSE;
      use_chunking = FALSE;
      *argv[i]='\0';
    }
    else if (strcmp(argv[i],"-no_chunk") == 0)
    {
      use_chunking = FALSE;
      *argv[i]='\0';
    }
    else if (strcmp(argv[i],"-chunk") == 0)
    {
      use_chunking = TRUE;
      *argv[i]='\0';
    }
    else if (strcmp(argv[i],"-chunk_size") == 0)
    {
      if ((i+1) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs 1 argument: number_of_points\n", argv[i]);
        return FALSE;
      }
      use_chunking = TRUE;
      chunk_size = atoi(argv[i+1]);
      *argv[i]='\0'; *argv[i+1]='\0'; i+=1;
    }
    else if (strcmp(argv[i],"-oparse") == 0)
    {
      if ((i+1) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs 1 argument: string\n", argv[i]);
        return FALSE;
      }
      set_parse_string(argv[i+1]);
      *argv[i]='\0'; *argv[i+1]='\0'; i+=1;
    }
    else if (strcmp(argv[i],"-sep") == 0)
    {
      if ((i+1) >= argc)
      {
        fprintf(stderr,"ERROR: '%s' needs 1 argument: separator\n", argv[i]);
        return FALSE;
      }
      set_separator(argv[i+1]);
      *argv[i]='\0'; *argv[i+1]='\0'; i+=1;
    }
  }
  return TRUE;
}

void LASwriteOpener::set_file_name(const char* file_name)
{
  if (this->file_name) free(this->file_name);
  if (file_name)
  {
    this->file_name = strdup(file_name);
    if (strstr(file_name, ".laz") || strstr(file_name, ".LAZ"))
    {
      use_compression = TRUE;
      use_txt = FALSE;
    }
    else if (strstr(file_name, ".las") || strstr(file_name, ".LAS"))
    {
      use_compression = FALSE;
      use_txt = FALSE;
    }
    else // assume ascii output
    {
      use_txt = TRUE;
    }
  }
  else
  {
    this->file_name = 0;
  }
}

void LASwriteOpener::make_file_name(const char* file_name, I32 file_number)
{
  int len;
  if (file_number > -1)
  {
    if (file_name)
    {
      if (this->file_name) free(this->file_name);
      len = strlen(file_name);
      this->file_name = (char*)malloc(len+10);
      strcpy(this->file_name, file_name);
    }
    else
    {
      len = strlen(this->file_name);
    }
    while (len > 0 && this->file_name[len] != '.') len--;
    len++;
    int num = len - 2;
    while (num > 0 && this->file_name[num] >= '0' && this->file_name[num] <= '9')
    {
      this->file_name[num] = '0' + (file_number%10);
      file_number = file_number/10;
      num--;
    }
  }
  else
  {
    if (this->file_name) free(this->file_name);
    if (file_name)
    {
      len = strlen(file_name);
      this->file_name = (char*)malloc(len+10);
      strcpy(this->file_name, file_name);
      while (len > 0 && this->file_name[len] != '.') len--;
      this->file_name[len] = '_';
      this->file_name[len+1] = '1';
      this->file_name[len+2] = '.';
      len += 3;
    }
    else
    {
      len = 7;
      this->file_name = strdup("output.xxx");
    }
  }
  if (use_txt)
  {
    this->file_name[len] = 't';
    this->file_name[len+1] = 'x';
    this->file_name[len+2] = 't';
  }
  else if (use_compression)
  {
    this->file_name[len] = 'l';
    this->file_name[len+1] = 'a';
    this->file_name[len+2] = 'z';
  }
  else
  {
    this->file_name[len] = 'l';
    this->file_name[len+1] = 'a';
    this->file_name[len+2] = 's';
  }
  this->file_name[len+3] = '\0';
}

const char* LASwriteOpener::get_file_name() const
{
  return file_name;
}

void LASwriteOpener::set_parse_string(const char* parse_string)
{
  if (this->parse_string) free(this->parse_string);
  this->parse_string = strdup(parse_string);
}

void LASwriteOpener::set_separator(const char* separator)
{
  if (this->separator) free(this->separator);
  this->separator = strdup(separator);
}

BOOL LASwriteOpener::active() const
{
  return (file_name != 0 || use_stdout || use_nil);
}

LASwriteOpener::LASwriteOpener()
{
  file_name = 0;
  parse_string = 0;
  separator = 0;
  use_stdout = FALSE;
  use_compression = FALSE;
  use_txt = FALSE;
  use_nil = FALSE;
  use_v2 = TRUE;
  use_chunking = TRUE;
  chunk_size = LASZIP_CHUNK_SIZE_DEFAULT;
}

LASwriteOpener::~LASwriteOpener()
{
  if (file_name) free(file_name);
  if (parse_string) free(parse_string);
  if (separator) free(separator);
}
