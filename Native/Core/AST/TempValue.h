/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_TEMP_VALUE_H
#define _FABRIC_AST_TEMP_VALUE_H

#include <Fabric/Core/AST/Expr.h>
#include <Fabric/Core/AST/ArgList.h>

namespace Fabric
{
  namespace AST
  {
    class TempValue : public Expr
    {
    public:
        
      static RC::Handle<TempValue> Create( CG::Location const &location, RC::ConstHandle<CG::Adapter> const &adapter, RC::ConstHandle<ArgList> const &args )
      {
        return new TempValue( location, adapter, args );
      }
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
      
    protected:
    
      TempValue( CG::Location const &location, RC::ConstHandle<CG::Adapter> const &adapter, RC::ConstHandle<ArgList> const &args );

    private:
    
      RC::ConstHandle<CG::Adapter> m_adapter;
      RC::ConstHandle<ArgList> m_args;
    };
  };
};

#endif //_FABRIC_AST_TEMP_VALUE_H
