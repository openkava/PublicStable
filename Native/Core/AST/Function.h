/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_FUNCTION_H
#define _FABRIC_AST_FUNCTION_H

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
    
    class Function: public Global
    {
      FABRIC_AST_NODE_DECL( Function );

    public:

      static RC::Handle<Function> Create(
        CG::Location const &location,
        std::string const &friendlyName,
        std::string const &entryName,
        CG::ExprType const &returnExprType,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        );
      
      static RC::Handle<Function> Create(
        CG::Location const &location,
        std::string const &friendlyName,
        std::string const *entryName,
        CG::ExprType const &returnExprType,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        )
      {
        return new Function( location, friendlyName, entryName? *entryName: friendlyName, returnExprType, params, body );
      }

      RC::Handle<JSON::Object> toJSON() const;
      
      virtual bool isFunction() const { return true; }
          
      std::string const &getFriendlyName() const
      {
        return m_friendlyName;
      }

      std::string const &getEntryName() const
      {
        return m_entryName;
      }

      virtual bool isOperator() const
      {
        return false;
      }
      
      RC::ConstHandle<ParamVector> getParams() const;
      RC::ConstHandle<CompoundStatement> getBody() const;
      
      virtual void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const;
      
    protected:
    
      Function(
        CG::Location const &location,
        std::string const &friendlyName,
        std::string const &entryName,
        CG::ExprType const &returnExprType,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        );
    
    private:
    
      std::string m_friendlyName;
      std::string m_entryName;
      CG::ExprType m_returnExprType;
      RC::ConstHandle<ParamVector> m_params;
      RC::ConstHandle<CompoundStatement> m_body;
    };
  };
};

#endif //_FABRIC_AST_FUNCTION_H
