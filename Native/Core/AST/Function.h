/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_FUNCTION_H
#define _FABRIC_AST_FUNCTION_H

#include <Fabric/Core/AST/Global.h>
#include <Fabric/Core/AST/ParamList.h>
#include <Fabric/Core/AST/CompoundStatement.h>

namespace llvm
{
  class Module;
  class FunctionPassManager;
};

namespace Fabric
{
  namespace AST
  {
    class Function: public Global
    {
    public:
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;

      static RC::Handle<Function> Create(
        CG::Location const &location,
        std::string const &name,
        CG::ExprType const &returnExprType,
        RC::ConstHandle<ParamList> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        )
      {
        return new Function( location, name, returnExprType, params, body );
      }
      
      virtual bool isFunction() const { return true; }
          
      std::string const &getName() const
      {
        return m_name;
      }

      virtual bool isOperator() const
      {
        return false;
      }
      
      RC::ConstHandle<ParamList> getParamList() const;
      RC::ConstHandle<CompoundStatement> getBody() const;
      
      virtual void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const;
      
    protected:
    
      Function(
        CG::Location const &location,
        std::string const &name,
        CG::ExprType const &returnExprType,
        RC::ConstHandle<ParamList> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        );
    
    private:
    
      std::string m_name;
      CG::ExprType m_returnExprType;
      RC::ConstHandle<ParamList> m_params;
      RC::ConstHandle<CompoundStatement> m_body;
    };
  };
};

#endif //_FABRIC_AST_FUNCTION_H
