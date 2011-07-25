/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_STATEMENT_VECTOR_H
#define _FABRIC_AST_STATEMENT_VECTOR_H

#include <Fabric/Base/RC/Vector.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace Util
  {
    class SimpleString;
  };
  
  namespace JSON
  {
    class Value;
    class Array;
  };
  
  namespace CG
  {
    class BasicBlockBuilder;
    class Diagnostics;
    class ModuleBuilder;
  };
  
  namespace AST
  {
    class Statement;
    
    class StatementVector : public RC::Vector< RC::ConstHandle<Statement> >
    {
    public:
      
      static RC::ConstHandle<StatementVector> Create( RC::ConstHandle<Statement> const &first = 0, RC::ConstHandle<StatementVector> const &remaining = 0 );

      void appendJSON( Util::SimpleString &ss ) const;
      
      void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
    
      void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const;

    protected:
    
      StatementVector();
      
    private:
    
      mutable RC::ConstHandle<JSON::Value> m_jsonValue;
    };
  };
};

#endif //_FABRIC_AST_STATEMENT_VECTOR_H
