/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_LOCATION_H
#define _FABRIC_KL_LOCATION_H

#include <Fabric/Core/CG/Location.h>

#include <stdlib.h>

namespace Fabric
{
  namespace KL
  {
    class Location
    {
    public:
    
      Location()
        : m_index( 0 )
        , m_line( 1 )
        , m_column( 1 )
      {
      }
    
      Location( size_t index, size_t line, size_t column )
        : m_index( index )
        , m_line( line )
        , m_column( column )
      {
      }
      
      Location( Location const &that )
        : m_index( that.m_index )
        , m_line( that.m_line )
        , m_column( that.m_column )
      {
      }
      
      Location &operator =( Location const &that )
      {
        m_index = that.m_index;
        m_line = that.m_line;
        m_column = that.m_column;
        return *this;
      }
      
      size_t getIndex() const
      {
        return m_index;
      }
      
      void setIndex( size_t index )
      {
        m_index = index;
      }
      
      size_t getLine() const
      {
        return m_line;
      }
      
      void setLine( size_t line )
      {
        m_line = line;
      }
      
      size_t getColumn() const
      {
        return m_column;
      }
      
      void setColumn( size_t column )
      {
        m_column = column;
      }
      
      operator CG::Location() const
      {
        return CG::Location( m_line, m_column );
      }
      
    private:
    
      size_t m_index;
      size_t m_line;
      size_t m_column;
    };
  };
};

#endif //_FABRIC_KL_LOCATION_H
