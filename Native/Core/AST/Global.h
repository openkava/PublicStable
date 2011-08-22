/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_GLOBAL_H
#define _FABRIC_AST_GLOBAL_H

#include <Fabric/Core/AST/Node.h>
#include <Fabric/Core/AST/ParamVector.h>
#include <Fabric/Core/AST/CompoundStatement.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace llvm
{
  class Module;
  class GlobalPassManager;
};

namespace Fabric
{
  namespace RT
  {
    class Manager;
  };
  
  namespace CG
  {
    class Diagnostics;
    class ModuleBuilder;
  };
  
  namespace AST
  {
    class Global : public Node
    {
    public:
    
      virtual void appendJSON( Util::JSONGenerator const &jsonGenerator ) const;
    
      virtual bool isFunction() const { return false; }
      
      virtual void registerTypes( RC::Handle<RT::Manager> const &rtManager, CG::Diagnostics &diagnostics ) const;
      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const = 0;
      virtual void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const;
      
    protected:
    
      Global( CG::Location const &location );
      
    private:
    
      mutable Util::SimpleString m_json;
    };
  };
};

#endif //_FABRIC_AST_GLOBAL_H
