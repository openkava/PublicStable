/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/ArgList.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Error.h>

namespace Fabric
{
  namespace AST
  {
    ArgList::ArgList( CG::Location const &location )
      : ListNode( location )
    {
    }
    
    ArgList::ArgList( CG::Location const &location, RC::ConstHandle<Expr> const &item )
      : ListNode( location, item )
    {
    }
    
    ArgList::ArgList( CG::Location const &location, RC::ConstHandle<Expr> const &item, RC::ConstHandle<ArgList> const &remaining )
      : ListNode( location, item, remaining )
    {
    }
    
    std::string ArgList::localDesc() const
    {
      return "ArgList";
    }

    void ArgList::appendTypes( CG::BasicBlockBuilder const &basicBlockBuilder, std::vector< RC::ConstHandle<CG::Adapter> > &argTypes ) const
    {
      for ( size_t i=0; i<numItems(); ++i )
      {
        RC::ConstHandle<Expr> arg = item(i);
        argTypes.push_back( arg->getType( basicBlockBuilder ) );
      }
    }

    void ArgList::appendExprValues( CG::BasicBlockBuilder &basicBlockBuilder, std::vector<CG::Usage> const &usages, std::vector<CG::ExprValue> &result, std::string const &lValueErrorDesc ) const
    {
      size_t size = numItems();
      FABRIC_ASSERT( usages.size() == size );
      for ( size_t i=0; i<size; ++i )
      {
        RC::ConstHandle<Expr> arg = item(i);
        CG::ExprValue exprValue;
        try
        {
          exprValue = arg->buildExprValue( basicBlockBuilder, usages[i], "cannot be used as an io argument" );
        }
        catch ( CG::Error e )
        {
          throw "parameter "+_(i+1)+": " + e;
        }
        result.push_back( exprValue );
      }
    }
  };
};
