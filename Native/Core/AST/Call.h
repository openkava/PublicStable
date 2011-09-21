/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_CALL_H
#define _FABRIC_AST_CALL_H

#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace CG
  {
    class FunctionParam;
    class FunctionSymbol;
  };
  
  namespace AST
  {
    class ExprVector;
    
    class Call : public Expr
    {
      FABRIC_AST_NODE_DECL( Call );
      
    public:
        
      static RC::ConstHandle<Call> Create(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<ExprVector> const &args
        );
      
      virtual void registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
      
    protected:
    
      Call(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<ExprVector> const &args
        );
      
      virtual void appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const;
      
      RC::ConstHandle<CG::FunctionSymbol> getFunctionSymbol( CG::BasicBlockBuilder &basicBlockBuilder ) const;

    private:
    
      std::string m_name;
      RC::ConstHandle<ExprVector> m_args;
    };
  };
};

#endif //_FABRIC_AST_CALL_H
