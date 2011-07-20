/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_INITIALIZER_H
#define _FABRIC_AST_INITIALIZER_H

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
    class Initializer : public FunctionBase
    {
      FABRIC_AST_NODE_DECL( Initializer );

    public:
    
      static RC::Handle<Initializer> Create(
        CG::Location const &location,
        std::string const &selfType,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        );
          
      virtual std::string getEntryName( RC::Handle<CG::Manager> const &cgManager ) const;
      
    protected:
    
      Initializer(
        CG::Location const &location,
        std::string const &selfType,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        );
    
    private:
    
      std::string m_selfType;
      RC::ConstHandle<ParamVector> m_params;
    };
  };
};

#endif //_FABRIC_AST_INITIALIZER_H
