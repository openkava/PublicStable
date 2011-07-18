/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_PARAM_H
#define _FABRIC_AST_PARAM_H

#include <Fabric/Core/AST/Node.h>
#include <Fabric/Core/CG/Location.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Scope.h>

namespace llvm
{
  class Value;
}

namespace Fabric
{
  namespace AST
  {
    class Param: public Node
    {
      FABRIC_AST_NODE_DECL( Param );

    public:
    
      static RC::Handle<Param> Create( CG::Location const &location, std::string const &name, RC::ConstHandle< CG::Adapter > const &adapter, CG::Usage usage )
      {
        return new Param( location, name, adapter, usage );
      }

      RC::Handle<JSON::Object> toJSON() const;
    
      std::string const &getName() const
      {
        return m_functionParam.getName();
      }
      
      RC::ConstHandle< CG::Adapter > getAdapter() const
      {
        return m_functionParam.getAdapter();
      }
      
      CG::Usage getUsage() const
      {
        return m_functionParam.getUsage();
      }
      
      RC::ConstHandle<CG::Adapter> getType() const
      {
        return m_functionParam.getAdapter();
      }
      
      CG::ExprType const &getExprType() const
      {
        return m_functionParam.getExprType();
      }
      
      llvm::Type const *getLLVMType() const
      {
        return m_functionParam.getLLVMType();
      }
      
      CG::FunctionParam const &getFunctionParam() const
      {
        return m_functionParam;
      }

    protected:
    
      Param( CG::Location const &location, std::string const &name, RC::ConstHandle< CG::Adapter > const &adapter, CG::Usage usage );
      
    private:
    
      CG::FunctionParam m_functionParam;
    };
  };
};

#endif //_FABRIC_AST_PARAM_H
