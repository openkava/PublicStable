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
    RC::Handle<CaseVector> CaseVector::Create()
    {
      return new CaseVector;
    }
    
    RC::Handle<CaseVector> CaseVector::Create( RC::ConstHandle<Case> const &first )
    {
      RC::Handle<CaseVector> result = Create();
      result->push_back( first );
      return result;
    }
    
    RC::Handle<CaseVector> CaseVector::Create( RC::ConstHandle<Case> const &first, RC::ConstHandle<CaseVector> const &remaining )
    {
      RC::Handle<CaseVector> result = Create( first );
      for ( CaseVector::const_iterator it=remaining->begin(); it!=remaining->end(); ++it )
        result->push_back( *it );
      return result;
    }
    
    CaseVector::CaseVector()
    {
    }
    
    RC::Handle<JSON::Array> CaseVector::toJSON() const
    {
      RC::Handle<JSON::Array> result = JSON::Array::Create();
      for ( size_t i=0; i<size(); ++i )
        result->push_back( get(i)->toJSON() );
      return result;
    }
  };
};
