/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_STRING_SOURCE_H
#define _FABRIC_KL_STRING_SOURCE_H

#include <Fabric/Core/KL/Source.h>
#include <Fabric/Base/RC/ConstHandle.h>

#include <string>

namespace Fabric
{
  namespace KL
  {
    class StringSource : public Source
    {
    public:
    
      static RC::ConstHandle<StringSource> Create( std::string const &source )
      {
        return new StringSource( source );
      }
      
    protected:
    
      StringSource( std::string const &source )
        : m_source( source )
      {
      }
      
      virtual char const *data() const
      {
        return m_source.data();
      }
      
      virtual size_t length() const
      {
        return m_source.length();
      }
      
    private:
    
      std::string m_source;
    };
  };
};

#endif //_FABRIC_KL_STRING_SOURCE_H
