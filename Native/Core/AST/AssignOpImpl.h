/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_ASSIGN_OP_IMPL_H
#define _FABRIC_AST_ASSIGN_OP_IMPL_H

#include <Fabric/Core/AST/MethodOpImpl.h>
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
    class AssignOpImpl: public MethodOpImpl
    {
    public:
    
      static RC::Handle<Function> Create(
        CG::Location const &location,
        RC::ConstHandle<CG::Adapter> const &selfType,
        CG::AssignOpType assignOpType,
        RC::ConstHandle< AST::Param > rhs,
        RC::ConstHandle<CompoundStatement> const &body
        )
      {
        return new AssignOpImpl( location, selfType, assignOpType, rhs, body );
      }
      
    protected:
    
      AssignOpImpl(
        CG::Location const &location,
        RC::ConstHandle<CG::Adapter> const &selfType,
        CG::AssignOpType assignOpType,
        RC::ConstHandle< AST::Param > rhs,
        RC::ConstHandle<CompoundStatement> const &body
        );
    };
  };
};

#endif //_FABRIC_AST_ASSIGN_OP_IMPL_H
