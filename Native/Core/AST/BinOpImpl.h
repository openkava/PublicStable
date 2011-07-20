/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_BIN_OP_IMPL_H
#define _FABRIC_AST_BIN_OP_IMPL_H

#include <Fabric/Core/AST/FunctionBase.h>
#include <Fabric/Core/CG/OpTypes.h>

namespace llvm
{
  class Module;
  class FunctionPassManager;
};

namespace Fabric
{
  namespace RT
  {
  };
  
  namespace AST
  {
    class BinOpImpl : public FunctionBase
    {
      FABRIC_AST_NODE_DECL( BinOpImpl );
      
    public:
    
      static RC::Handle<BinOpImpl> Create(
        CG::Location const &location,
        std::string const &returnType,
        CG::BinOpType binOpType,
        RC::ConstHandle<AST::Param> const &lhs,
        RC::ConstHandle<AST::Param> const &rhs,
        RC::ConstHandle<CompoundStatement> const &body
        );

      RC::Handle<JSON::Object> toJSON() const;
                  
      virtual std::string getEntryName( RC::Handle<CG::Manager> const &cgManager ) const;
      
    protected:
    
      BinOpImpl(
        CG::Location const &location,
        std::string const &returnType,
        CG::BinOpType binOpType,
        RC::ConstHandle<AST::Param> const &lhs,
        RC::ConstHandle<AST::Param> const &rhs,
        RC::ConstHandle<CompoundStatement> const &body
        );
        
    private:
    
      CG::BinOpType m_binOpType;
    };
  };
};

#endif //_FABRIC_AST_BIN_OP_IMPL_H
