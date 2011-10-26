/*
===============================================================================

  FILE:  lasreadersingle.cpp
  
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
#include "lasreadersingle.hpp"

#include "bytestreamin.hpp"
#include "bytestreamin_file.hpp"
#include "bytestreamin_istream.hpp"
#include "lasreadpoint.hpp"

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

#include <stdlib.h>
#include <string.h>

BOOL LASreaderSingle::open(const char* file_name)
{
  if (file_name == 0)
  {
    fprintf(stderr,"ERROR: fine name pointer is zero\n");
    return FALSE;
  }

  file = fopen(file_name, "rb");
  if (file == 0)
  {
    fprintf(stderr, "ERROR: cannot open file '%s'\n", file_name);
    return FALSE;
  }

  // create input
  ByteStreamIn* in;
  if (IS_LITTLE_ENDIAN())
    in = new ByteStreamInFileLE(file);
  else
    in = new ByteStreamInFileBE(file);

  return open(in);
}

BOOL LASreaderSingle::open(FILE* file)
{
  if (file == 0)
  {
    fprintf(stderr,"ERROR: file pointer is zero\n");
    return FALSE;
  }

#ifdef _WIN32
  if (file == stdin)
  {
    if(_setmode( _fileno( stdin ), _O_BINARY ) == -1 )
    {
      fprintf(stderr, "ERROR: cannot set stdin to binary (untranslated) mode\n");
      return FALSE;
    }
  }
#endif

  // create input
  ByteStreamIn* in;
  if (IS_LITTLE_ENDIAN())
    in = new ByteStreamInFileLE(file);
  else
    in = new ByteStreamInFileBE(file);

  return open(in);
}

BOOL LASreaderSingle::open(istream& stream)
{
  // create input
  ByteStreamIn* in;
  if (IS_LITTLE_ENDIAN())
    in = new ByteStreamInIstreamLE(stream);
  else
    in = new ByteStreamInIstreamBE(stream);

  return open(in);
}

BOOL LASreaderSingle::open(ByteStreamIn* stream)
{
  U32 i,j;

  if (stream == 0)
  {
    fprintf(stderr,"ERROR: ByteStreamIn* pointer is zero\n");
    return FALSE;
  }

  this->stream = stream;

  // clean the header

  header.clean();

  // read the header variable after variable (to avoid alignment issues)

  if (!stream->getBytes((U8*)&(header.file_signature), 4))
  {
    fprintf(stderr,"ERROR: reading header.file_signature\n");
    return FALSE;
  }
  if (!stream->get16bitsLE((U8*)&(header.file_source_id)))
  {
    fprintf(stderr,"ERROR: reading header.file_source_id\n");
    return FALSE;
  }
  if (!stream->get16bitsLE((U8*)&(header.global_encoding)))
  {
    fprintf(stderr,"ERROR: reading header.global_encoding\n");
    return FALSE;
  }
  if (!stream->get32bitsLE((U8*)&(header.project_ID_GUID_data_1)))
  {
    fprintf(stderr,"ERROR: reading header.project_ID_GUID_data_1\n");
    return FALSE;
  }
  if (!stream->get16bitsLE((U8*)&(header.project_ID_GUID_data_2)))
  {
    fprintf(stderr,"ERROR: reading header.project_ID_GUID_data_2\n");
    return FALSE;
  }
  if (!stream->get16bitsLE((U8*)&(header.project_ID_GUID_data_3)))
  {
    fprintf(stderr,"ERROR: reading header.project_ID_GUID_data_3\n");
    return FALSE;
  }
  if (!stream->getBytes((U8*)&(header.project_ID_GUID_data_4), 8))
  {
    fprintf(stderr,"ERROR: reading header.project_ID_GUID_data_4\n");
    return FALSE;
  }
  if (!stream->getBytes((U8*)&(header.version_major), 1))
  {
    fprintf(stderr,"ERROR: reading header.version_major\n");
    return FALSE;
  }
  if (!stream->getBytes((U8*)&(header.version_minor), 1))
  {
    fprintf(stderr,"ERROR: reading header.version_minor\n");
    return FALSE;
  }
  if (!stream->getBytes((U8*)&(header.system_identifier), 32))
  {
    fprintf(stderr,"ERROR: reading header.system_identifier\n");
    return FALSE;
  }
  if (!stream->getBytes((U8*)&(header.generating_software), 32))
  {
    fprintf(stderr,"ERROR: reading header.generating_software\n");
    return FALSE;
  }
  if (!stream->get16bitsLE((U8*)&(header.file_creation_day)))
  {
    fprintf(stderr,"ERROR: reading header.file_creation_day\n");
    return FALSE;
  }
  if (!stream->get16bitsLE((U8*)&(header.file_creation_year)))
  {
    fprintf(stderr,"ERROR: reading header.file_creation_year\n");
    return FALSE;
  }
  if (!stream->get16bitsLE((U8*)&(header.header_size)))
  {
    fprintf(stderr,"ERROR: reading header.header_size\n");
    return FALSE;
  }
  if (!stream->get32bitsLE((U8*)&(header.offset_to_point_data)))
  {
    fprintf(stderr,"ERROR: reading header.offset_to_point_data\n");
    return FALSE;
  }
  if (!stream->get32bitsLE((U8*)&(header.number_of_variable_length_records)))
  {
    fprintf(stderr,"ERROR: reading header.number_of_variable_length_records\n");
    return FALSE;
  }
  if (!stream->getBytes((U8*)&(header.point_data_format), 1))
  {
    fprintf(stderr,"ERROR: reading header.point_data_format\n");
    return FALSE;
  }
  if (!stream->get16bitsLE((U8*)&(header.point_data_record_length)))
  {
    fprintf(stderr,"ERROR: reading header.point_data_record_length\n");
    return FALSE;
  }
  if (!stream->get32bitsLE((U8*)&(header.number_of_point_records)))
  {
    fprintf(stderr,"ERROR: reading header.number_of_point_records\n");
    return FALSE;
  }
  for (i = 0; i < 5; i++)
  {
    if (!stream->get32bitsLE((U8*)&(header.number_of_points_by_return[i])))
    {
      fprintf(stderr,"ERROR: reading header.number_of_points_by_return %d\n", i);
      return FALSE;
    }
  }
  if (!stream->get64bitsLE((U8*)&(header.x_scale_factor)))
  {
    fprintf(stderr,"ERROR: reading header.x_scale_factor\n");
    return FALSE;
  }
  if (!stream->get64bitsLE((U8*)&(header.y_scale_factor)))
  {
    fprintf(stderr,"ERROR: reading header.y_scale_factor\n");
    return FALSE;
  }
  if (!stream->get64bitsLE((U8*)&(header.z_scale_factor)))
  {
    fprintf(stderr,"ERROR: reading header.z_scale_factor\n");
    return FALSE;
  }
  if (!stream->get64bitsLE((U8*)&(header.x_offset)))
  {
    fprintf(stderr,"ERROR: reading header.x_offset\n");
    return FALSE;
  }
  if (!stream->get64bitsLE((U8*)&(header.y_offset)))
  {
    fprintf(stderr,"ERROR: reading header.y_offset\n");
    return FALSE;
  }
  if (!stream->get64bitsLE((U8*)&(header.z_offset)))
  {
    fprintf(stderr,"ERROR: reading header.z_offset\n");
    return FALSE;
  }
  if (!stream->get64bitsLE((U8*)&(header.max_x)))
  {
    fprintf(stderr,"ERROR: reading header.max_x\n");
    return FALSE;
  }
  if (!stream->get64bitsLE((U8*)&(header.min_x)))
  {
    fprintf(stderr,"ERROR: reading header.min_x\n");
    return FALSE;
  }
  if (!stream->get64bitsLE((U8*)&(header.max_y)))
  {
    fprintf(stderr,"ERROR: reading header.max_y\n");
    return FALSE;
  }
  if (!stream->get64bitsLE((U8*)&(header.min_y)))
  {
    fprintf(stderr,"ERROR: reading header.min_y\n");
    return FALSE;
  }
  if (!stream->get64bitsLE((U8*)&(header.max_z)))
  {
    fprintf(stderr,"ERROR: reading header.max_z\n");
    return FALSE;
  }
  if (!stream->get64bitsLE((U8*)&(header.min_z)))
  {
    fprintf(stderr,"ERROR: reading header.min_z\n");
    return FALSE;
  }

  // special handling for LAS 1.3
  if ((header.version_major == 1) && (header.version_minor == 3))
  {
    if (header.header_size < 235)
    {
      fprintf(stderr,"WARNING: for LAS 1.3 header_size should at least be 235 but it is only %d\n", header.header_size);
      header.user_data_in_header_size = header.header_size - 227;
    }
    else
    {
      if (!stream->get64bitsLE((U8*)&(header.start_of_waveform_data_packet_record)))
      {
        fprintf(stderr,"ERROR: reading header.start_of_waveform_data_packet_record\n");
        return FALSE;
      }
      header.user_data_in_header_size = header.header_size - 235;
    }
  }
  else
  {
    header.user_data_in_header_size = header.header_size - 227;
  }

  // load any number of user-defined bytes that might have been added to the header
  if (header.user_data_in_header_size)
  {
    header.user_data_in_header = new char[header.user_data_in_header_size];

    if (!stream->getBytes((U8*)header.user_data_in_header, header.user_data_in_header_size))
    {
      fprintf(stderr,"ERROR: reading %d bytes of data into header.user_data_in_header\n", header.user_data_in_header_size);
      return FALSE;
    }
  }

  // check header contents
  if (!header.check())
  {
    return FALSE;
  }

  // read the variable length records into the header

  U32 vlrs_size = 0;

  if (header.number_of_variable_length_records)
  {
    header.vlrs = (LASvlr*)malloc(sizeof(LASvlr)*header.number_of_variable_length_records);
  
    for (i = 0; i < header.number_of_variable_length_records; i++)
    {
      // make sure there are enough bytes left to read a variable length record before the point block starts

      if (((int)header.offset_to_point_data - vlrs_size - header.header_size) < 54)
      {
        fprintf(stderr,"WARNING: only %d bytes until point block after reading %d of %d vlrs. skipping remaining vlrs ...\n", (int)header.offset_to_point_data - vlrs_size - header.header_size, i, header.number_of_variable_length_records);
        header.number_of_variable_length_records = i;
        break;
      }

      // read variable length records variable after variable (to avoid alignment issues)

      if (!stream->get16bitsLE((U8*)&(header.vlrs[i].reserved)))
      {
        fprintf(stderr,"ERROR: reading header.vlrs[%d].reserved\n", i);
        return FALSE;
      }

      if (!stream->getBytes((U8*)header.vlrs[i].user_id, 16))
      {
        fprintf(stderr,"ERROR: reading header.vlrs[%d].user_id\n", i);
        return FALSE;
      }
      if (!stream->get16bitsLE((U8*)&(header.vlrs[i].record_id)))
      {
        fprintf(stderr,"ERROR: reading header.vlrs[%d].record_id\n", i);
        return FALSE;
      }
      if (!stream->get16bitsLE((U8*)&(header.vlrs[i].record_length_after_header)))
      {
        fprintf(stderr,"ERROR: reading header.vlrs[%d].record_length_after_header\n", i);
        return FALSE;
      }
      if (!stream->getBytes((U8*)header.vlrs[i].description, 32))
      {
        fprintf(stderr,"ERROR: reading header.vlrs[%d].description\n", i);
        return FALSE;
      }

      // keep track on the number of bytes we have read so far

      vlrs_size += 54;

      // check variable length record contents

      if (header.vlrs[i].reserved != 0xAABB)
      {
//        fprintf(stderr,"WARNING: wrong header.vlrs[%d].reserved: %d != 0xAABB\n", i, header.vlrs[i].reserved);
      }

      // make sure there are enough bytes left to read the data of the variable length record before the point block starts

      if (((int)header.offset_to_point_data - vlrs_size - header.header_size) < header.vlrs[i].record_length_after_header)
      {
        fprintf(stderr,"WARNING: only %d bytes until point block when trying to read %d bytes into header.vlrs[%d].data\n", (int)header.offset_to_point_data - vlrs_size - header.header_size, header.vlrs[i].record_length_after_header, i);
        header.vlrs[i].record_length_after_header = (int)header.offset_to_point_data - vlrs_size - header.header_size;
      }

      // load data following the header of the variable length record

      if (header.vlrs[i].record_length_after_header)
      {
        if (strcmp(header.vlrs[i].user_id, "laszip encoded") == 0)
        {
          header.laszip = new LASzip();

          // read this data following the header of the variable length record
          //     U16  compressor         2 bytes 
          //     U32  coder              2 bytes 
          //     U8   version_major      1 byte 
          //     U8   version_minor      1 byte
          //     U16  version_revision   2 bytes
          //     U32  options            4 bytes 
          //     I32  chunk_size         4 bytes
          //     I64  num_points         8 bytes
          //     I64  num_bytes          8 bytes
          //     U16  num_items          2 bytes
          //        U16 type                2 bytes * num_items
          //        U16 size                2 bytes * num_items
          //        U16 version             2 bytes * num_items
          // which totals 34+6*num_items

          if (!stream->get16bitsLE((U8*)&(header.laszip->compressor)))
          {
            fprintf(stderr,"ERROR: reading compressor %d\n", (I32)header.laszip->compressor);
            return FALSE;
          }
          if (!stream->get16bitsLE((U8*)&(header.laszip->coder)))
          {
            fprintf(stderr,"ERROR: reading coder %d\n", (I32)header.laszip->coder);
            return FALSE;
          }
          if (!stream->getBytes((U8*)&(header.laszip->version_major), 1))
          {
            fprintf(stderr,"ERROR: reading version_major %d\n", header.laszip->version_major);
            return FALSE;
          }
          if (!stream->getBytes((U8*)&(header.laszip->version_minor), 1))
          {
            fprintf(stderr,"ERROR: reading version_minor %d\n", header.laszip->version_minor);
            return FALSE;
          }
          if (!stream->get16bitsLE((U8*)&(header.laszip->version_revision)))
          {
            fprintf(stderr,"ERROR: reading version_revision %d\n", header.laszip->version_revision);
            return FALSE;
          }
          if (!stream->get32bitsLE((U8*)&(header.laszip->options)))
          {
            fprintf(stderr,"ERROR: reading options %d\n", (I32)header.laszip->options);
            return FALSE;
          }
          if (!stream->get32bitsLE((U8*)&(header.laszip->chunk_size)))
          {
            fprintf(stderr,"ERROR: reading chunk_size %d\n", header.laszip->chunk_size);
            return FALSE;
          }
          if (!stream->get64bitsLE((U8*)&(header.laszip->num_points)))
          {
            fprintf(stderr,"ERROR: reading num_points %d\n", (I32)header.laszip->num_points);
            return FALSE;
          }
          if (!stream->get64bitsLE((U8*)&(header.laszip->num_bytes)))
          {
            fprintf(stderr,"ERROR: reading num_bytes %d\n", (I32)header.laszip->num_bytes);
            return FALSE;
          }
          if (!stream->get16bitsLE((U8*)&(header.laszip->num_items)))
          {
            fprintf(stderr,"ERROR: reading num_items %d\n", header.laszip->num_items);
            return FALSE;
          }
          header.laszip->items = new LASitem[header.laszip->num_items];
          for (j = 0; j < header.laszip->num_items; j++)
          {
            U16 type, size, version;
            if (!stream->get16bitsLE((U8*)&type))
            {
              fprintf(stderr,"ERROR: reading type %d of item %d\n", type, j);
              return FALSE;
            }
            if (!stream->get16bitsLE((U8*)&size))
            {
              fprintf(stderr,"ERROR: reading size %d of item %d\n", size, j);
              return FALSE;
            }
            if (!stream->get16bitsLE((U8*)&version))
            {
              fprintf(stderr,"ERROR: reading version %d of item %d\n", version, j);
              return FALSE;
            }
            header.laszip->items[j].type = (LASitem::Type)type;
            header.laszip->items[j].size = size;
            header.laszip->items[j].version = version;
          }
        }
        else if (strcmp(header.vlrs[i].user_id, "finalized tile") == 0 || strcmp(header.vlrs[i].user_id, "lastiling tile") == 0)
        {
          header.clean_lastiling();
          header.vlr_lastiling = new LASvlr_lastiling();

          // read this data following the header of the variable length record
          //     U32  level           4 bytes 
          //     U32  level_index     4 bytes 
          //     U32  implicit_levels 4 bytes 
          //     F32  min_x           4 bytes 
          //     F32  max_x           4 bytes 
          //     F32  min_y           4 bytes 
          //     F32  max_y           4 bytes 
          // which totals 28 bytes

          if (!stream->get32bitsLE((U8*)&(header.vlr_lastiling->level)))
          {
            fprintf(stderr,"ERROR: reading vlr_lastiling->level %d\n", (I32)header.vlr_lastiling->level);
            return FALSE;
          }
          if (!stream->get32bitsLE((U8*)&(header.vlr_lastiling->level_index)))
          {
            fprintf(stderr,"ERROR: reading vlr_lastiling->level_index %d\n", (I32)header.vlr_lastiling->level_index);
            return FALSE;
          }
          if (!stream->get32bitsLE((U8*)&(header.vlr_lastiling->implicit_levels)))
          {
            fprintf(stderr,"ERROR: reading vlr_lastiling->implicit_levels %d\n", (I32)header.vlr_lastiling->implicit_levels);
            return FALSE;
          }
          if (!stream->get32bitsLE((U8*)&(header.vlr_lastiling->min_x)))
          {
            fprintf(stderr,"ERROR: reading vlr_lastiling->min_x %g\n", header.vlr_lastiling->min_x);
            return FALSE;
          }
          if (!stream->get32bitsLE((U8*)&(header.vlr_lastiling->max_x)))
          {
            fprintf(stderr,"ERROR: reading vlr_lastiling->max_x %g\n", header.vlr_lastiling->max_x);
            return FALSE;
          }
          if (!stream->get32bitsLE((U8*)&(header.vlr_lastiling->min_y)))
          {
            fprintf(stderr,"ERROR: reading vlr_lastiling->min_y %g\n", header.vlr_lastiling->min_y);
            return FALSE;
          }
          if (!stream->get32bitsLE((U8*)&(header.vlr_lastiling->max_y)))
          {
            fprintf(stderr,"ERROR: reading vlr_lastiling->max_y %g\n", header.vlr_lastiling->max_y);
            return FALSE;
          }
        }
        else
        {
          header.vlrs[i].data = new char[header.vlrs[i].record_length_after_header];

          if (!stream->getBytes((U8*)header.vlrs[i].data, header.vlrs[i].record_length_after_header))
          {
            fprintf(stderr,"ERROR: reading %d bytes of data into header.vlrs[%d].data\n", header.vlrs[i].record_length_after_header, i);
            return FALSE;
          }
        }
      }
      else
      {
        header.vlrs[i].data = 0;
      }

      // keep track on the number of bytes we have read so far

      vlrs_size += header.vlrs[i].record_length_after_header;

      // special handling for known variable header tags

      if (strcmp(header.vlrs[i].user_id, "LASF_Projection") == 0)
      {
        if (header.vlrs[i].record_id == 34735) // GeoKeyDirectoryTag
        {
          if (header.vlr_geo_keys)
          {
            fprintf(stderr,"WARNING: variable length records contain more than one GeoKeyDirectoryTag\n");
          }
          header.vlr_geo_keys = (LASvlr_geo_keys*)header.vlrs[i].data;

          // check variable header geo keys contents

          if (header.vlr_geo_keys->key_directory_version != 1)
          {
            fprintf(stderr,"WARNING: wrong vlr_geo_keys->key_directory_version: %d != 1\n",header.vlr_geo_keys->key_directory_version);
          }
          if (header.vlr_geo_keys->key_revision != 1)
          {
            fprintf(stderr,"WARNING: wrong vlr_geo_keys->key_revision: %d != 1\n",header.vlr_geo_keys->key_revision);
          }
          if (header.vlr_geo_keys->minor_revision != 0)
          {
            fprintf(stderr,"WARNING: wrong vlr_geo_keys->minor_revision: %d != 0\n",header.vlr_geo_keys->minor_revision);
          }
          header.vlr_geo_key_entries = (LASvlr_key_entry*)&header.vlr_geo_keys[1];
        }
        else if (header.vlrs[i].record_id == 34736) // GeoDoubleParamsTag
        {
          if (header.vlr_geo_double_params)
          {
            fprintf(stderr,"WARNING: variable length records contain more than one GeoF64ParamsTag\n");
          }
          header.vlr_geo_double_params = (F64*)header.vlrs[i].data;
        }
        else if (header.vlrs[i].record_id == 34737) // GeoAsciiParamsTag
        {
          if (header.vlr_geo_ascii_params)
          {
            fprintf(stderr,"WARNING: variable length records contain more than one GeoAsciiParamsTag\n");
          }
          header.vlr_geo_ascii_params = (I8*)header.vlrs[i].data;
        }
      }
      else if (strcmp(header.vlrs[i].user_id, "LASF_Spec") == 0)
      {
        if (header.vlrs[i].record_id == 0) // ClassificationLookup
        {
          if (header.vlr_classification)
          {
            fprintf(stderr,"WARNING: variable length records contain more than one ClassificationLookup\n");
          }
          header.vlr_classification = (LASvlr_classification*)header.vlrs[i].data;
        }
        else if (header.vlrs[i].record_id == 2) // Histogram
        {
        }
        else if (header.vlrs[i].record_id == 3) // TextAreaDescription
        {
        }
        else if ((header.vlrs[i].record_id >= 100) && (header.vlrs[i].record_id < 355)) // WavePacketDescriptor
        {
          I32 idx = header.vlrs[i].record_id - 99;

          if (header.vlr_wave_packet_descr == 0)
          {
            header.vlr_wave_packet_descr = new LASvlr_wave_packet_descr*[256];
            for (j = 0; j < 256; j++) header.vlr_wave_packet_descr[j] = 0;
          }
          if (header.vlr_wave_packet_descr[idx])
          {
            fprintf(stderr,"WARNING: variable length records defines wave packet descr %d more than once\n", idx);
          }
          header.vlr_wave_packet_descr[idx] = (LASvlr_wave_packet_descr*)header.vlrs[i].data;
        }
      }
      else if (strcmp(header.vlrs[i].user_id, "laszip encoded") == 0 || strcmp(header.vlrs[i].user_id, "finalized tile") == 0)
      {
        // we take our own VLRs away from everywhere
        header.offset_to_point_data -= (54+header.vlrs[i].record_length_after_header);
        vlrs_size -= (54+header.vlrs[i].record_length_after_header);
        i--;
        header.number_of_variable_length_records--;
      }
    }
  }

  if (header.laszip)
  {
    if (!header.laszip->check())
    {
      fprintf(stderr,"ERROR: %s\n", header.laszip->get_error());
      fprintf(stderr,"       please upgrade to the latest release of LAStools (with LASzip)\n");
      fprintf(stderr,"       or contact 'martin.isenburg@gmail.com' for assistance.\n");
      return FALSE;
    }
  }

  // load any number of user-defined bytes that might have been added after the header

  header.user_data_after_header_size = (int)header.offset_to_point_data - vlrs_size - header.header_size;
  if (header.user_data_after_header_size)
  {
    header.user_data_after_header = new char[header.user_data_after_header_size];

    if (!stream->getBytes((U8*)header.user_data_after_header, header.user_data_after_header_size))
    {
      fprintf(stderr,"ERROR: reading %d bytes of data into header.user_data_after_header\n", header.user_data_after_header_size);
      return FALSE;
    }
  }

  // remove extra bits in point data type

  if ((header.point_data_format & 128) || (header.point_data_format & 64)) 
  {
    if (!header.laszip)
    {
      fprintf(stderr,"ERROR: this file was compressed with an experimental version of laszip\n");
      fprintf(stderr,"ERROR: please contact 'martin.isenburg@gmail.com' for assistance.\n");
      return FALSE;
    }
    header.point_data_format &= 127;
  }

  // create the point reader

  reader = new LASreadPoint();

  // initialize point and the reader

  if (header.laszip)
  {
    if (!point.init(&header, header.laszip->num_items, header.laszip->items)) return FALSE;
    if (!reader->setup(header.laszip->num_items, header.laszip->items, header.laszip)) return FALSE;
  }
  else
  {
    if (!point.init(&header, header.point_data_format, header.point_data_record_length)) return FALSE;
    if (!reader->setup(point.num_items, point.items)) return FALSE;
  }
  if (!reader->init(stream)) return FALSE;

  npoints = header.number_of_point_records;
  p_count = 0;

  return TRUE;
}

BOOL LASreaderSingle::seek(const I64 p_index)
{
  if (p_index < npoints)
  {
    if (reader->seek((U32)p_count, (U32)p_index))
    {
      p_count = p_index;
      return TRUE;
    }
  }
  return FALSE;
}

BOOL LASreaderSingle::read_point_default()
{
  if (p_count < npoints)
  {
    if (reader->read(point.point) == FALSE)
    {
      fprintf(stderr,"WARNING: end-of-file after %u of %u points\n", (U32)p_count, (U32)npoints);
      return FALSE;
    }
    p_count++;
    return TRUE;
  }
  return FALSE;
}

void LASreaderSingle::close()
{
  if (reader) 
  {
    reader->done();
    delete reader;
    reader = 0;
  }
  if (stream)
  {
    delete stream;
    stream = 0;
  }
  if (file)
  {
    fclose(file);
    file = 0;
  }
}

LASreaderSingle::LASreaderSingle()
{
  file = 0;
  stream = 0;
  reader = 0;
}

LASreaderSingle::~LASreaderSingle()
{
  if (reader || stream) close();
}

LASreaderSingleRescale::LASreaderSingleRescale(F64 x_scale_factor, F64 y_scale_factor, F64 z_scale_factor) : LASreaderSingle()
{
  scale_factor[0] = x_scale_factor;
  scale_factor[1] = y_scale_factor;
  scale_factor[2] = z_scale_factor;
}

BOOL LASreaderSingleRescale::read_point_default()
{
  if (!LASreaderSingle::read_point_default()) return FALSE;
  if (rescale_x)
  {
    F64 coordinate = (orig_x_scale_factor*point.x)/header.x_scale_factor;
    point.x = I32_QUANTIZE(coordinate);
  }
  if (rescale_y)
  {
    F64 coordinate = (orig_y_scale_factor*point.y)/header.y_scale_factor;
    point.y = I32_QUANTIZE(coordinate);
  }
  if (rescale_z)
  {
    F64 coordinate = (orig_z_scale_factor*point.z)/header.z_scale_factor;
    point.z = I32_QUANTIZE(coordinate);
  }
  return TRUE;
}

BOOL LASreaderSingleRescale::open(ByteStreamIn* stream)
{
  if (!LASreaderSingle::open(stream)) return FALSE;
  // do we need to change anything
  rescale_x = rescale_y = rescale_z = FALSE;
  orig_x_scale_factor = header.x_scale_factor;
  orig_y_scale_factor = header.y_scale_factor;
  orig_z_scale_factor = header.z_scale_factor;
  if (header.x_scale_factor != scale_factor[0])
  {
    header.x_scale_factor = scale_factor[0];
    rescale_x = TRUE;
  }
  if (header.y_scale_factor != scale_factor[1])
  {
    header.y_scale_factor = scale_factor[1];
    rescale_y = TRUE;
  }
  if (header.z_scale_factor != scale_factor[2])
  {
    header.z_scale_factor = scale_factor[2];
    rescale_z = TRUE;
  }
  return TRUE;
}

LASreaderSingleReoffset::LASreaderSingleReoffset(F64 x_offset, F64 y_offset, F64 z_offset) : LASreaderSingle()
{
  this->offset[0] = x_offset;
  this->offset[1] = y_offset;
  this->offset[2] = z_offset;
}

BOOL LASreaderSingleReoffset::read_point_default()
{
  if (!LASreaderSingle::read_point_default()) return FALSE;
  if (reoffset_x)
  {
    F64 coordinate = ((header.x_scale_factor*point.x)+orig_x_offset-header.x_offset)/header.x_scale_factor;
    point.x = I32_QUANTIZE(coordinate);
  }
  if (reoffset_y)
  {
    F64 coordinate = ((header.y_scale_factor*point.y)+orig_y_offset-header.y_offset)/header.y_scale_factor;
    point.y = I32_QUANTIZE(coordinate);
  }
  if (reoffset_z)
  {
    F64 coordinate = ((header.z_scale_factor*point.z)+orig_z_offset-header.z_offset)/header.z_scale_factor;
    point.z = I32_QUANTIZE(coordinate);
  }
  return TRUE;
}

BOOL LASreaderSingleReoffset::open(ByteStreamIn* stream)
{
  if (!LASreaderSingle::open(stream)) return FALSE;
  // do we need to change anything
  reoffset_x = reoffset_y = reoffset_z = FALSE;
  orig_x_offset = header.x_offset;
  orig_y_offset = header.y_offset;
  orig_z_offset = header.z_offset;
  if (header.x_offset != offset[0])
  {
    header.x_offset = offset[0];
    reoffset_x = TRUE;
  }
  if (header.y_offset != offset[1])
  {
    header.y_offset = offset[1];
    reoffset_y = TRUE;
  }
  if (header.z_offset != offset[2])
  {
    header.z_offset = offset[2];
    reoffset_z = TRUE;
  }
  return TRUE;
}

LASreaderSingleRescaleReoffset::LASreaderSingleRescaleReoffset(F64 x_scale_factor, F64 y_scale_factor, F64 z_scale_factor, F64 x_offset, F64 y_offset, F64 z_offset) : LASreaderSingleRescale(x_scale_factor, y_scale_factor, z_scale_factor), LASreaderSingleReoffset(x_offset, y_offset, z_offset)
{
}

BOOL LASreaderSingleRescaleReoffset::read_point_default()
{
  if (!LASreaderSingle::read_point_default()) return FALSE;
  if (reoffset_x)
  {
    F64 coordinate = ((orig_x_scale_factor*point.x)+orig_x_offset-header.x_offset)/header.x_scale_factor;
    point.x = I32_QUANTIZE(coordinate);
  }
  else if (rescale_x)
  {
    F64 coordinate = (orig_x_scale_factor*point.x)/header.x_scale_factor;
    point.x = I32_QUANTIZE(coordinate);
  }
  if (reoffset_y)
  {
    F64 coordinate = ((orig_y_scale_factor*point.y)+orig_y_offset-header.y_offset)/header.y_scale_factor;
    point.y = I32_QUANTIZE(coordinate);
  }
  else if (rescale_y)
  {
    F64 coordinate = (orig_y_scale_factor*point.y)/header.y_scale_factor;
    point.y = I32_QUANTIZE(coordinate);
  }
  if (reoffset_z)
  {
    F64 coordinate = ((orig_z_scale_factor*point.z)+orig_z_offset-header.z_offset)/header.z_scale_factor;
    point.z = I32_QUANTIZE(coordinate);
  }
  else if (rescale_z)
  {
    F64 coordinate = (orig_z_scale_factor*point.z)/header.z_scale_factor;
    point.z = I32_QUANTIZE(coordinate);
  }
  return TRUE;
}

BOOL LASreaderSingleRescaleReoffset::open(ByteStreamIn* stream)
{
  if (!LASreaderSingleRescale::open(stream)) return FALSE;
  // do we need to change anything
  reoffset_x = reoffset_y = reoffset_z = FALSE;
  orig_x_offset = header.x_offset;
  orig_y_offset = header.y_offset;
  orig_z_offset = header.z_offset;
  if (header.x_offset != offset[0])
  {
    header.x_offset = offset[0];
    reoffset_x = TRUE;
  }
  if (header.y_offset != offset[1])
  {
    header.y_offset = offset[1];
    reoffset_y = TRUE;
  }
  if (header.z_offset != offset[2])
  {
    header.z_offset = offset[2];
    reoffset_z = TRUE;
  }
  return TRUE;
}
