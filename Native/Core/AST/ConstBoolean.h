/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_CONST_BOOLEAN_H
#define _FABRIC_AST_CONST_BOOLEAN_H

#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace AST
  {
    class ConstBoolean : public Expr
    {
      FABRIC_AST_NODE_DECL( ConstBoolean );

    public:
    
      static RC::ConstHandle<ConstBoolean> Create( CG::Location const &location, bool value );
      
      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
        
    protected:
    
      ConstBoolean( CG::Location const &location, bool value );
      
      virtual void appendJSONMembers( Util::SimpleString &ss ) const;
      
    private:
    
      bool m_value;
    };
  };
};

#endif //_FABRIC_AST_CONST_BOOLEAN_H
