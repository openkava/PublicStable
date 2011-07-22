/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_FUNCTION_H
#define _FABRIC_AST_FUNCTION_H

#include <Fabric/Core/AST/FunctionBase.h>
#include <Fabric/Core/CG/ExprType.h>

namespace Fabric
{
  namespace AST
  {
    class Function : public FunctionBase
    {
      FABRIC_AST_NODE_DECL( Function );

    public:

      static RC::ConstHandle<Function> Create(
        CG::Location const &location,
        std::string const &friendlyName,
        std::string const &entryName,
        std::string const &returnTypeName,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        );

      static RC::ConstHandle<Function> Create(
        CG::Location const &location,
        std::string const &friendlyName,
        std::string const *entryName,
        std::string const &returnTypeName,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        );

      RC::Handle<JSON::Object> toJSONImpl() const;
          
      virtual bool isFunction() const { return true; }
      virtual bool isOperator() const { return false; }
      
      std::string const &getDeclaredName() const
      {
        return m_friendlyName;
      }
      virtual std::string const *getFriendlyName( RC::Handle<CG::Manager> const &cgManager ) const;
      virtual std::string getEntryName( RC::Handle<CG::Manager> const &cgManager ) const;
      virtual RC::ConstHandle<ParamVector> getParams( RC::Handle<CG::Manager> const &cgManager ) const;
      
    protected:
    
      Function(
        CG::Location const &location,
        std::string const &friendlyName,
        std::string const &entryName,
        std::string const &returnTypeName,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        );
    
    private:
    
      std::string m_friendlyName;
      std::string m_entryName;
      RC::ConstHandle<ParamVector> m_params;
    };
  };
};

#endif //_FABRIC_AST_FUNCTION_H
