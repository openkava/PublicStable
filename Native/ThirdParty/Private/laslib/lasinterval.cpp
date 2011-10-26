/*
===============================================================================

  FILE:  lasinterval.cpp
  
  CONTENTS:
  
    see corresponding header file
  
  PROGRAMMERS:
  
    martin.isenburg@gmail.com
  
  COPYRIGHT:
  
    (c) 2011, Martin Isenburg, LASSO - tools to catch reality

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    see corresponding header file
  
===============================================================================
*/
#include "lasinterval.hpp"

#include "bytestreamin.hpp"
#include "bytestreamout.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <hash_map.h>
#include <multimap.h>
#include <set.h>

typedef hash_map<I32, LASintervalStartCell*> my_cell_hash;
typedef multimap<U32, LASintervalCell*> my_cell_map;
typedef set<LASintervalCell*> my_cell_set;

LASintervalCell::LASintervalCell()
{
  start = 0;
  end = 0;
  full = 0;
  deleted = 0;
  next = 0;
}

LASintervalCell::LASintervalCell(const U32 p_index)
{
  start = p_index;
  end = p_index;
  full = 1;
  deleted = 0;
  next = 0;
}

LASintervalCell::LASintervalCell(const LASintervalCell* cell)
{
  start = cell->start;
  end = cell->end;
  full = cell->full;
  deleted = 0;
  next = 0;
}

LASintervalStartCell::LASintervalStartCell() : LASintervalCell()
{
  total = full;
  last = 0;
}

LASintervalStartCell::LASintervalStartCell(const U32 p_index) : LASintervalCell(p_index)
{
  total = full;
  last = 0;
}

LASintervalStartCell::LASintervalStartCell(const LASintervalCell* cell) : LASintervalCell(cell)
{
  total = full;
  last = 0;
}

BOOL LASintervalStartCell::add(const U32 p_index, const U32 threshold)
{
  U32 current_end = (last ? last->end : end);
  assert(p_index > current_end);
  U32 diff = p_index - current_end - 1;
  total++;
  if (diff > threshold)
  {
    if (last)
    {
      last->next = new LASintervalCell(p_index);
      last = last->next;
    }
    else
    {
      next = new LASintervalCell(p_index);
      last = next;
    }
    return TRUE; // created new interval
  }
  if (last)
  {
    last->end = p_index;
    last->full++;
  }
  else
  {
    end = p_index;
    full++;
  }
  return FALSE; // added to interval
}

BOOL LASinterval::add(const U32 p_index, const I32 c_index)
{
  if (last_cell == 0 || last_index != c_index)
  {
    last_index = c_index;
    my_cell_hash::iterator hash_element = ((my_cell_hash*)cells)->find(c_index);
    if (hash_element == ((my_cell_hash*)cells)->end())
    {
      last_cell = new LASintervalStartCell(p_index);
      ((my_cell_hash*)cells)->insert(my_cell_hash::value_type(c_index, last_cell));
      number_intervals++;
      return TRUE;
    }
    last_cell = (*hash_element).second;
  }
  if (last_cell->add(p_index, threshold))
  {
    number_intervals++;
    return TRUE;
  }
  return FALSE;
}

// get total number of cells
U32 LASinterval::get_number_cells() const
{
  return ((my_cell_hash*)cells)->size();
}

// get total number of intervals
U32 LASinterval::get_number_intervals() const
{
  return number_intervals;
}

// merge cells (and their intervals) into one cell
BOOL LASinterval::merge_cells(const U32 num_indices, const I32* indices, const I32 new_index)
{
  U32 i;
  if (num_indices == 1)
  {
    my_cell_hash::iterator hash_element = ((my_cell_hash*)cells)->find(indices[0]);
    if (hash_element == ((my_cell_hash*)cells)->end())
    {
      return FALSE;
    }
    ((my_cell_hash*)cells)->erase(hash_element);
    ((my_cell_hash*)cells)->insert(my_cell_hash::value_type(new_index, (*hash_element).second));
  }
  else
  {
    if (cells_to_merge) ((my_cell_set*)cells_to_merge)->clear();
    for (i = 0; i < num_indices; i++)
    {
      add_cell_to_merge(indices[i], TRUE);
    }
    if (!merge(TRUE)) return FALSE;
    ((my_cell_hash*)cells)->insert(my_cell_hash::value_type(new_index, merged_cells));
    merged_cells = 0;
  }
  return TRUE;
}

// merge adjacent intervals with small gaps in cells to reduce total interval number to maximum
void LASinterval::merge_intervals(U32 maximum_intervals)
{
  U32 diff;
  LASintervalCell* cell;
  LASintervalCell* delete_cell;

  // since each cell has minimum one interval
  if (maximum_intervals < get_number_cells())
  {
    maximum_intervals = 0;
  }
  else
  {
    maximum_intervals -= get_number_cells();
  }

  my_cell_map map;
  my_cell_hash::iterator hash_element = ((my_cell_hash*)cells)->begin();
  while (hash_element != ((my_cell_hash*)cells)->end())
  {
    cell = (*hash_element).second;
    while (cell->next)
    {
      diff = cell->next->start - cell->end - 1;
      map.insert(my_cell_map::value_type(diff, cell));
      cell = cell->next;
    }
    hash_element++;
  }

  my_cell_map::iterator map_element;
  U32 size = map.size();
  diff = threshold;
  while (size > maximum_intervals)
  {
    map_element = map.begin();
    map.erase(map_element);
    diff = (*map_element).first;
    cell = (*map_element).second;
    if (cell->deleted)
    {
      number_intervals--;
      delete cell;
    }
    else
    {
      size--;
      delete_cell = cell->next;
      cell->full += delete_cell->full;
      cell->end = delete_cell->end;
      cell->next = delete_cell->next;
      if (cell->next)
      {
        map.insert(my_cell_map::value_type(cell->next->start - cell->end - 1, cell));
        delete_cell->deleted = 1;
      }
      else
      {
        number_intervals--;
        delete delete_cell;
      }
    }
  }
  fprintf(stderr,"largest interval gap increased to %u\n", diff);
  map_element = map.begin();
  while (true)
  {
    if (map_element == map.end()) break;
    cell = (*map_element).second;
    if (cell->deleted)
    {
      number_intervals--;
      delete cell;
    }
    map_element++;
  }
}

void LASinterval::get_cells()
{
  last_index = I32_MIN;
  current_cell = 0;
}

BOOL LASinterval::has_cells()
{
  my_cell_hash::iterator hash_element;
  if (last_index == I32_MIN)
  {
    hash_element = ((my_cell_hash*)cells)->begin();
  }
  else
  {
    hash_element = ((my_cell_hash*)cells)->find(last_index);
    hash_element++;
  }
  if (hash_element == ((my_cell_hash*)cells)->end())
  {
    last_index = I32_MIN;
    current_cell = 0;
    return FALSE;
  }
  last_index = (*hash_element).first;
  index = (*hash_element).first;
  total = (*hash_element).second->total;
  current_cell = (*hash_element).second;
  return TRUE;
}

BOOL LASinterval::get_cell(const I32 c_index)
{
  my_cell_hash::iterator hash_element = ((my_cell_hash*)cells)->find(c_index);
  if (hash_element == ((my_cell_hash*)cells)->end())
  {
    current_cell = 0;
    return FALSE;
  }
  index = (*hash_element).first;
  total = (*hash_element).second->total;
  current_cell = (*hash_element).second;
  return TRUE;
}

BOOL LASinterval::add_cell_to_merge(const I32 c_index, const BOOL erase)
{
  my_cell_hash::iterator hash_element = ((my_cell_hash*)cells)->find(c_index);
  if (hash_element == ((my_cell_hash*)cells)->end())
  {
    return FALSE;
  }
  if (cells_to_merge == 0)
  {
    cells_to_merge = (void*) new my_cell_set;
  }
  ((my_cell_set*)cells_to_merge)->insert((*hash_element).second);
  if (erase) ((my_cell_hash*)cells)->erase(hash_element);
  return TRUE;
}

BOOL LASinterval::merge(const BOOL erase)
{
  if (cells_to_merge == 0) return FALSE;
  if (((my_cell_set*)cells_to_merge)->size() == 0) return FALSE;
  LASintervalCell* cell;
  // add all intervals to map
  my_cell_map map;
  my_cell_set::iterator set_element = ((my_cell_set*)cells_to_merge)->begin();
  while (true)
  {
    if (set_element == ((my_cell_set*)cells_to_merge)->end()) break;
    cell = (*set_element);
    while (cell)
    {
      map.insert(my_cell_map::value_type(cell->start, cell));
      cell = cell->next;
    }
    set_element++;
  }
  // initialize merged cells with first interval
  my_cell_map::iterator map_element;
  map_element = map.begin();
  map.erase(map_element);
  cell = (*map_element).second;
  merged_cells = new LASintervalStartCell(cell);
  LASintervalCell* last_cell = merged_cells;
  if (erase) delete cell;
  // merge intervals
  I32 diff;
  while (map.size())
  {
    map_element = map.begin();
    map.erase(map_element);
    cell = (*map_element).second;
    diff = cell->start - last_cell->end - 1;
    merged_cells->total += cell->full;
    if (diff > (I32)threshold)
    {
      last_cell->next = new LASintervalCell(cell);
      last_cell = last_cell->next;
    }
    else
    {
      if (cell->end > last_cell->end) last_cell->end = cell->end;
      last_cell->full += cell->full;
    }
    if (erase) delete cell;
  }
  current_cell = merged_cells;
  return TRUE;
}

BOOL LASinterval::get_merged_cell()
{
  current_cell = merged_cells;
  return (current_cell != 0);
}

BOOL LASinterval::has_intervals()
{
  if (current_cell)
  {
    start = current_cell->start;
    end = current_cell->end;
    full = current_cell->full;
    current_cell = current_cell->next;
    return TRUE;
  }
  return FALSE;
}

LASinterval::LASinterval(const U32 threshold)
{
  cells = new my_cell_hash;
  cells_to_merge = 0;
  this->threshold = threshold;
  last_cell = 0;
  current_cell = 0;
  merged_cells = 0;
  number_intervals = 0;
}

LASinterval::~LASinterval()
{
  delete ((my_cell_hash*)cells);
  if (cells_to_merge) delete ((my_cell_set*)cells_to_merge);
}

BOOL LASinterval::read(ByteStreamIn* stream)
{
  char signature[4];
  if (!stream->getBytes((U8*)signature, 4))
  {
    fprintf(stderr,"ERROR (LASinterval): reading signature\n");
    return FALSE;
  }
  U32 version;
  if (!stream->get32bitsLE((U8*)&version))
  {
    fprintf(stderr,"ERROR (LASinterval): reading version\n");
    return FALSE;
  }
  // read number of cells
  U32 number_cells;
  if (!stream->get32bitsLE((U8*)&number_cells))
  {
    fprintf(stderr,"ERROR (LASinterval): reading number of cells\n");
    return FALSE;
  }
  // loop over all cells
  while (number_cells)
  {
    // read index of cell
    I32 cell_index;
    if (!stream->get32bitsLE((U8*)&cell_index))
    {
      fprintf(stderr,"ERROR (LASinterval): reading cell index\n");
      return FALSE;
    }
    // create cell and insert into hash
    LASintervalStartCell* start_cell = new LASintervalStartCell();
    ((my_cell_hash*)cells)->insert(my_cell_hash::value_type(cell_index, start_cell));
    LASintervalCell* cell = start_cell;
    // read number of intervals in cell
    U32 number_intervals;
    if (!stream->get32bitsLE((U8*)&number_intervals))
    {
      fprintf(stderr,"ERROR (LASinterval): reading number of intervals in cell\n");
      return FALSE;
    }
    // read number of points in cell
    U32 number_points;
    if (!stream->get32bitsLE((U8*)&number_points))
    {
      fprintf(stderr,"ERROR (LASinterval): reading number of points in cell\n");
      return FALSE;
    }
    start_cell->full = number_points;
    start_cell->total = number_points;
    while (number_intervals)
    {
      // read start of interval
      if (!stream->get32bitsLE((U8*)&(cell->start)))
      {
        fprintf(stderr,"ERROR (LASinterval): reading start %d of interval\n", cell->start);
        return FALSE;
      }
      // read end of interval
      if (!stream->get32bitsLE((U8*)&(cell->end)))
      {
        fprintf(stderr,"ERROR (LASinterval): reading end %d of interval\n", cell->end);
        return FALSE;
      }
      number_intervals--;
      if (number_intervals)
      {
        cell->next = new LASintervalCell();
        cell = cell->next;
      }
    }
    number_cells--;
  }

  return TRUE;
}

BOOL LASinterval::write(ByteStreamOut* stream) const
{
  if (!stream->putBytes((U8*)"LASV", 4))
  {
    fprintf(stderr,"ERROR (LASinterval): writing signature\n");
    return FALSE;
  }
  U32 version = 0;
  if (!stream->put32bitsLE((U8*)&version))
  {
    fprintf(stderr,"ERROR (LASinterval): writing version\n");
    return FALSE;
  }
  // write number of cells
  U32 number_cells = ((my_cell_hash*)cells)->size();
  if (!stream->put32bitsLE((U8*)&number_cells))
  {
    fprintf(stderr,"ERROR (LASinterval): writing number of cells %d\n", number_cells);
    return FALSE;
  }
  // loop over all cells
  my_cell_hash::iterator hash_element = ((my_cell_hash*)cells)->begin();
  while (hash_element != ((my_cell_hash*)cells)->end())
  {
    // count number of intervals and points in cell
    U32 number_intervals = 0;
    U32 number_points = 0;
    LASintervalCell* cell = (*hash_element).second;
    while (cell)
    {
      number_intervals++;
      number_points += cell->full;
      cell = cell->next;
    }
    // write index of cell
    I32 cell_index = (*hash_element).first;
    if (!stream->put32bitsLE((U8*)&cell_index))
    {
      fprintf(stderr,"ERROR (LASinterval): writing cell index %d\n", cell_index);
      return FALSE;
    }
    // write number of intervals in cell
    if (!stream->put32bitsLE((U8*)&number_intervals))
    {
      fprintf(stderr,"ERROR (LASinterval): writing number of intervals %d in cell\n", number_intervals);
      return FALSE;
    }
    // write number of points in cell
    if (!stream->put32bitsLE((U8*)&number_points))
    {
      fprintf(stderr,"ERROR (LASinterval): writing number of points %d in cell\n", number_points);
      return FALSE;
    }
    // write intervals
    cell = (*hash_element).second;
    while (cell)
    {
      // write start of interval
      if (!stream->put32bitsLE((U8*)&(cell->start)))
      {
        fprintf(stderr,"ERROR (LASinterval): writing start %d of interval\n", cell->start);
        return FALSE;
      }
      // write end of interval
      if (!stream->put32bitsLE((U8*)&(cell->end)))
      {
        fprintf(stderr,"ERROR (LASinterval): writing end %d of interval\n", cell->end);
        return FALSE;
      }
      cell = cell->next;
    }
    hash_element++;
  }
  return TRUE;
}
