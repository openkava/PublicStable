/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "ExprVector.h"
#include <Fabric/Core/AST/Expr.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/Util/Assert.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace AST
  {
    RC::Handle<ExprVector> ExprVector::Create()
    {
      return new ExprVector;
    }
    
    RC::Handle<ExprVector> ExprVector::Create( RC::ConstHandle<Expr> const &first )
    {
      RC::Handle<ExprVector> result = Create();
      result->push_back( first );
      return result;
    }
    
    RC::Handle<ExprVector> ExprVector::Create( RC::ConstHandle<Expr> const &first, RC::ConstHandle<ExprVector> const &remaining )
    {
      RC::Handle<ExprVector> result = Create( first );
      for ( ExprVector::const_iterator it=remaining->begin(); it!=remaining->end(); ++it )
        result->push_back( *it );
      return result;
    }
    
    ExprVector::ExprVector()
    {
    }
    
    RC::Handle<JSON::Array> ExprVector::toJSON() const
    {
      RC::Handle<JSON::Array> result = JSON::Array::Create();
      for ( size_t i=0; i<size(); ++i )
        result->push_back( get(i)->toJSON() );
      return result;
    }

    void ExprVector::appendTypes( CG::BasicBlockBuilder const &basicBlockBuilder, std::vector< RC::ConstHandle<CG::Adapter> > &argTypes ) const
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
