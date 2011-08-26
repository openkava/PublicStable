/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_AST_USE_H
#define _FABRIC_AST_USE_H

#include <Fabric/Core/AST/Node.h>

namespace Fabric
{
  namespace Util
  {
    class SimpleString;
  };
  
  namespace AST
  {
    class UseNameToLocationMap;
    
    class Use : public Node
    {
      FABRIC_AST_NODE_DECL( Use );
      
    public:

      static RC::ConstHandle<Use> Create(
        CG::Location const &location,
        std::string const &name
        );
      
      void collectUses( UseNameToLocationMap &uses ) const;
     
    protected:
    
      Use(
        CG::Location const &location,
        std::string const &name
        );
      
      virtual void appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const;
      
    private:
    
      std::string m_name;
    };
  };
};

#endif //_FABRIC_AST_USE_H
