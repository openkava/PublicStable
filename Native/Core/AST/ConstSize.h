/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_AST_CONST_SIZE_H
#define _FABRIC_AST_CONST_SIZE_H

#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace AST
  {
    class ConstSize : public Expr
    {
      FABRIC_AST_NODE_DECL( ConstSize );

    public:
    
      static RC::ConstHandle<ConstSize> Create( CG::Location const &location, std::string const &valueString );
      
      virtual void registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const;
      
      virtual CG::ExprType getExprType( CG::BasicBlockBuilder &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
        
    protected:
    
      ConstSize( CG::Location const &location, size_t value );
      
      virtual void appendJSONMembers( JSON::ObjectEncoder const &jsonObjectEncoder, bool includeLocation ) const;
      
    private:
    
      size_t m_value;
    };
  };
};

#endif //_FABRIC_AST_CONST_SIZE_H
