/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_FUNCTION_BASE_H
#define _FABRIC_AST_FUNCTION_BASE_H

#include <Fabric/Core/AST/Global.h>
#include <Fabric/Core/AST/CompoundStatement.h>

namespace llvm
{
  class Module;
  class FunctionPassManager;
};

namespace Fabric
{
  namespace CG
  {
    class Manager;
  };
  
  namespace AST
  {
    class ParamVector;
    
    class FunctionBase : public Global
    {
    public:
      
      virtual std::string const *getFriendlyName( RC::Handle<CG::Manager> const &cgManager ) const;
      virtual std::string getEntryName( RC::Handle<CG::Manager> const &cgManager ) const = 0;
      virtual RC::ConstHandle<ParamVector> getParams( RC::Handle<CG::Manager> const &cgManager ) const = 0;

      std::string const &getReturnType() const
      {
        return m_returnTypeName;
      }
      RC::ConstHandle<CompoundStatement> getBody() const;
      
      virtual void registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const;
      virtual void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const;
      
    protected:
    
      FunctionBase(
        CG::Location const &location,
        std::string const &returnTypeName,
        RC::ConstHandle<CompoundStatement> const &body,
        bool exportSymbol = false
        );
      
      virtual void appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const;
    
    private:
    
      std::string m_returnTypeName;
      RC::ConstHandle<CompoundStatement> m_body;
      bool m_exportSymbol;
    };
  };
};

#endif //_FABRIC_AST_FUNCTION_BASE_H
