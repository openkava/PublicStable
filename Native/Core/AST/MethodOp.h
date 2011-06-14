/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  CopyargList 2010 Fabric Technologies Inc. All argLists reserved.
 *
 */

#ifndef _FABRIC_AST_METHOD_OP_H
#define _FABRIC_AST_METHOD_OP_H

#include <Fabric/Core/AST/Expr.h>
#include <Fabric/Core/AST/ArgList.h>

namespace Fabric
{
  namespace CG
  {
    class FunctionSymbol;
  };
  
  namespace AST
  {
    class MethodOp : public Expr
    {
    public:
        
      static RC::Handle<MethodOp> Create( CG::Location const &location, std::string const &name, RC::ConstHandle<Expr> const &expr, RC::ConstHandle<ArgList> const &argList )
      {
        return new MethodOp( location, name, expr, argList );
      }
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
      
    protected:
    
      MethodOp( CG::Location const &location, std::string const &name, RC::ConstHandle<Expr> const &expr, RC::ConstHandle<ArgList> const &argList );
      
      RC::ConstHandle<CG::FunctionSymbol> getFunctionSymbol( CG::BasicBlockBuilder const &basicBlockBuilder ) const;

    private:
    
      std::string m_name;
      RC::ConstHandle<Expr> m_expr;
      RC::ConstHandle<ArgList> m_argList;
    };
  };
};

#endif //_FABRIC_AST_METHOD_OP_H
