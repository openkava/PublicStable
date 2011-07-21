/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_CONST_STRING_H
#define _FABRIC_AST_CONST_STRING_H

#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace CG
  {
    class ConstStringAdapter;
  };
  
  namespace AST
  {
    class ConstString : public Expr
    {
      FABRIC_AST_NODE_DECL( ConstString );

    public:
    
      static RC::Handle<ConstString> Create( CG::Location const &location, std::string const &value );

      RC::Handle<JSON::Object> toJSON() const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
        
    protected:
    
      ConstString( CG::Location const &location, std::string const &value );
      
    private:
    
      std::string m_value;
    };
  };
};

#endif //_FABRIC_AST_CONST_STRING_H
