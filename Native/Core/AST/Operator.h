// Operator.h
// *****************************************************************************
// (C) 2011 Fabric Technologies, Inc.

#ifndef __FABRIC_AST_OPERATOR_H__
#define __FABRIC_AST_OPERATOR_H__

#include <Fabric/Core/AST/Function.h>

namespace Fabric
{
  namespace AST
  {
    class Operator : public Function
    {
    public:
      static RC::Handle<Function> Create(
        CG::Location const &location,
        std::string const &friendlyName,
        std::string const &entryName,
        RC::ConstHandle<ParamList> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        )
      {
        return new Operator( location, friendlyName, entryName, params, body );
      }

      virtual std::string localDesc() const;

      virtual bool isOperator() const
      {
        return true;
      }

    protected:
    
      Operator( 
        CG::Location const &location,
        std::string const &friendlyName,
        std::string const &entryName,
        RC::ConstHandle<ParamList> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        );
    };
  }
}

#endif // __FABRIC_AST_OPERATOR_H__
