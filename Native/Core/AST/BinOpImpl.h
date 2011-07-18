/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_BIN_OP_IMPL_H
#define _FABRIC_AST_BIN_OP_IMPL_H

#include <Fabric/Core/AST/Function.h>
#include <Fabric/Core/CG/OpTypes.h>

namespace llvm
{
  class Module;
  class FunctionPassManager;
};

namespace Fabric
{
  namespace RT
  {
  };
  
  namespace AST
  {
    class BinOpImpl : public Function
    {
      FABRIC_AST_NODE_DECL( BinOpImpl );
      
    public:
    
      static RC::Handle<Function> Create(
        CG::Location const &location,
        CG::ExprType const &returnExprType,
        CG::BinOpType binOpType,
        RC::ConstHandle< AST::Param > lhs,
        RC::ConstHandle< AST::Param > rhs,
        RC::ConstHandle<CompoundStatement> const &body
        )
      {
        return new BinOpImpl( location, returnExprType, binOpType, lhs, rhs, body );
      }
      
    protected:
    
      BinOpImpl(
        CG::Location const &location,
        CG::ExprType const &returnExprType,
        CG::BinOpType binOpType,
        RC::ConstHandle< AST::Param > lhs,
        RC::ConstHandle< AST::Param > rhs,
        RC::ConstHandle<CompoundStatement> const &body
        );
    };
  };
};

#endif //_FABRIC_AST_BIN_OP_IMPL_H
