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
      
      static RC::ConstHandle<CaseVector> Create( RC::ConstHandle<Case> const &first = 0, RC::ConstHandle<CaseVector> const &remaining = 0 );

      RC::Handle<JSON::Array> toJSON() const;
      
      void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
    
    protected:
    
      CaseVector();
    };
  };
};

#endif //_FABRIC_AST_CASE_VECTOR_H
