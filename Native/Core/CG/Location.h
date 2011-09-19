/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_CG_LOCATION_H
#define _FABRIC_CG_LOCATION_H

#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Base/RC/String.h>

namespace Fabric
{
  namespace CG
  {
    class Location
    {
    public:
    
      Location( RC::ConstHandle<RC::String> const &filename, size_t line, size_t column )
        : m_filename( filename )
        , m_line( line )
        , m_column( column )
      {
        FABRIC_ASSERT( m_filename );
      }
      
      Location( Location const &that )
        : m_filename( that.m_filename )
        , m_line( that.m_line )
        , m_column( that.m_column )
      {
      }
      
      Location &operator =( Location const &that )
      {
        m_filename = that.m_filename;
        m_line = that.m_line;
        m_column = that.m_column;
        return *this;
      }
      
      RC::ConstHandle<RC::String> getFilename() const
      {
        return m_filename;
      }
      
      size_t getLine() const
      {
        return m_line;
      }
      
      size_t getColumn() const
      {
        return m_column;
      }
      
      std::string desc() const
      {
        return m_filename->stdString() + ":" + _(m_line) + ":" + _(m_column);
      }
      
      void appendJSON( Util::JSONGenerator const &jsonGenerator, bool includeLocation ) const
      {
        Util::JSONArrayGenerator jsonArrayGenerator = jsonGenerator.makeArray();
        jsonArrayGenerator.makeElement().makeString( m_filename );
        jsonArrayGenerator.makeElement().makeInteger( m_line );
        jsonArrayGenerator.makeElement().makeInteger( m_column );
      }
      
    private:
    
      RC::ConstHandle<RC::String> m_filename;
      size_t m_line;
      size_t m_column;
    };
  };
};

#endif //_FABRIC_CG_LOCATION_H
