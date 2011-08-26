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
  namespace Util
  {
    class SimpleString;
  };
  
  namespace CG
  {
    class FunctionSymbol;
  };
  
  namespace AST
  {
    class UniOp : public Expr
    {
      FABRIC_AST_NODE_DECL( UniOp );

    public:
    
      static RC::ConstHandle<UniOp> Create( CG::Location const &location, CG::UniOpType uniOpType, RC::ConstHandle<Expr> const &child )
      {
        return new UniOp( location, uniOpType, child );
      }

      virtual void registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
        
    protected:
    
      UniOp( CG::Location const &location, CG::UniOpType uniOpType, RC::ConstHandle<Expr> const &child );
      
      virtual void appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const;
      
      RC::ConstHandle<CG::FunctionSymbol> getFunctionSymbol( CG::BasicBlockBuilder &basicBlockBuilder ) const;

    private:
    
      CG::UniOpType m_uniOpType;
      RC::ConstHandle<Expr> m_child;
    };
  };
};

#endif //_FABRIC_AST_UNI_OP_H
