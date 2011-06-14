/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_AST_NODE_H
#define _FABRIC_AST_NODE_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Core/CG/Location.h>

namespace Fabric
{
  namespace CG
  {
    class Diagnostics;
    class Error;
  }
  
  namespace AST
  {
    class Node : public RC::Object
    {
    public:
    
      Node( CG::Location const &location );
    
      virtual std::string localDesc() const = 0;
      virtual std::string deepDesc( std::string const &indent ) const;
      
      CG::Location const &getLocation() const
      {
        return m_location;
      }
      
      void addWarning( CG::Diagnostics &diagnostics, std::string const &desc ) const;
      void addError( CG::Diagnostics &diagnostics, std::string const &desc ) const;

    protected:
    
      void addError( CG::Diagnostics &diagnostics, CG::Error const &error ) const;
      
    private:
    
      CG::Location m_location;
    };
  };
};

#endif //_FABRIC_AST_NODE_H
