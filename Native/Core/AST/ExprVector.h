/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_EXPR_VECTOR_H
#define _FABRIC_AST_EXPR_VECTOR_H

#include <Fabric/Base/RC/Vector.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>
#include <Fabric/Core/CG/ExprType.h>

namespace Fabric
{
  namespace JSON
  {
    class Array;
  };
  
  namespace CG
  {
    class Adapter;
    class BasicBlockBuilder;
    class ExprValue;
  };
  
  namespace AST
  {
    class Expr;
    
    class ExprVector : public RC::Vector< RC::ConstHandle<Expr> >
    {
    public:
      
      static RC::Handle<ExprVector> Create();
      static RC::Handle<ExprVector> Create( RC::ConstHandle<Expr> const &first );
      static RC::Handle<ExprVector> Create( RC::ConstHandle<Expr> const &first, RC::ConstHandle<ExprVector> const &remaining );

      RC::Handle<JSON::Array> toJSON() const;
          
      void appendTypes( CG::BasicBlockBuilder const &basicBlockBuilder, std::vector< RC::ConstHandle<CG::Adapter> > &argTypes ) const;
      void appendExprValues( CG::BasicBlockBuilder &basicBlockBuilder, std::vector<CG::Usage> const &usages, std::vector<CG::ExprValue> &result, std::string const &lValueErrorDesc ) const;
    
    protected:
    
      ExprVector();
    };
  };
};

#endif //_FABRIC_AST_EXPR_VECTOR_H
