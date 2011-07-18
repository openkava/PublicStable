/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_PARAM_VECTOR_H
#define _FABRIC_AST_PARAM_VECTOR_H

#include <Fabric/Base/RC/Vector.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace llvm
{
  class Type;
};

namespace Fabric
{
  namespace JSON
  {
    class Array;
  };
  
  namespace CG
  {
    class Adapter;
    class ExprType;
    class FunctionParam;
  };

  namespace AST
  {
    class Param;
    
    class ParamVector : public RC::Vector< RC::ConstHandle<Param> >
    {
    public:
      
      static RC::Handle<ParamVector> Create();
      static RC::Handle<ParamVector> Create( RC::ConstHandle<Param> const &firstParam );
      static RC::Handle<ParamVector> Create( RC::ConstHandle<Param> const &firstParam, RC::ConstHandle<Param> const &secondParam );
      static RC::Handle<ParamVector> Create( RC::ConstHandle<Param> const &firstParam, RC::ConstHandle<ParamVector> const &remainingParams );

      RC::Handle<JSON::Array> toJSON() const;
    
      std::vector<llvm::Type const *> getLLVMTypes() const;
      std::string getTypeDesc() const;
      std::vector<CG::FunctionParam> getFunctionParams() const;
      std::vector< RC::ConstHandle<CG::Adapter> > getTypes() const;
      std::vector<CG::ExprType> getExprTypes() const;
      
    protected:
    
      ParamVector();
    };
  };
};

#endif //_FABRIC_AST_PARAM_VECTOR_H
