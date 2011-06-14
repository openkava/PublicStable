/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_UNI_OP_H
#define _FABRIC_AST_UNI_OP_H

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
    class UniOp : public Expr
    {
    public:
    
      static RC::Handle<UniOp> Create( CG::Location const &location, CG::UniOpType uniOpType, RC::ConstHandle<Expr> const &child )
      {
        return new UniOp( location, uniOpType, child );
      }
      
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
        
    protected:
    
      UniOp( CG::Location const &location, CG::UniOpType uniOpType, RC::ConstHandle<Expr> const &child );
      
      RC::ConstHandle<CG::FunctionSymbol> getFunctionSymbol( CG::BasicBlockBuilder const &basicBlockBuilder ) const;

    private:
    
      CG::UniOpType m_uniOpType;
      RC::ConstHandle<Expr> m_child;
    };
  };
};

#endif //_FABRIC_AST_UNI_OP_H
