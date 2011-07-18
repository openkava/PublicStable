/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/AST/StatementVector.h>
#include <Fabric/Core/AST/Statement.h>
#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace AST
  {
    RC::Handle<StatementVector> StatementVector::Create()
    {
      return new StatementVector;
    }
    
    StatementVector::StatementVector()
    {
    }
    
    RC::Handle<JSON::Array> StatementVector::toJSON() const
    {
      RC::Handle<JSON::Array> result = JSON::Array::Create();
      for ( size_t i=0; i<size(); ++i )
        result->push_back( get(i)->toJSON() );
      return result;
    }
    
    void StatementVector::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      for ( size_t i=0; i<size(); ++i )
      {
        RC::ConstHandle<Statement> const &statement = get(i);
        if ( basicBlockBuilder->GetInsertBlock()->getTerminator() )
        {
          statement->addError( diagnostics, "code is unreachable" );
          break;
        }
        statement->llvmCompileToBuilder( basicBlockBuilder, diagnostics );
      }
    }
  };
};
