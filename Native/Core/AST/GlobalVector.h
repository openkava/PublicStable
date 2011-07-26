/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_GLOBAL_VECTOR_H
#define _FABRIC_AST_GLOBAL_VECTOR_H

#include <Fabric/Base/RC/Vector.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace JSON
  {
    class Value;
    class Array;
  };
  
  namespace RT
  {
    class Manager;
  };
  
  namespace CG
  {
    class ModuleBuilder;
    class Diagnostics;
  };
  
  namespace AST
  {
    class Global;
    
    class GlobalVector : public RC::Vector< RC::ConstHandle<Global> >
    {
    public:
      
      static RC::ConstHandle<GlobalVector> Create( RC::ConstHandle<Global> const &first = 0, RC::ConstHandle<GlobalVector> const &remaining = 0 );
      static RC::ConstHandle<GlobalVector> Create( RC::ConstHandle<GlobalVector> const &lhs, RC::ConstHandle<GlobalVector> const &rhs );

      RC::ConstHandle<JSON::Value> toJSON() const;

      void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
          
      void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctions ) const;
    
    protected:
    
      GlobalVector();
      
    private:
    
      mutable RC::ConstHandle<JSON::Value> m_jsonValue;
    };
  };
};

#endif //_FABRIC_AST_GLOBAL_VECTOR_H
