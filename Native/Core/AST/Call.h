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
    class FunctionSymbol;
  };
  
  namespace AST
  {
    class ExprVector;
    
    class Call : public Expr
    {
      FABRIC_AST_NODE_DECL( Call );
      
    public:
        
      static RC::Handle<Call> Create(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<ExprVector> const &args
        );

      RC::Handle<JSON::Object> toJSON() const;
      
      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
      
    protected:
    
      Call(
        CG::Location const &location,
        std::string const &name,
        RC::ConstHandle<ExprVector> const &args
        );
      
      RC::ConstHandle<CG::FunctionSymbol> getFunctionSymbol( CG::BasicBlockBuilder const &basicBlockBuilder ) const;

    private:
    
      std::string m_name;
      RC::ConstHandle<ExprVector> m_args;
    };
  };
};

#endif //_FABRIC_AST_CALL_H
