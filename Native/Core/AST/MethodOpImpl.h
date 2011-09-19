/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_METHOD_OP_IMPL_H
#define _FABRIC_AST_METHOD_OP_IMPL_H

#include <Fabric/Core/AST/FunctionBase.h>

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
    class MethodOpImpl : public FunctionBase
    {
      FABRIC_AST_NODE_DECL( MethodOpImpl );

    public:
    
      static RC::ConstHandle<MethodOpImpl> Create(
        CG::Location const &location,
        std::string const &returnTypeName,
        std::string const &thisTypeName,
        std::string const &methodName,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        );
          
      virtual std::string getEntryName( RC::Handle<CG::Manager> const &cgManager ) const;
      virtual RC::ConstHandle<ParamVector> getParams( RC::Handle<CG::Manager> const &cgManager ) const;
              
    protected:
    
      MethodOpImpl(
        CG::Location const &location,
        std::string const &returnTypeName,
        std::string const &thisTypeName,
        std::string const &methodName,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        );
      
      virtual void appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const;
        
    private:
    
      std::string m_selfTypeName;
      std::string m_methodName;
      RC::ConstHandle<ParamVector> m_params;
    };
  };
};

#endif //_FABRIC_AST_METHOD_OP_IMPL_H
