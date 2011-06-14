/*
 *
 *  Created by Peter Zion on 10-12-01.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_KL_LOCATION_H
#define _FABRIC_KL_LOCATION_H

#include <Fabric/Base/Config.h>
#include <stdlib.h> // For size_t
#include <stdio.h> // for snprintf
#include <string>

namespace Fabric
{
  namespace KL
  {
    class Location
    {
    public:
    
      Location( size_t index = 0, size_t line = 1, size_t column = 1 )
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
      
      size_t index() const
      {
        return m_index;
      }
      
      void setIndex( size_t index )
      {
        m_index = index;
      }
      
      size_t line() const
      {
        return m_line;
      }
      
      void setLine( size_t line )
      {
        m_line = line;
      }
      
      size_t column() const
      {
        return m_column;
      }
      
      void setColumn( size_t column )
      {
        m_column = column;
      }
      
      std::string desc() const
      {
        char buf[128];
        snprintf( buf, 128, "%u:%u[%u]", (unsigned)m_line, (unsigned)m_column, (unsigned)m_index );
        return std::string(buf);
      }
      
    private:
    
      size_t m_index;
      size_t m_line;
      size_t m_column;
    };
  };
};

#endif //_FABRIC_KL_LOCATION_H
