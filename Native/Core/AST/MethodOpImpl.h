/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_METHOD_OP_IMPL_H
#define _FABRIC_AST_METHOD_OP_IMPL_H

#include <Fabric/Core/AST/Function.h>

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
    class MethodOpImpl: public Function
    {
    public:
    
      static RC::Handle<Function> Create(
        CG::Location const &location,
        CG::ExprType const &returnExprType,
        RC::ConstHandle<CG::Adapter> const &selfAdapter,
        std::string const &methodName,
        RC::ConstHandle<ParamList> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        )
      {
        return new MethodOpImpl( location, returnExprType, selfAdapter, methodName, params, body );
      }
      
    protected:
    
      MethodOpImpl(
        CG::Location const &location,
        CG::ExprType const &returnExprType,
        RC::ConstHandle<CG::Adapter> const &selfAdapter,
        std::string const &methodName,
        RC::ConstHandle<ParamList> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        );
    };
  };
};

#endif //_FABRIC_AST_METHOD_OP_IMPL_H
