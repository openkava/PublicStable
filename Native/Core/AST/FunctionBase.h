/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_FUNCTION_BASE_H
#define _FABRIC_AST_FUNCTION_BASE_H

#include <Fabric/Core/AST/Global.h>
#include <Fabric/Core/CG/ExprType.h>

namespace llvm
{
  class Module;
  class FunctionPassManager;
};

namespace Fabric
{
  namespace AST
  {
    class CompoundStatement;
    class ParamVector;
    
    class FunctionBase : public Global
    {
    public:

      RC::Handle<JSON::Object> toJSON() const;
      
      virtual bool isFunction() const { return true; }
          
      virtual std::string const *getFriendlyName() const = 0;
      virtual std::string const &getEntryName( RC::Handle<CG::Manager> const &cgManager ) const = 0;

      virtual bool isOperator() const
      {
        return false;
      }
      
      RC::ConstHandle<ParamVector> getParams() const;
      RC::ConstHandle<CompoundStatement> getBody() const;
      
      virtual void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const;
      
    protected:
    
      FunctionBase(
        CG::Location const &location,
        std::string const &returnTypeName,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        );
    
    private:
    
      std::string m_returnTypeName;
      RC::ConstHandle<ParamVector> m_params;
      RC::ConstHandle<CompoundStatement> m_body;
    };
  };
};

#endif //_FABRIC_AST_FUNCTION_BASE_H
