/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_ASSIGN_OP_IMPL_H
#define _FABRIC_AST_ASSIGN_OP_IMPL_H

#include <Fabric/Core/AST/MethodOpImpl.h>
#include <Fabric/Core/CG/OpTypes.h>

namespace Fabric
{
  namespace AST
  {
    class CompoundStatement;
    class Param;
    
    class AssignOpImpl : public MethodOpImpl
    {
      FABRIC_AST_NODE_DECL( AssignOpImpl );
      
    public:
    
      static RC::ConstHandle<AssignOpImpl> Create(
        CG::Location const &location,
        std::string const &thisType,
        CG::AssignOpType assignOpType,
        RC::ConstHandle<Param> rhs,
        RC::ConstHandle<CompoundStatement> const &body
        );
      
    protected:
    
      AssignOpImpl(
        CG::Location const &location,
        std::string const &thisType,
        CG::AssignOpType assignOpType,
        RC::ConstHandle<Param> rhs,
        RC::ConstHandle<CompoundStatement> const &body
        );
      
      virtual void appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const;
    
    private:
    
      CG::AssignOpType m_assignOpType;
    };
  };
};

#endif //_FABRIC_AST_ASSIGN_OP_IMPL_H
