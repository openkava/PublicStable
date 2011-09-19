/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  CopyargList 2010 Fabric Technologies Inc. All argLists reserved.
 *
 */

#ifndef _FABRIC_AST_METHOD_OP_H
#define _FABRIC_AST_METHOD_OP_H

#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace CG
  {
    class FunctionSymbol;
  };
  
  namespace AST
  {
    class ExprVector;
    
    class MethodOp : public Expr
    {
      FABRIC_AST_NODE_DECL( MethodOp );

    public:
        
      static RC::ConstHandle<MethodOp> Create(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<Expr> const &expr,
        RC::ConstHandle<ExprVector> const &args
        );

      virtual void registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
      
    protected:
    
      MethodOp(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<Expr> const &expr,
        RC::ConstHandle<ExprVector> const &args
        );
      
      virtual void appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const;
      
      RC::ConstHandle<CG::FunctionSymbol> getFunctionSymbol( CG::BasicBlockBuilder &basicBlockBuilder ) const;

    private:
    
      std::string m_name;
      RC::ConstHandle<Expr> m_expr;
      RC::ConstHandle<ExprVector> m_args;
    };
  };
};

#endif //_FABRIC_AST_METHOD_OP_H
