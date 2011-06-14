/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_CG_DIAGNOSTICS_H
#define _FABRIC_CG_DIAGNOSTICS_H

#include <Fabric/Core/CG/Location.h>
#include <map>

namespace Fabric
{
  namespace CG
  {
    class Diagnostic
    {
    public:
    
      enum Level
      {
        LEVEL_WARNING,
        LEVEL_ERROR
      };
      
      Diagnostic( Level level, std::string const &desc )
        : m_level( level )
        , m_desc( desc )
      {
      }
      
      Diagnostic( Diagnostic const &that )
        : m_level( that.m_level )
        , m_desc( that.m_desc )
      {
      }
      
      Diagnostic &operator =( Diagnostic const &that )
      {
        m_level = that.m_level;
        m_desc = that.m_desc;
        return *this;
      }
      
      Level getLevel() const
      {
        return m_level;
      }
      
      char const *getLevelDesc() const
      {
        switch ( m_level )
        {
          case LEVEL_WARNING:
            return "warning";
          case LEVEL_ERROR:
            return "error";
        }
        return "<unknown level>";
      }
      
      std::string const &getDesc() const
      {
        return m_desc;
      }
      
      std::string desc() const
      {
        char buf[1024];
        snprintf( buf, 1024, "%s: %s", getLevelDesc(), m_desc.c_str() );
        return std::string( buf );
      }
      
    private:
    
      Level m_level;
      std::string m_desc;
    };
    
    class Diagnostics
    {
      typedef std::multimap< Location, Diagnostic, LocationLessThanComparator > Impl;

    public:
    
      Diagnostics()
        : m_containsError( false )
      {
      }
      
      void clear()
      {
        m_impl.clear();
        m_containsError = false;
      }
      
      void add( Diagnostic::Level level, Location const &location, std::string const &desc )
      {
        Diagnostic diagnostic( level, desc );
        m_impl.insert( Impl::value_type( location, diagnostic ) );
        if ( level == Diagnostic::LEVEL_ERROR )
          m_containsError = true;
        //FABRIC_LOG( "*** %u:%u: %s: %s\n", (unsigned)location.getLine(), (unsigned)location.getColumn(), diagnostic.getLevelDesc(), diagnostic.getDesc().c_str() );
      }
      
      void addWarning( Location const &location, std::string const &desc )
      {
        add( Diagnostic::LEVEL_WARNING, location, desc );
      }
      
      void addWarning( Location const &location, char const *format, ... )
      {
        char buf[1024];
        va_list argList;
        va_start( argList, format );
        vsnprintf( buf, 1024, format, argList );
        va_end( argList );
        add( Diagnostic::LEVEL_WARNING, location, buf );
      }
      
      void addError( Location const &location, std::string const &desc )
      {
        add( Diagnostic::LEVEL_ERROR, location, desc );
      }
      
      void addError( Location const &location, char const *format, ... )
      {
        char buf[1024];
        va_list argList;
        va_start( argList, format );
        vsnprintf( buf, 1024, format, argList );
        va_end( argList );
        add( Diagnostic::LEVEL_ERROR, location, buf );
      }
      
      bool empty() const
      {
        return m_impl.empty();
      }
      
      typedef Impl::const_iterator const_iterator;
      
      const_iterator begin() const
      {
        return m_impl.begin();
      }
      
      const_iterator end() const
      {
        return m_impl.end();
      }
      
      bool containsError() const
      {
        return m_containsError;
      }
    
    private:
    
      Impl m_impl;
      bool m_containsError;
    };
  };
};

#endif //_FABRIC_CG_DIAGNOSTICS_H
