/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_GLOBAL_H
#define _FABRIC_AST_GLOBAL_H

#include <Fabric/Core/AST/Node.h>
#include <Fabric/Core/AST/ParamList.h>
#include <Fabric/Core/AST/CompoundStatement.h>

namespace llvm
{
  class Module;
  class GlobalPassManager;
};

namespace Fabric
{
  namespace AST
  {
    class Global : public Node
    {
    public:
    
      virtual bool isFunction() const { return false; }
          
      virtual void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const = 0;
      
    protected:
    
      Global( CG::Location const &location );
    };
  };
};

#endif //_FABRIC_AST_GLOBAL_H
