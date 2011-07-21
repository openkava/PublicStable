/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_BIN_OP_H
#define _FABRIC_AST_BIN_OP_H

#include <Fabric/Core/AST/Expr.h>
#include <Fabric/Core/CG/OpTypes.h>

namespace Fabric
{
  namespace CG
  {
    class FunctionSymbol;
  };
  
  namespace AST
  {
    class BinOp : public Expr
    {
      FABRIC_AST_NODE_DECL( BinOp );
      
    public:
        
      static RC::Handle<BinOp> Create( CG::Location const &location, CG::BinOpType binOpType, RC::ConstHandle<Expr> const &left, RC::ConstHandle<Expr> const &right )
      {
        return new BinOp( location, binOpType, left, right );
      }

      RC::Handle<JSON::Object> toJSON() const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
      
    protected:
    
      BinOp( CG::Location const &location, CG::BinOpType binOpType, RC::ConstHandle<Expr> const &left, RC::ConstHandle<Expr> const &right );
      
      RC::ConstHandle<CG::FunctionSymbol> getFunctionSymbol( CG::BasicBlockBuilder const &basicBlockBuilder ) const;

    private:
    
      CG::BinOpType m_binOpType;
      RC::ConstHandle<Expr> m_left;
      RC::ConstHandle<Expr> m_right;
    };
  };
};

#endif //_FABRIC_AST_BIN_OP_H
