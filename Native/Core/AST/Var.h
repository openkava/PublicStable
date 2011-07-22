/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_VAR_H
#define _FABRIC_AST_VAR_H

#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace CG
  {
    class ValueSymbol;
  };
  
  namespace AST
  {
    class Var : public Expr
    {
      FABRIC_AST_NODE_DECL( Var );

    public:
    
      static RC::Handle<Var> Create( CG::Location const &location, std::string const &name )
      {
        return new Var( location, name );
      }

      RC::Handle<JSON::Object> toJSON() const;

      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
        
    protected:
    
      Var( CG::Location const &location, std::string const &name );
      
      RC::ConstHandle<CG::ValueSymbol> getValueSymbol( CG::BasicBlockBuilder const &basicBlockBuilder ) const;
      
    private:
    
      std::string m_name;
    };
  };
};

#endif //_FABRIC_AST_VAR_H
