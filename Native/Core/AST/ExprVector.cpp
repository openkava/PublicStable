/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "ExprVector.h"
#include <Fabric/Core/AST/Expr.h>
#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/Util/Assert.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    RC::ConstHandle<ExprVector> ExprVector::Create( RC::ConstHandle<Expr> const &first, RC::ConstHandle<ExprVector> const &remaining )
    {
      ExprVector *result = new ExprVector;
      if ( first )
        result->push_back( first );
      if ( remaining )
      {
        for ( ExprVector::const_iterator it=remaining->begin(); it!=remaining->end(); ++it )
          result->push_back( *it );
      }
      return result;
    }
    
    ExprVector::ExprVector()
    {
    }
    
    void ExprVector::appendJSON( Util::JSONGenerator const &jsonGenerator ) const
    {
      Util::JSONArrayGenerator jsonArrayGenerator = jsonGenerator.makeArray();
      for ( const_iterator it=begin(); it!=end(); ++it )
        (*it)->appendJSON( jsonArrayGenerator.makeElement() );
    }

    void ExprVector::appendTypes( CG::BasicBlockBuilder &basicBlockBuilder, std::vector< RC::ConstHandle<CG::Adapter> > &argTypes ) const
    {
      for ( size_t i=0; i<size(); ++i )
      {
        RC::ConstHandle<Expr> const &arg = get(i);
        argTypes.push_back( arg->getType( basicBlockBuilder ) );
      }
    }

    void ExprVector::appendExprValues( CG::BasicBlockBuilder &basicBlockBuilder, std::vector<CG::Usage> const &usages, std::vector<CG::ExprValue> &result, std::string const &lValueErrorDesc ) const
    {
      FABRIC_ASSERT( usages.size() == size() );
      for ( size_t i=0; i<size(); ++i )
      {
        RC::ConstHandle<Expr> const &arg = get(i);
        CG::ExprValue exprValue( basicBlockBuilder.getContext() );
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
    
    void ExprVector::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      for ( const_iterator it=begin(); it!=end(); ++it )
        (*it)->registerTypes( cgManager, diagnostics );
    }
  };
};
