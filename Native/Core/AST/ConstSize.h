/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
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
      
      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctions ) const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
        
    protected:
    
      ConstSize( CG::Location const &location, size_t value );
      
      virtual void appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const;
      
    private:
    
      size_t m_value;
    };
  };
};

#endif //_FABRIC_AST_CONST_SIZE_H
