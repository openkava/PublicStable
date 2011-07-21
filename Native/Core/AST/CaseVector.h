/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_CASE_VECTOR_H
#define _FABRIC_AST_CASE_VECTOR_H

#include <Fabric/Base/RC/Vector.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace CG
  {
    class ModuleBuilder;
    class Diagnostics;
  };
  
  namespace JSON
  {
    class Array;
  };
  
  namespace AST
  {
    class Case;
    
    class CaseVector : public RC::Vector< RC::ConstHandle<Case> >
    {
    public:
      
      static RC::Handle<CaseVector> Create();
      static RC::Handle<CaseVector> Create( RC::ConstHandle<Case> const &first );
      static RC::Handle<CaseVector> Create( RC::ConstHandle<Case> const &first, RC::ConstHandle<CaseVector> const &remaining );

      RC::Handle<JSON::Array> toJSON() const;
    
    protected:
    
      CaseVector();
    };
  };
};

#endif //_FABRIC_AST_CASE_VECTOR_H