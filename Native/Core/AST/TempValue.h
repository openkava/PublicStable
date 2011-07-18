/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_TEMP_VALUE_H
#define _FABRIC_AST_TEMP_VALUE_H

#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace AST
  {
    class ExprVector;
    
    class TempValue : public Expr
    {
      FABRIC_AST_NODE_DECL( TempValue );

    public:
        
      static RC::Handle<TempValue> Create(
        CG::Location const &location,
        RC::ConstHandle<CG::Adapter> const &adapter,
        RC::ConstHandle<ExprVector> const &args
        );

      RC::Handle<JSON::Object> toJSON() const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
      
    protected:
    
      TempValue(
        CG::Location const &location,
        RC::ConstHandle<CG::Adapter> const &adapter,
        RC::ConstHandle<ExprVector> const &args
        );

    private:
    
      RC::ConstHandle<CG::Adapter> m_adapter;
      RC::ConstHandle<ExprVector> m_args;
    };
  };
};

#endif //_FABRIC_AST_TEMP_VALUE_H
