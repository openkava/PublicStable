/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_USE_VECTOR_H
#define _FABRIC_AST_USE_VECTOR_H

#include <Fabric/Base/RC/Vector.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>

#include <string>
#include <set>

namespace Fabric
{
  namespace Util
  {
    class JSONGenerator;
  };
  
  namespace AST
  {
    class Use;
    class UseNameToLocationMap;
    
    class UseVector : public RC::Vector< RC::ConstHandle<Use> >
    {
    public:
      
      static RC::ConstHandle<UseVector> Create( RC::ConstHandle<Use> const &first = 0, RC::ConstHandle<UseVector> const &remaining = 0 );

      void appendJSON( Util::JSONGenerator const &jsonGenerator, bool includeLocation ) const;
      void collectUses( UseNameToLocationMap &uses ) const;

    protected:
    
      UseVector();
    };
  };
};

#endif //_FABRIC_AST_USE_VECTOR_H
