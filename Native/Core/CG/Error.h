/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_CG_ERROR_H
#define _FABRIC_CG_ERROR_H

#include <Fabric/Base/Exception.h>
#include <Fabric/Core/CG/Location.h>

namespace Fabric
{
  namespace CG
  {
    class Error : public Exception
    {
    public:
    
      Error( Location const &location, std::string const &desc );
      Error( Error const &that );
      
      Error &operator =( Error const &that );
      
      Location const &getLocation() const
      {
        return m_location;
      }
      
    private:
    
      Location m_location;
    };
    
    Error operator +( std::string const &prefix, Error const &error );
    
    inline Error operator +( char const *cString, Error const &error )
    {
      return std::string(cString) + error;
    }
  };
};

#endif //_FABRIC_CG_ERROR_H
