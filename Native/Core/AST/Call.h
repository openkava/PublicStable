/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_CALL_H
#define _FABRIC_AST_CALL_H

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
    class Call : public Expr
    {
    public:
        
      static RC::Handle<Call> Create( CG::Location const &location, std::string const &name, RC::ConstHandle<ArgList> const &args )
      {
        return new Call( location, name, args );
      }
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
      
    protected:
    
      Call( CG::Location const &location, std::string const &name, RC::ConstHandle<ArgList> const &args );
      
      RC::ConstHandle<CG::FunctionSymbol> getFunctionSymbol( CG::BasicBlockBuilder const &basicBlockBuilder ) const;

    private:
    
      std::string m_name;
      RC::ConstHandle<ArgList> m_args;
    };
  };
};

#endif //_FABRIC_AST_CALL_H
