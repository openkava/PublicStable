#ifndef _FABRIC_CG_LOCATION_H
#define _FABRIC_CG_LOCATION_H

#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace CG
  {
    class Location
    {
    public:
    
      Location( size_t line, size_t column )
        : m_line( line )
        , m_column( column )
      {
      }
      
      Location( Location const &that )
        : m_line( that.m_line )
        , m_column( that.m_column )
      {
      }
      
      Location &operator =( Location const &that )
      {
        m_line = that.m_line;
        m_column = that.m_column;
        return *this;
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
        return _(m_line) + ":" + _(m_column);
      }
      
      void appendJSON( Util::JSONGenerator const &jsonGenerator ) const
      {
        Util::JSONArrayGenerator jsonArrayGenerator = jsonGenerator.makeArray();
        jsonArrayGenerator.makeElement().makeInteger( m_line );
        jsonArrayGenerator.makeElement().makeInteger( m_column );
      }
      
    private:
    
      size_t m_line;
      size_t m_column;
    };

    struct LocationLessThanComparator
    {
      bool operator ()( Location const &lhs, Location const &rhs )
      {
        return lhs.getLine() < rhs.getLine()
          || (lhs.getLine() == rhs.getLine() && lhs.getColumn() < rhs.getColumn());
      }
    };
  };
  
  inline std::string _( CG::Location const &location )
  {
    return location.desc();
  }
};

#endif //_FABRIC_CG_LOCATION_H
