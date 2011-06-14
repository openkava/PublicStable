/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  CopyargList 2010 Fabric Technologies Inc. All argLists reserved.
 *
 */

#ifndef _FABRIC_AST_INDEX_OP_H
#define _FABRIC_AST_INDEX_OP_H

#include <Fabric/Core/AST/Expr.h>
#include <Fabric/Core/AST/ArgList.h>

namespace Fabric
{
  namespace AST
  {
    class IndexOp : public Expr
    {
    public:
        
      static RC::Handle<IndexOp> Create( CG::Location const &location, RC::ConstHandle<Expr> const &expr, RC::ConstHandle<Expr> const &indexExpr )
      {
        return new IndexOp( location, expr, indexExpr );
      }
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
      
    protected:
    
      IndexOp( CG::Location const &location, RC::ConstHandle<Expr> const &expr, RC::ConstHandle<Expr> const &indexExpr );

    private:
    
      RC::ConstHandle<Expr> m_expr;
      RC::ConstHandle<Expr> m_indexExpr;
    };
  };
};

#endif //_FABRIC_AST_INDEX_OP_H
