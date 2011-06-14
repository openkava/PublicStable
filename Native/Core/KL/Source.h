/*
 *
 *  Created by Peter Zion on 10-12-01.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_KL_SOURCE_H
#define _FABRIC_KL_SOURCE_H

#include <Fabric/Core/KL/Location.h>
#include <Fabric/Core/Util/Assert.h>

namespace Fabric
{
  namespace KL
  {
    class Source
    {
    public:
    
      Source( char const *data, size_t length )
        : m_data( data )
        , m_length( length )
      {
      }
    
      char const *data() const
      {
        return m_data;
      }
      
      size_t length() const
      {
        return m_length;
      }
      
      Location locationForStart()
      {
        forwardThroughEscapes();
        return m_location;
      }
      
      Location locationForEnd() const
      {
        return m_location;
      }
      
      bool isEOF()
      {
        forwardThroughEscapes();
        return m_location.index() == m_length;
      }
      
      int peek()
      {
        forwardThroughEscapes();
        size_t    index = m_location.index();
        if( (m_location.index()+2) < m_length &&
            m_data[m_location.index()] == '\r' &&
            m_data[m_location.index()+1] == '\n' )
        {
          index++;
        }
        if ( index < m_length )
          return m_data[index];
        else return -1;
      }
      
      char advance( std::string *text = 0 )
      {
        FABRIC_ASSERT( m_location.index() < m_length );
        char result = m_data[m_location.index()];

        // Eat DOS eol.
        if( result == '\r' && ( m_location.index() + 1 ) < m_length )
        {
          // Is the next one '\n'?
          char next = m_data[m_location.index()+1];
          if( next == '\n' )
          {
            result = next;
            m_location.setIndex( m_location.index() + 1 );
          }
        }

        if ( text )
          text->push_back( result );

        if ( result == '\n' )
        {
          m_location.setLine( m_location.line() + 1 );
          m_location.setColumn( 1 );
        }
        else 
        {
          m_location.setColumn( m_location.column() + 1 );
        }
        m_location.setIndex( m_location.index() + 1 );
        return result;
      }
    
      void forwardThroughEscapes()
      {
        for( ;; )
        {
          bool    dos = false;
          if( ( m_location.index()+1 < m_length
                && m_data[m_location.index()] == '\\'
                && m_data[m_location.index()+1] == '\n'
             ) || ( dos = (
                m_location.index()+2 < m_length 
                && m_data[m_location.index()] == '\\'
                && m_data[m_location.index()+1] == '\r'
                && m_data[m_location.index()+2] == '\n'
              ) ) )
          {
            m_location.setIndex( m_location.index() + (dos ? 3 : 2) );
            m_location.setLine( m_location.line() + 1 );
            m_location.setColumn( 1 );
          }
          else
          {
            break;
          }
        }
      }
      
      std::string friendlyStringForLocations( Location const &startLocation, Location const &endLocation ) const
      {
        size_t start = startLocation.index();
        size_t length = endLocation.index() - startLocation.index();
        FABRIC_ASSERT( start + length <= m_length );
        if ( length > 35 )
          return std::string( &m_data[start], 16 ) + "..." + std::string( &m_data[start+length-16], 16 );
        else return std::string( &m_data[start], length );
      }
    
    private:
      
      char const *m_data;
      size_t m_length;
      Location m_location;
    };
  };
};

#endif //_FABRIC_KL_SOURCE_H
