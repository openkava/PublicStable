/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_SOURCE_RANGE_H
#define _FABRIC_KL_SOURCE_RANGE_H

#include <Fabric/Core/KL/Source.h>
#include <Fabric/Core/KL/Location.h>
#include <Fabric/Core/Util/Assert.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace KL
  {
    class SourceRange
    {
    public:
    
      SourceRange( RC::ConstHandle<Source> const &source, Location const &startLocation, Location const &endLocation )
        : m_source( source )
        , m_startLocation( startLocation )
        , m_endLocation( endLocation )
      {
        FABRIC_ASSERT( m_startLocation.getIndex() <= m_endLocation.getIndex() );
        FABRIC_ASSERT( m_endLocation.getIndex() <= m_source->length() );
      }
      
      SourceRange( SourceRange const &that )
        : m_source( that.m_source )
        , m_startLocation( that.m_startLocation )
        , m_endLocation( that.m_endLocation )
      {
      }
      
      SourceRange &operator =( SourceRange const &that )
      {
        m_source = that.m_source;
        m_startLocation = that.m_startLocation;
        m_endLocation = that.m_endLocation;
        return *this;
      }
      
      RC::ConstHandle<Source> getSource() const
      {
        return m_source;
      }
      
      Location const &getStartLocation() const
      {
        return m_startLocation;
      }
      
      Location const &getEndLocation() const
      {
        return m_endLocation;
      }
      
      std::string toString() const
      {
        return std::string( &m_source->data()[m_startLocation.getIndex()], m_endLocation.getIndex() - m_startLocation.getIndex() );
      }
      
      size_t getLength() const
      {
        return m_endLocation.getIndex() - m_startLocation.getIndex();
      }
      
      bool operator ==( char const *cStr )
      {
        char const *start = &m_source->data()[m_startLocation.getIndex()];
        const char *end = start + getLength();
        
        while ( start != end )
        {
          if ( *start++ != *cStr++ )
            return false;
        }
        
        return *cStr == '\0';
      }
      
    private:
    
      RC::ConstHandle<Source> m_source;
      Location m_startLocation;
      Location m_endLocation;
    };
  };
};

#endif //_FABRIC_KL_SOURCE_RANGE_H
