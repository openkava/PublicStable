/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_STATEMENT_H
#define _FABRIC_AST_STATEMENT_H

#include <Fabric/Core/AST/Node.h>

namespace llvm
{
  class BasicBlock;
}

namespace Fabric
{
  namespace Util
  {
    class SimpleString;
  };
  
  namespace CG
  {
    class BasicBlockBuilder;
    class ModuleBuilder;
  };
  
  namespace AST
  {
    class Statement : public Node
    {
    public:
      
      Statement( CG::Location const &location );
      
      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const = 0;
      
      virtual void llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const = 0;
    };
  };
};

#endif //_FABRIC_AST_STATEMENT_H
