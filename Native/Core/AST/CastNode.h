/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_CAST_NODE_H
#define _FABRIC_AST_CAST_NODE_H

#include <Fabric/Core/AST/Expr.h>

namespace llvm
{
  class Value;
};

namespace Fabric
{
  namespace AST
  {
    class CastNode: public Expr
    {
    public:

      static RC::Handle<CastNode> Create( CG::Location const &location, RC::ConstHandle< CG::Adapter > const &adapter, RC::ConstHandle<Expr> const &child )
      {
        return new CastNode( location, adapter, child );
      }
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
      
    protected:
    
      CastNode( CG::Location const &location, RC::ConstHandle< CG::Adapter > const &adapter, RC::ConstHandle<Expr> const &child );
    
    private:
    
      RC::ConstHandle<CG::Adapter> m_adapter;
      RC::ConstHandle<Expr> m_child;
    };
  };
};

#endif //_FABRIC_AST_CAST_NODE_H
