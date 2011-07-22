/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/AST/CaseVector.h>
#include <Fabric/Core/AST/Case.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace AST
  {
    RC::ConstHandle<CaseVector> CaseVector::Create( RC::ConstHandle<Case> const &first, RC::ConstHandle<CaseVector> const &remaining )
    {
      CaseVector *result = new CaseVector;
      if ( first )
        result->push_back( first );
      if ( remaining )
      {
        for ( const_iterator it=remaining->begin(); it!=remaining->end(); ++it )
          result->push_back( *it );
      }
      return result;
    }
    
    CaseVector::CaseVector()
    {
    }
    
    RC::Handle<JSON::Array> CaseVector::toJSON() const
    {
      RC::Handle<JSON::Array> result = JSON::Array::Create();
      for ( const_iterator it=begin(); it!=end(); ++it )
        result->push_back( (*it)->toJSON() );
      return result;
    }
    
    void CaseVector::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      for ( const_iterator it=begin(); it!=end(); ++it )
        (*it)->llvmPrepareModule( moduleBuilder, diagnostics );
    }
  };
};
