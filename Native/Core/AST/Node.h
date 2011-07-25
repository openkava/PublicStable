/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_AST_NODE_H
#define _FABRIC_AST_NODE_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Core/CG/Location.h>

namespace Fabric
{
  namespace Util
  {
    SimpleString;
  };
  
  namespace JSON
  {
    class Value;
    class Object;
  };
  
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

      virtual RC::ConstHandle<JSON::String> nodeTypeNameJSONString() const = 0;
      void appendJSONString( Util::SimpleString &ss ) const;
      
      CG::Location const &getLocation() const
      {
        return m_location;
      }
      
      void addWarning( CG::Diagnostics &diagnostics, std::string const &desc ) const;
      void addError( CG::Diagnostics &diagnostics, std::string const &desc ) const;

    protected:
      
      virtual void appendJSONStringMembers( Util::SimpleString &ss ) const = 0;

      void addError( CG::Diagnostics &diagnostics, CG::Error const &error ) const;
      
    private:
    
      CG::Location m_location;
    };
  };
};

#define FABRIC_AST_NODE_DECL(NodeName) \
  public: \
    \
    static char const *NodeTypeName(); \
    virtual char const *nodeTypeName() const; \
    
#define FABRIC_AST_NODE_IMPL(NodeName) \
    char const *NodeName::NodeTypeName() \
    { \
      static char const *result = #NodeName; \
      return result; \
    } \
    \
    char const *NodeName::nodeTypeName() const \
    { \
      return NodeTypeName(); \
    } \

#endif //_FABRIC_AST_NODE_H
