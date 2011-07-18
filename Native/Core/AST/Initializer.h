/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_INITIALIZER_H
#define _FABRIC_AST_INITIALIZER_H

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
    class Initializer : public Function
    {
      FABRIC_AST_NODE_DECL( Initializer );

    public:
    
      static RC::Handle<Function> Create(
        CG::Location const &location,
        RC::ConstHandle<CG::Adapter> const &selfAdapter,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        );
      
    protected:
    
      Initializer(
        CG::Location const &location,
        RC::ConstHandle<CG::Adapter> const &selfAdapter,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        );
    };
  };
};

#endif //_FABRIC_AST_INITIALIZER_H
