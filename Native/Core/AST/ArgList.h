/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_ARG_LIST_NODE_H
#define _FABRIC_AST_ARG_LIST_NODE_H

#include <Fabric/Core/AST/ListNode.h>
#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace AST
  {
    class ArgList: public ListNode
    {
    public:
    
      virtual std::string localDesc() const;

      static RC::Handle<ArgList> Create( CG::Location const &location )
      {
        return new ArgList( location );
      }
      
      static RC::Handle<ArgList> Create( CG::Location const &location, RC::ConstHandle<Expr> const &item )
      {
        return new ArgList( location, item );
      }
      
      static RC::Handle<ArgList> Create( CG::Location const &location, RC::ConstHandle<Expr> const &item, RC::ConstHandle<ArgList> const &remaining )
      {
        return new ArgList( location, item, remaining );
      }
      
      RC::ConstHandle<Expr> item( size_t index ) const
      {
        return RC::ConstHandle<Expr>::StaticCast( ListNode::item(index) );
      }
      
      void appendTypes( CG::BasicBlockBuilder const &basicBlockBuilder, std::vector< RC::ConstHandle<CG::Adapter> > &argTypes ) const;
      void appendExprValues( CG::BasicBlockBuilder &basicBlockBuilder, std::vector<CG::Usage> const &usages, std::vector<CG::ExprValue> &result, std::string const &lValueErrorDesc ) const;
      
    protected:
    
      ArgList( CG::Location const &location );
      ArgList( CG::Location const &location, RC::ConstHandle<Expr> const &item );
      ArgList( CG::Location const &location, RC::ConstHandle<Expr> const &item, RC::ConstHandle<ArgList> const &next );
    };
  };
  
  inline std::string _( RC::ConstHandle<AST::ArgList> const &argList )
  {
    return argList->localDesc();
  }
};

#endif //_FABRIC_AST_ARG_LIST_NODE_H
