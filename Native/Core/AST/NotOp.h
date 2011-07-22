/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_NOT_OP_H
#define _FABRIC_AST_NOT_OP_H

#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace AST
  {
    class NotOp : public Expr
    {
      FABRIC_AST_NODE_DECL( NotOp );

    public:
        
      static RC::ConstHandle<NotOp> Create( CG::Location const &location, RC::ConstHandle<Expr> const &child )
      {
        return new NotOp( location, child );
      }

      RC::Handle<JSON::Object> toJSON() const;

      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
      
    protected:
    
      NotOp( CG::Location const &location, RC::ConstHandle<Expr> const &child );

    private:
    
      RC::ConstHandle<Expr> m_child;
    };
  };
};

#endif //_FABRIC_AST_NOT_OP_H
