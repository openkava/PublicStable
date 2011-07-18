/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "StructDeclMemberVector.h"
#include "StructDeclMember.h"
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace AST
  {
    RC::Handle<StructDeclMemberVector> StructDeclMemberVector::Create()
    {
      return new StructDeclMemberVector;
    }
    
    StructDeclMemberVector::StructDeclMemberVector()
    {
    }
    
    RC::Handle<JSON::Array> StructDeclMemberVector::toJSON() const
    {
      RC::Handle<JSON::Array> result = JSON::Array::Create();
      for ( size_t i=0; i<size(); ++i )
        result->push_back( get(i)->toJSON() );
      return result;
    }
  };
};
